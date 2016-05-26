#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <msem182.h>
#include "SharedMemory.h"

static int sharedMemKey;
static int semKeyRead;
static int semKeyWrite;

static int sharedMemId;
static int semWriteId;
static int SemReadId;

static int sharedMemSize;
static const void * sharedMemAddr = NULL;

#define KEY_MULTIPLIER 1000
#define SEM_PERMISSIONS 0660

static key_t generateKey(void);

/* soll alles initialisieren (von einem der beiden Prozesse egal welcher) */
void initialize(int size)
{
	// save size
	sharedMemSize = size;

	// generate a keys
	sharedMemKey = generateKey();
	semKeyRead = generateKey();
	semKeyWrite = generateKey();

	// create shared memory
	sharedMemId = shmget(sharedMemKey, (size * sizeof(char)), IPC_CREAT | IPC_EXCL);
	if(sharedMemId == EXIT_ERROR && errno == EEXISTS)
	{
		// shared memory already exists
		sharedMemId = shmget(sharedMemKey, (size * sizeof(char)), 0);
		if(sharedMemId == EXIT_ERROR)
		{
			// now we have a problem
		}
	}

	// create write semaphore (is initialized with size)
	semWriteId = seminit(semKeyWrite, SEM_PERMISSIONS, size);
	if (semWriteId == EXIT_ERROR)
	{
		// sem already exists
		semWriteId = semgrab(semKeyWrite);
		if (semWriteId == EXIT_ERROR)
		{
			// now we have a problem
		}
	}

	// create read semaphore (initialize with 0)
	SemReadId = seminit(semKeyRead, SEM_PERMISSIONS, 0);
	if (SemReadId == EXIT_ERROR)
	{
		// sem already exists
		SemReadId = semgrab(semKeyRead);
		if (SemReadId == EXIT_ERROR)
		{
			// now we have a problem
		}
	}
}

/* soll das shared memory Segement in den Speicher einfügen. Muss gemacht werden um Zugriff zum Shared memory zu bekommen */
void attach()
{
	sharedMemAddr = shmat(sharedMemId, NULL, 0);
}

/* TODO: what if a Process crashes in case of an error; EDIT: shared mem gets detached by an _exit(2) call */
/* soll das shared memory segment vom Speicher lösen. Das MUSS jeder Prozess machen sonst kann das Segment nicht gelöscht werden */
void detach()
{
	shmdt(sharedMemAddr);
}

/* soll alles entfernen (im Fehlerfall egal welcher Prozess) nur vom empfaenger */
void remove()
{
	struct shmid_ds buf;
	// remove semaphores first
	semrm(SemReadId);
	semrm(semWriteId);

	// now mark shared memory for destruction
	if(shmctl(sharedMemId, IPC_RMID, buf) == EXIT_ERROR)
	{
		// errno is set...
		// but what do we do? maybe buf will help
	}
}

/* liest ein Element aus dem shared memory bereich */
int memread()
{
	static int index = 0;
	int elem;

	while(P(semWriteId) == EXIT_ERROR && errno == EINTR);
	if(errno != 0)
	{
		// handle error
	}

	// now read
	elem = *(sharedMemAddr + index++);


	while(V(SemReadId) == EXIT_ERROR && errno == EINTR);
	if(errno != 0)
	{
		// handle error
	}

	// make sure index stays in bounds
	if (index >= sharedMemSize) index = 0;

}

/* schreibt ein shared memory element */
void memwrite(const int elem)
{
	static int index = 0;

	while(P(semWriteId) == EXIT_ERROR && errno == EINTR);
	if(errno != 0)
	{
		// handle error
	}

	// now write
	*(sharedMemAddr + index++) = elem;


	while(V(SemReadId) == EXIT_ERROR && errno == EINTR);
	if(errno != 0)
	{
		// handle error
	}

	// make sure index stays in bounds
	if (index >= sharedMemSize) index = 0;
}

static key_t generateKey(void)
{
	static int cnt = 1;

	return ((KEY_MULTIPLIER * getuid()) + cnt);
}