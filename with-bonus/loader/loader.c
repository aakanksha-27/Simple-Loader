#include "loader.h"

Elf32_Ehdr *ehdr;
Elf32_Phdr *phdr;
int fd;

void loader_cleanup() {
    free(ehdr);
    ehdr = NULL;
    free(phdr);
    phdr = NULL;
    close(fd);
}

void load_and_run_elf(char** exe) {
  // 1. Load entire binary content into the memory from the ELF file.
    fd = open(exe[1], O_RDONLY);
    if (fd < 0) {
        perror("Error: opening file");
        exit (1);
    }

    ehdr = (Elf32_Ehdr *)malloc(sizeof(Elf32_Ehdr));
    if (ehdr == NULL) {
        perror("Error: memory allocation for ELF header");
        close(fd);
        exit(1);
    }
    if (read(fd, ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
        perror("Error: reading ELF header");
        close(fd);
        exit (1);
    }

    lseek(fd, ehdr->e_phoff, SEEK_SET);
    phdr = (Elf32_Phdr *)malloc(ehdr->e_phnum * ehdr->e_phentsize);
    if (phdr == NULL) {
        perror("Error: memory allocation for program header");
        close(fd);
        exit(1);
    }
    if (read(fd, phdr, ehdr->e_phnum * ehdr->e_phentsize) != ehdr->e_phnum * ehdr->e_phentsize) {
        perror("Error: reading program header");
        close(fd);
        exit(1);
    }

  // 2. Iterate through the PHDR table and find the section of PT_LOAD
  //    type that contains the address of the entrypoint method in fib.c
  // 3. Allocate memory of the size "p_memsz" using mmap function
  //    and then copy the segment content

    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            void *virtual_mem = mmap((void*)phdr[i].p_vaddr, phdr[i].p_memsz, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
                if (virtual_mem == MAP_FAILED) {
                    perror("Error: memory allocation with mmap");
                    close(fd);
                    exit (1);
                }
                lseek(fd, phdr[i].p_offset, SEEK_SET);
                if (lseek(fd, phdr[i].p_offset, SEEK_SET) < 0 ) {
                     perror("Error: seeking to entrypoint");
                     munmap(virtual_mem, phdr->p_memsz);
                     close(fd);
                     exit (1);
                }

                if (read(fd, virtual_mem, phdr[i].p_filesz) != phdr[i].p_filesz) {
                    perror("Error: reading segment content");
                    munmap(virtual_mem, phdr[i].p_memsz);
                    close(fd);
                    exit (1);
                }

                if (phdr[i].p_memsz > phdr[i].p_filesz) {
                    memset(virtual_mem + phdr[i].p_filesz, 0, phdr[i].p_memsz - phdr[i].p_filesz);
                }

        }
    }
  // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step
    int (*_start)(void) = (int (*)(void))(ehdr->e_entry);
  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.
    int result = _start();
  // 6. Call the "_start" method and print the value returned from the "_start"
    printf("User _start return value = %d\n",result);
}
