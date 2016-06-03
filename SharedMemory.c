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
 * generated key for shared memory segment
 */
static int sharedMemKey;
/**
 * generated key for read semaphore
 */
static int semKeyRead;
/**
 * generated key for write semaphore
 */
static int semKeyWrite;

/**
 * id of shared memory segment
 */
static int sharedMemId = 0;
/**
 * id of write semaphore
 */
static int semWriteId = 0;
/**
 * id of read semaphore
 */
static int SemReadId = 0;

/**
 * size of the shared memory segment (in elements not bytes)
 */
static int sharedMemSize;
/**
 * pointer to the starting address of the shared memory segment
 */
static short * sharedMemAddr = NULL;

/**
 * define for the key generation
 */
#define KEY_MULTIPLIER 1000
/**
 * define for permissions
 */
#define PERMISSIONS 0660

/**
 * @brief generates a new key
 * @details generates a key for the semaphores and the shared memory segment using the user id and key multiplier.
 * with every call a new key is generated
 * @return generated key
 */
static key_t generateKey(void);

void meminit(int size)
{
	// save size
	sharedMemSize = size;

	// generate a keys
	sharedMemKey = generateKey();
	semKeyRead = generateKey();
	semKeyWrite = generateKey();

	// create shared memory
	errno = 0;
	sharedMemId = shmget(sharedMemKey, (size * sizeof(short)), PERMISSIONS | IPC_CREAT | IPC_EXCL);
	if(sharedMemId == EXIT_ERROR)
	{
		if(errno != EEXIST)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			exit(EXIT_FAILURE);
		}
		// shared memory already exists
		errno = 0;
		sharedMemId = shmget(sharedMemKey, (size * sizeof(short)), 0);
		if(sharedMemId == EXIT_ERROR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	// create write semaphore (is initialized with size)
	errno = 0;
	semWriteId = seminit(semKeyWrite, PERMISSIONS, size);
	if (semWriteId == EXIT_ERROR)
	{
		if(errno != EEXIST)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			memrmv();
			exit(EXIT_FAILURE);
		}
		errno = 0;
		// sem already exists
		semWriteId = semgrab(semKeyWrite);
		if (semWriteId == EXIT_ERROR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			memrmv();
			exit(EXIT_FAILURE);
		}
	}

	// create read semaphore (initialize with 0)
	errno = 0;
	SemReadId = seminit(semKeyRead, PERMISSIONS, 0);
	if (SemReadId == EXIT_ERROR)
	{
		if(errno != EEXIST)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			memrmv();
			exit(EXIT_FAILURE);
		}
		errno = 0;
		// sem already exists
		SemReadId = semgrab(semKeyRead);
		if (SemReadId == EXIT_ERROR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			memrmv();
			exit(EXIT_FAILURE);
		}
	}
}

void memattach(void)
{
	errno = 0;
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
	errno = 0;
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
		errno = 0;
		if(semrm(SemReadId) == EXIT_ERROR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			// do not return here; try to clean up as much as possible	
		}
	}
	if(semWriteId != 0 || semWriteId != EXIT_ERROR)
	{
		errno = 0;
		if(semrm(semWriteId) == EXIT_ERROR)
		{
			fprintf(stderr, "%s: %s\n", appname, strerror(errno));
			// do not return here; try to clean up as much as possible
		}
	}

	// now mark shared memory for destruction
	errno = 0;
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
