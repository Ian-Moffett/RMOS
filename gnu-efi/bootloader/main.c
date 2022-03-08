#include <efi.h>
#include <efilib.h>
#include <stddef.h>
#include <elf.h>

#define PSF1_MAGIC0 0x00000036
#define PSF1_MAGIC1 0x00000004


typedef struct {
    void* baseAddr;
    size_t bufferSize;
    unsigned int width;
    unsigned int height;
    unsigned int ppsl;      // Pixels per scanline.
} framebuffer_t;


typedef struct {
    unsigned char magic[2];
    unsigned char mode;
    unsigned char chsize;
} psf1_header_t;


typedef struct {
    psf1_header_t* header;
    void* glyphBuffer;
} psf1_font_t;


framebuffer_t* initGOP(EFI_SYSTEM_TABLE* sysTable) {
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    EFI_STATUS s = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);

    if (EFI_ERROR(s)) {
        return NULL;
    }

    framebuffer_t* fb;
    sysTable->BootServices->AllocatePool(EfiLoaderData, sizeof(framebuffer_t), (void**)&fb);
    fb->baseAddr = (void*)gop->Mode->FrameBufferBase;
    fb->bufferSize = gop->Mode->FrameBufferSize;
    fb->width = gop->Mode->Info->HorizontalResolution;
    fb->height = gop->Mode->Info->VerticalResolution;
    fb->ppsl = gop->Mode->Info->PixelsPerScanLine;
    return fb;
}


int memcmp(const void* aptr, const void* bptr, size_t n) {
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++) {
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}


EFI_FILE* loadFile(EFI_FILE* dir, CHAR16* path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* sysTable) {
    EFI_FILE* fileRes;
    EFI_LOADED_IMAGE_PROTOCOL* loadedImage;
    sysTable->BootServices->HandleProtocol(imageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loadedImage);
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs;
    sysTable->BootServices->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&fs);

    if (!(dir)) {
        fs->OpenVolume(fs, &dir);
    }

    EFI_STATUS s = dir->Open(dir, &fileRes, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

    if (s != EFI_SUCCESS) {
        return NULL;
    }

    return fileRes;
}


psf1_font_t* loadFont(EFI_FILE* dir, CHAR16* path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* sysTable) {
    EFI_FILE* font = loadFile(dir, path, imageHandle, sysTable);

    if (!(font)) {
        Print(L"Font file not found!\n");
        return NULL;
    }

    psf1_header_t* fontHeader;
    sysTable->BootServices->AllocatePool(EfiLoaderData, sizeof(psf1_header_t), (void**)&fontHeader);
    UINTN size = sizeof(psf1_header_t);
    font->Read(font, &size, fontHeader);

    if (!(fontHeader->magic[0] & PSF1_MAGIC0) || !(fontHeader->magic[1] & PSF1_MAGIC1)) {
        Print(L"Font header invalid!\n");
        return NULL;
    }

    UINTN glyphBufferSize = fontHeader->chsize * 255;

    if (fontHeader->mode == 1) {
        glyphBufferSize = fontHeader->chsize * 512;
    }

    void* glyphBuffer;
    font->SetPosition(font, sizeof(psf1_header_t));
    sysTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
    font->Read(font, &glyphBufferSize, glyphBuffer);

    psf1_font_t* fontRes;
    sysTable->BootServices->AllocatePool(EfiLoaderData, sizeof(psf1_font_t), (void**)&fontRes);
    fontRes->header = fontHeader;
    fontRes->glyphBuffer = glyphBuffer;
    return fontRes;
}


EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* sysTable) {
    InitializeLib(imageHandle, sysTable);

    EFI_FILE* kernel = loadFile(NULL, L"kernel.elf", imageHandle, sysTable);

    if (!(kernel)) {
        Print(L"Kernel does not exist!\n");
    } else {
        Elf64_Ehdr header;
        UINTN fileInfoSize;
        kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, NULL);
        UINTN size = sizeof(header);
        kernel->Read(kernel, &size, &header);

        if (memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
                header.e_ident[EI_CLASS] != ELFCLASS64 || 
                header.e_type != ET_EXEC || 
                header.e_machine != EM_X86_64 || header.e_version != EV_CURRENT) {
            Print(L"Kernel ELF header bad!\n");
        } else {
            Elf64_Phdr* progHeaders;
            kernel->SetPosition(kernel, header.e_phoff);
            UINTN progHeaderSize = header.e_phnum * header.e_phentsize;
            sysTable->BootServices->AllocatePool(EfiLoaderData, progHeaderSize, (void**)&progHeaders);
            kernel->Read(kernel, &progHeaderSize, progHeaders);

            for (Elf64_Phdr* phdr = progHeaders; (char*)phdr < (char*)progHeaders + header.e_phnum * header.e_phentsize; phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)) {
                if (phdr->p_type == PT_LOAD) {
                    int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
                    Elf64_Addr segment = phdr->p_paddr;
                    sysTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);
                    kernel->SetPosition(kernel, phdr->p_offset);
                    UINTN size = phdr->p_filesz;
                    kernel->Read(kernel, &size, (void*)segment);
                }
            }

            framebuffer_t* lfb = initGOP(sysTable);
            psf1_font_t* font = loadFont(NULL, L"zap-light16.psf", imageHandle, sysTable);
            void(*kernel_entry)(framebuffer_t*, psf1_font_t*) = ((__attribute__((sysv_abi))void(*)(framebuffer_t*, psf1_font_t*))header.e_entry);

            if (font) {
                kernel_entry(lfb, font);
            }
        }
    }

    return EFI_SUCCESS;
}
