#include <stdio.h>
#include "SharedMemory.h"

int main(int argc, char ** argv)
{
	// use getopt()
	int size = 0;
	
	return method(size);
}

// method to initialize main loop
int method(int size)
{
	initialize(size);
	attach();
	
	short c = 0;
	
	do{
		c = memread();
		printf("%c", c);
	}while((char) c != 'EOF');
	
	detach();
	
	remove();
	
	return 0;
}

int print_usage()
{
	// error checking ...
	printf("%s\n" , "USAGE: ");
}
