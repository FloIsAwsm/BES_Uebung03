/**
 * @file sender.c
 *
 * Beispiel 3
 *
 * @author Florian Froestl <florian.froestl@technikum-wien.at>
 * @author David Boisits <david.boisits@technikum-wien.at>
 *
 * @date 2016/06/03
 *
 * @version 100
 *
 */
#include <stdio.h> // fgetc()
#include <string.h> // strerror()
#include <errno.h> // errno
#include <stdlib.h> // exit()
#include "SharedMemory.h"
#include "common.h"

/**
 * @brief [brief description]
 * @details [long description]
 *
 * @param argc [description]
 * @param argv [description]
 * @return [description]
 */
int main(int argc, char * const * argv)
{
	int size = getBufferSize(argc, argv);
	short c = 0;

	meminit(size);
	memattach();

	do {
		if ((c = fgetc(stdin)) == (short) EOF && ferror(stdin) != 0)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			memdetach();
			memrmv();
			return EXIT_FAILURE;
		}
		memwrite(c);
	} while (c != EOF);

	memdetach();

	return EXIT_SUCCESS;
}