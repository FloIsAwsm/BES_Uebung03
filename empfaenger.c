#include <stdio.h>
#include "SharedMemory.h"

static const char * argstr = "m:";

int main(int argc, const char * const * argv)
{
	// use getopt()
	const char * appname = *argv;
	int opt;
	int size = 0;
  while ((opt = getopt (argc, argv, argstr)) != -1)
  {
    if(opt == 'm')
    {
    	char * pEnd = 0;
		size = strtol(params[i], &pEnd, 10);
    	if(*pEnd != '\0')
    	{
    		print_usage(appname);
    	}
    }
    else
    {
    	print_usage(appname);
    }
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
	initialize(size);
	attach();
	
	short c = 0;
	
	do{
		c = memread();
		fputc((char) c, stdout);
	}while(c != EOF);
	
	detach();
	
	remove();
	
	return 0;
}

/* TODO: needs to be in a shared file... */
void print_usage(const char * appname)
{
	if(fprintf(stderr, "USAGE: %s -m <elements>\n", appname) < 0)
	{
		fprintf(stderr, "%s: %s\n", appname, stderr(errno));
		exit(EXIT_FAILURE);
	}
}
