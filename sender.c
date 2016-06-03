#include <stdio.h> // fgetc()
#include <string.h> // strerror()
#include <errno.h> // errno
#include <stdlib.h> // exit()
#include "SharedMemory.h"
#include "common.h"

/* TODO sender -v prints twice */

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param size [description]
 * @return [description]
 */
static int sender(int size);

int main(int argc, char * const * argv)
{	
	return sender(getBufferSize(argc, argv));
}

int sender(int size)
{
	int c = 0;
	
	meminit(size);
	memattach();
	
	do{
		c = fgetc(stdin); // TODO error check
		memwrite(c);
	} while(c != EOF);
  if (ferror(stdin)) // TODO refactor
  {
    memdetach();
    return EXIT_FAILURE;
  }

	memdetach();
	
	return EXIT_SUCCESS;	
}
