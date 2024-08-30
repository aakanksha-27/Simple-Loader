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

/*
 * Load and run the ELF executable file
 */
void load_and_run_elf(char** exe) {
  // 1. Load entire binary content into the memory from the ELF file.
    fd = open(exe[1], O_RDONLY);
    if (fd < 0) {
        printf("Error in opening file");
        exit (1);
    }
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size < 0) {
        printf("Error seeking to end of file");
        close(fd);
        exit (1);
    }
    
    lseek(fd, 0, SEEK_SET);
    char *buffer = (char *)malloc(file_size);

    if (buffer == NULL) {
        printf("Error during Memory allocation");
        close(fd);
        exit (1) ;
    }
    ssize_t file_read = read(fd, buffer, file_size);
    close(fd);
  
    
  // 2. Iterate through the PHDR table and find the section of PT_LOAD
  //    type that contains the address of the entrypoint method in fib.c
  // 3. Allocate memory of the size "p_memsz" using mmap function
  //    and then copy the segment content
  // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step
    int (*_start)(void) = (int (*)(void))(ehdr->e_entry);
  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.
    int result = _start();
  // 6. Call the "_start" method and print the value returned from the "_start"
    printf("User _start return value = %d\n",result);
}
int main(int argc, char** argv)
{
    if(argc != 2) {
        printf("Usage: %s <ELF Executable> \n",argv[0]);
        exit(1);
  }
  // 1. carry out necessary checks on the input ELF file
  // 2. passing it to the loader for carrying out the loading/execution
    load_and_run_elf(argv);
  // 3. invoke the cleanup routine inside the loader
    loader_cleanup();
    return 0;
}
