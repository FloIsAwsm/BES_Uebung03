#include <stdio.h> // fputc()
#include <string.h> // strerr()
#include <errno.h> // errno
#include <stdlib.h> // exit()
#include "SharedMemory.h"
#include "common.h"

/**
 * @brief [brief description]
 * @details [long description]
 *
 * @param size [description]
 * @return [description]
 */
int reciever(int size);

int main(int argc, char * const * argv)
{
  return reciever(getBufferSize(argc, argv));
}

// method to initialize main loop
int reciever(int size)
{
  meminit(size);
  memattach();

  int c = 0;

  // TODO rework
  do {
    c = memread();
    if (c == (short) EOF)
      break;
    if (fputc((char) c, stdout) == EOF)
    {
      fprintf(stderr, "%s\n", strerror(errno));
      memdetach();
      memrmv();
      exit(EXIT_FAILURE);
    }
  } while (c != EOF);

  memdetach();

  memrmv();

  return EXIT_SUCCESS;
}