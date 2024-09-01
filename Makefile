all:
	gcc  -m32 -no-pie -nostdlib -o fib fib.c
	gcc -m32 -o loader loader.c
	gcc -m32 -c launcher.c

clean:
	-@rm -f fib loader
	-@rm -f launcher.o launcher
