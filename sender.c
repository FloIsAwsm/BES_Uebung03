#include <stdio.h>
#include "SharedMemory.h"

int main(int argc, char ** argv)
{
	int size = 0;
	return method(size);
}

int method(int size)
{
	short c = 0;
	
	initalsize(size);
	attach();
	
	do{
		c = fgetc(stdin);
	} while(c != 'EOF');
	
	return 0;	
}