/**
 * @file common.c
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
#include <stdio.h> // fprintf()
#include <stdlib.h> // strtol()
#include <unistd.h> // getopt()
#include <string.h> // strerr()
#include <errno.h> // errno
#include <limits.h> // INT_MAX
#include "common.h"

/**
 * c-string containing all argument options for getopt()
 */
static const char * argstr = "m:";

char * appname = NULL;

void print_usage(void)
{
	errno = 0;
	if (fprintf(stderr, "USAGE: %s -m <size>\n", appname) < 0)
	{
		fprintf(stderr, "%s: %s\n", appname, strerror(errno));
	}
	exit(EXIT_FAILURE);
}

int getBufferSize(int argc, char * const * argv)
{
	appname = *argv;
	int opt;
	long int size = 0;
	if ((opt = getopt (argc, argv, argstr)) != -1)
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
			if(errno != 0)
			{
				fprintf(stderr, "%s: %s\n", appname, strerror(errno));
				print_usage();
			}
			if (*pEnd != '\0')
			{
				fprintf(stderr, "%s: argument is not a valid number\n", appname);
				print_usage();
			}
			if (size > INT_MAX || size <= 0)
			{
				fprintf(stderr, "%s: size must be between %d and %d\n", appname, 1, INT_MAX);
				print_usage();
			}
		}
		else
		{
			// getopt() prints an error message ;)
			print_usage();
		}
	}
	else
	{
		fprintf(stderr, "%s: missing mandatory argument.\n", appname);
		print_usage();
	}

	if (optind < argc)
	{
		fprintf(stderr, "%s: containing one or more invalid arguments.\n", appname);
		print_usage();
	}

	return (int) size;
}