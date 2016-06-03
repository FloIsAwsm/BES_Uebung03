#include <stdio.h>
#include <stdlib.h> // strtol()
#include <unistd.h> // getopt()
#include <string.h> // strerr()
#include <errno.h> // errno
#include <limits.h> // INT_MAX
#include "common.h"

static const char * argstr = "m:";
char * appname = NULL;

void print_usage(void)
{
	if (fprintf(stderr, "USAGE: %s -m <elements>\n", appname) < 0)
	{
		fprintf(stderr, "%s: %s\n", appname, strerror(errno));
		exit(EXIT_FAILURE);
	}
	exit(EXIT_FAILURE);
}

int getBufferSize(int argc, char * const * argv)
{
	appname = *argv;
	int opt;
	long int size = 0;
	while ((opt = getopt (argc, argv, argstr)) != -1)
	{
		if (opt == 'm')
		{
			char * pEnd = 0;
			if (optarg == NULL)
			{
				print_usage();
				exit(EXIT_FAILURE);
			}
			size = strtol(optarg, &pEnd, 10);
			if (*pEnd != '\0')
			{
				print_usage();
				exit(EXIT_FAILURE);
			}
			if (size > INT_MAX || size < 0 || errno == ERANGE) // optarg == '\0'
			{
				print_usage();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			print_usage();
			exit(EXIT_FAILURE);
		}
	}

	if (optind < argc)
	{
		print_usage();
		exit(EXIT_FAILURE);
	}

	if (size < 1)
	{
		print_usage();
		exit(EXIT_FAILURE);
	}

	return (int) size;
}