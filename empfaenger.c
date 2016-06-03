/**
 * @file empfaenger.c
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
 * @todo format
 *
 */
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

int reciever(int size)
{
  meminit(size);
  memattach();

  short c = memread();

  while (c != (short) EOF)
  {
    if (fputc((char) c, stdout) == EOF)
    {
      fprintf(stderr, "%s: %s\n", appname, strerror(errno));
      memdetach();
      memrmv();
      exit(EXIT_FAILURE);
    }
    c = memread();
  }

  memdetach();

  memrmv();

  return EXIT_SUCCESS;
}