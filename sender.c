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
<<<<<<< HEAD
  if (ferror(stdin)) // TODO refactor
=======
  if (ferror(stdin))
>>>>>>> 26266e67af58fca944a0a45f43003f8640241cec
  {
    memdetach();
    return EXIT_FAILURE;
  }

	memdetach();
	
<<<<<<< HEAD
	return EXIT_SUCCESS;	
}
=======
	return 0;	
}

void print_usage(const char * appname)
{
	if(fprintf(stderr, "USAGE: %s -m <elements>\n", appname) < 0)
	{
		fprintf(stderr, "%s: %s\n", appname, strerror(errno));
		exit(EXIT_FAILURE);
	}
  exit(EXIT_FAILURE);
}
>>>>>>> 26266e67af58fca944a0a45f43003f8640241cec
