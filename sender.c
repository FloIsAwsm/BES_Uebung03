#include <stdio.h>
#include "SharedMemory.h"

int main(int argc, char ** argv)
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

int method(int size)
{
	short c = 0;
	
	initalsize(size);
	attach();
	
	do{
		c = fgetc(stdin);
		memwrite(c);
	} while(c != EOF);

	detach();
	
	return 0;	
}

void print_usage(const char * appname)
{
	if(fprintf(stderr, "USAGE: %s -m <elements>\n", appname) < 0)
	{
		fprintf(stderr, "%s: %s\n", appname, stderr(errno));
		exit(EXIT_FAILURE);
	}
}
