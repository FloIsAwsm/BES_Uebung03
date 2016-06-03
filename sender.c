#include <stdio.h>
#include <stdlib.h> // strtol()
#include <unistd.h> // getopt()
#include <string.h> // strerror()
#include <errno.h> // errno
#include <limits.h> // INT_MAX
#include "SharedMemory.h"

/* TODO sender -v prints twice */

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
    	if(size > INT_MAX || size < 0 || errno == ERANGE) // optarg == '\0'
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

int method(int size)
{
	int c = 0;
	
	meminit(size);
	memattach();
	
	do{
		c = fgetc(stdin);
		memwrite(c);
	} while(c != EOF);
  if (ferror(stdin))
  {
    memdetach();
    return EXIT_FAILURE;
  }

	memdetach();
	
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
