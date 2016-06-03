<<<<<<< HEAD
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
=======
#include <stdio.h>
#include <stdlib.h> // strtol()
#include <unistd.h> // getopt()
#include <string.h> // strerr()
#include <errno.h> // errno
#include <limits.h> // INT_MAX
#include "SharedMemory.h"

void print_usage(const char * appname);
int method(int size);

static const char * argstr = "m:";

int main(int argc, char * const * argv)
{
	// use getopt()
	const char * appname = *argv;
	int opt;
	long int size = 0;
  while ((opt = getopt (argc, argv, argstr)) != -1)
  {
    if(opt == 'm')
    {
    	char * pEnd = 0;
    	if(optarg == NULL)
    	{
    		print_usage(appname);
    		exit(EXIT_FAILURE);
    	}
		size = strtol(optarg, &pEnd, 10);
    	if(*pEnd != '\0')
    	{
    		print_usage(appname);
    		exit(EXIT_FAILURE);
    	}
    	if(size > INT_MAX || size < 0 || errno == ERANGE)
    	{
    		print_usage(appname);
    		exit(EXIT_FAILURE);
    	}
    }
    else
    {
    	print_usage(appname);
    	exit(EXIT_FAILURE);
    }
  }

  if (optind < argc)
  {
  	print_usage(appname);
  	exit(EXIT_FAILURE);
  }

  if(size < 1)
  {
  	print_usage(appname);
  	return EXIT_FAILURE;
  }
	
	return method(size);
}

// method to initialize main loop
int method(int size)
{
	meminit(size);
	memattach();
	
	int c = 0;
	
	do{
		c = memread();
    if (c == EOF)
      break;
		if(fputc((char) c, stdout) == EOF)
		{
			fprintf(stderr, "%s\n", strerror(errno));
			memdetach();
			memrmv();
			exit(EXIT_FAILURE);
		}
	}while(c != EOF);
	
	memdetach();
	
	memrmv();
	
	return 0;
}

/* TODO: needs to be in a shared file... */
void print_usage(const char * appname)
{
	if(fprintf(stderr, "USAGE: %s -m <elements>\n", appname) < 0)
	{
		fprintf(stderr, "%s: %s\n", appname, strerror(errno));
		exit(EXIT_FAILURE);
	}
}
>>>>>>> 26266e67af58fca944a0a45f43003f8640241cec
