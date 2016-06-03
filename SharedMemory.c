/**
 * @file SharedMemory.c
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
 * @todo error messages
 * 
 */
#include <stdio.h> // TODO
#include <stdlib.h> // TODO
#include <unistd.h> //getuid
#include <sys/ipc.h> // TODO
#include <sys/shm.h> // shmget()...
#include <sem182.h> // P(), V()...
#include <string.h> // strerror()
#include <errno.h> // errno
#include "SharedMemory.h"
#include "common.h"

/**
 * 
 */
static int sharedMemKey;
static int semKeyRead;
static int semKeyWrite;

/**
 * 
 */
static int sharedMemId = 0;
static int semWriteId = 0;
static int SemReadId = 0;

/**
 * 
 */
static int sharedMemSize;
static short * sharedMemAddr = NULL;

/**
 * 
 */
#define KEY_MULTIPLIER 1000
#define PERMISSIONS 0660

/**
 * @brief [brief description]
 * @details [long description]
 * @return [description]
 */
static key_t generateKey(void);

/* soll alles initialisieren (von einem der beiden Prozesse egal welcher) */
void meminit(int size)
{
	// save size
	sharedMemSize = size;

	// generate a keys
	sharedMemKey = generateKey();
	semKeyRead = generateKey();
	semKeyWrite = generateKey();

	// create shared memory
	sharedMemId = shmget(sharedMemKey, (size * sizeof(short)), PERMISSIONS | IPC_CREAT | IPC_EXCL);
	if(sharedMemId == EXIT_ERROR && errno == EEXIST)
	{
		// shared memory already exists
		sharedMemId = shmget(sharedMemKey, (size * sizeof(short)), 0);
		if(sharedMemId == EXIT_ERROR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	else if(sharedMemId == EXIT_ERROR)
	{
		fprintf(stderr, "%s: %s\n", appname, strerror(errno));
		exit(EXIT_FAILURE);
	}

	// create write semaphore (is initialized with size)
	semWriteId = seminit(semKeyWrite, PERMISSIONS, (size - 1)); // todo test with size
	if (semWriteId == EXIT_ERROR)
	{
		// todo does errno get set?
		// sem already exists
		semWriteId = semgrab(semKeyWrite);
		if (semWriteId == EXIT_ERROR)
		{
			// todo does errno get set?
			fprintf(stderr, "%s: Could not aquire semaphore.\n", appname);
			memrmv();
			exit(EXIT_FAILURE);
		}
	}

	// create read semaphore (initialize with 0)
	SemReadId = seminit(semKeyRead, PERMISSIONS, 0);
	if (SemReadId == EXIT_ERROR)
	{
		// sem already exists
		SemReadId = semgrab(semKeyRead);
		if (SemReadId == EXIT_ERROR)
		{
			fprintf(stderr, "%s: Could not aquire semaphore.\n", appname);
			memrmv();
			exit(EXIT_FAILURE);
		}
	}
}

/* soll das shared memory Segement in den Speicher einfügen. Muss gemacht werden um Zugriff zum Shared memory zu bekommen */
void memattach(void)
{
	sharedMemAddr = (short *) shmat(sharedMemId, NULL, 0);
	if((void *) sharedMemAddr == (void *) -1)
	{
		memrmv();
		fprintf(stderr, "%s: %s\n", appname, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void memdetach(void)
{
	if(shmdt(sharedMemAddr) != 0)
	{
		fprintf(stderr, "%s: %s\n", appname, strerror(errno));
		memrmv();
		exit(EXIT_FAILURE);
	}

}

void memrmv(void)
{
	struct shmid_ds buf;

	// remove semaphores first
	if(SemReadId != 0 || SemReadId != EXIT_ERROR)
	{
		if(semrm(SemReadId) == EXIT_ERROR)
		{
			fprintf(stderr, "%s: Could not remove semaphore\n", appname);
			// do not return here; try to clean up as much as possible	
		}
	}
	if(semWriteId != 0 || semWriteId != EXIT_ERROR)
	{
		if(semrm(semWriteId) == EXIT_ERROR)
		{
			fprintf(stderr, "%s: Could not remove semaphore\n", appname);
			// do not return here; try to clean up as much as possible
		}
	}

	// now mark shared memory for destruction
	if(sharedMemId != 0 && sharedMemId != EXIT_ERROR && shmctl(sharedMemId, IPC_RMID, &buf) == EXIT_ERROR)
	{
		fprintf(stderr, "%s: %s\n", appname, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

short memread(void)
{
	static int index = 0;
	short elem;

	errno = 0;
	while(P(SemReadId) == EXIT_ERROR)
	{
		if(errno != EINTR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			memdetach();
			memrmv();
			exit(EXIT_FAILURE);
		}
	}

	elem = *(sharedMemAddr + index++);

	errno = 0;
	while(V(semWriteId) == EXIT_ERROR)
	{
		if(errno != EINTR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			memdetach();
			memrmv();
			exit(EXIT_FAILURE);
		}
	}

	// make sure index stays in bounds
	if (index >= sharedMemSize) index = 0;

	return elem;
}

void memwrite(const short elem)
{
	static int index = 0;

	errno = 0;
	while(P(semWriteId) == EXIT_ERROR)
	{
		if(errno != EINTR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			memdetach();
			memrmv();
			exit(EXIT_FAILURE);
		}
	}

	*(sharedMemAddr + index++) = elem;

	errno = 0;
	while(V(SemReadId) == EXIT_ERROR)
	{
		if(errno != EINTR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			memdetach();
			memrmv();
			exit(EXIT_FAILURE);
		}
	}

	// make sure index stays in bounds
	if (index >= sharedMemSize) index = 0;
}

static key_t generateKey(void)
{
	static int cnt = 0;

	return ((KEY_MULTIPLIER * getuid()) + cnt++);
}
