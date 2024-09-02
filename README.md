Simple Loader
-------

Description:

The SimpleLoader is an ELF loader written in C that facilitates the loading and execution of 32-bit ELF executables. It operates as a shared library (`lib_simpleloader.so`), offering essential functionalities to map ELF segments into memory and execute the loaded program. This implementation avoids using external ELF manipulation libraries, focusing on low-level operations to achieve its objectives.


Implementation:

a) We initialize a pointer ehdr (of structure Elf32_Ehdr) representing the ELF header, a pointer phdr (of structure Elf32_Phdr) representing the program header, and a file descriptor.

b) loader_cleanup function frees up the memory allocated. It helps prevent memory leaks and ensures the file is closed properly.

c) load_and_run function loads and executes the ELF file. Opening file, memory allocation, reading ELF header, seeking to phoff and reading program header is done and all intermediate errors are handled.

d) Iterate through the PHDR table and find the PT_LOAD section that contains the address of the entrypoint method in fib.c. Memory is allocated using mmap function and segment content is copied.

e) If the memory size (p_memsz) is larger than the file size (p_filesz), the remaining memory is set to 0.

f) The entrypoint is executed as a function pointer. The entrypoint function is called, and the returned result is printed.


To run the project: 

LD_LIBRARY_PATH=./bin/ ./bin/launch ./test/fib


Contributions:

Aakanksha (2023004) - loader_cleanup, finding the PT_LOAD section, memory allocation of and copying the segment content, setting remaining memory to 0, error handling, make files
Palak Yadav (2023363) - loading binary content into the memory, navigating to the entrypoint address, typecasting the address, error handling, make files
