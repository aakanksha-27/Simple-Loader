Simple Loader
-------

Description:
The SimpleLoader is an ELF loader written in C that facilitates the loading and execution of 32-bit ELF executables. It operates as a shared library (`lib_simpleloader.so`), offering essential functionalities to map ELF segments into memory and execute the loaded program. This implementation avoids using external ELF manipulation libraries, focusing on low-level operations to achieve its objectives.


Implementation:
    a) 

 LD_LIBRARY_PATH=./bin/ ./bin/launch ./test/fib
