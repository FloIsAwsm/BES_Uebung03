#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //getuid
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sem182.h>
#include <string.h>
#include <errno.h> // errno
#include "SharedMemory.h"

static int sharedMemKey;
static int semKeyRead;
static int semKeyWrite;

static int sharedMemId = 0;
static int semWriteId = 0;
static int SemReadId = 0;

static int sharedMemSize;
static short * sharedMemAddr = NULL;

#define KEY_MULTIPLIER 1000
#define PERMISSIONS 0666

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
			// now we have a problem
			memrmv();
			fprintf(stderr, "sharedMemId\n");
			exit(EXIT_FAILURE);
		}
	}
	else if(sharedMemId == EXIT_ERROR)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// create write semaphore (is initialized with size)
	semWriteId = seminit(semKeyWrite, PERMISSIONS, (size - 1));
	if (semWriteId == EXIT_ERROR)
	{
		// sem already exists
		semWriteId = semgrab(semKeyWrite);
		if (semWriteId == EXIT_ERROR)
		{
			// now we have a problem
			memrmv();
			fprintf(stderr, "semWriteId\n");
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
			memrmv();
			// now we have a problem
			fprintf(stderr, "SemReadId\n");
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
		fprintf(stderr, "attatch(): %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

/* TODO: what if a Process crashes in case of an error; EDIT: shared mem gets detached by an _exit(2) call */
/* soll das shared memory segment vom Speicher lösen. Das MUSS jeder Prozess machen sonst kann das Segment nicht gelöscht werden */
void memdetach(void)
{
	if(shmdt(sharedMemAddr) != 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		memrmv();
		exit(EXIT_FAILURE);
	}

}

/* soll alles entfernen (im Fehlerfall egal welcher Prozess) nur vom empfaenger */
void memrmv(void)
{
	struct shmid_ds buf;

	//printf("SemReadId: %d semWriteId: %d sharedMemId: %d\n",SemReadId, semWriteId,sharedMemId);
	// remove semaphores first
	if(SemReadId != 0 || SemReadId != EXIT_ERROR)
	{
		semrm(SemReadId);
	}
	if(semWriteId != 0 || semWriteId != EXIT_ERROR)
	{
		semrm(semWriteId);
	}

	// now mark shared memory for destruction
	if(sharedMemId != 0 && sharedMemId != EXIT_ERROR && shmctl(sharedMemId, IPC_RMID, &buf) == EXIT_ERROR)
	{
		// errno is set...
		// but what do we do? maybe buf will help
		exit(EXIT_FAILURE);

	}
}

/* liest ein Element aus dem shared memory bereich */
short memread(void)
{
	static int index = 0;
	int elem;

	errno = 0;
	while(P(SemReadId) == EXIT_ERROR && errno == EINTR);
	if(errno != 0)
	{
		// handle error
		memrmv();
		fprintf(stderr, "P() read\n");
		exit(EXIT_FAILURE);

	}

	//printf("read\n");
	// now read
	elem = *(sharedMemAddr + index++);


	while(V(semWriteId) == EXIT_ERROR && errno == EINTR);
	if(errno != 0)
	{
		// handle error
		memrmv();
		fprintf(stderr, "V() read\n");
		exit(EXIT_FAILURE);

	}

	// make sure index stays in bounds
	if (index >= sharedMemSize) index = 0;

	return elem;

}

/* schreibt ein shared memory element */
void memwrite(const short elem)
{
	static int index = 0;

	errno = 0;
	while(P(semWriteId) == EXIT_ERROR && errno == EINTR);
	if(errno != 0)
	{
		// handle error
		memrmv();
		fprintf(stderr, "P() write\n");
		exit(EXIT_FAILURE);

	}

	// now write
	//printf("write\n");
	*(sharedMemAddr + index++) = elem;


	while(V(SemReadId) == EXIT_ERROR && errno == EINTR);
	if(errno != 0)
	{
		// handle error
		memrmv();
		fprintf(stderr, "V() write\n");
		exit(EXIT_FAILURE);

	}

	// make sure index stays in bounds
	if (index >= sharedMemSize) index = 0;
}

static key_t generateKey(void)
{
	static int cnt = 0;

	return ((KEY_MULTIPLIER * getuid()) + cnt++);
}
