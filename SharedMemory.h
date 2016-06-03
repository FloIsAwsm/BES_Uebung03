/**
 * @file SharedMemory.h
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
#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

/**
 * return code indicating an error
 */
#define EXIT_ERROR (-1)

/**
 * @brief initializes the semaphores and shared memory segment
 * @details 
 * 
 * @param size size of the shared memory segment
 */
void meminit(int size);

/**
 * @brief removes semaphores and marks shared memory segment for destruction
 */
void memrmv(void);

/**
 * @brief attaches the process to the shared memory segment
 */
void memattach(void);

/**
 * @brief detaches the process to the shared memory segment
 */
void memdetach(void);

/**
 * @brief reads from shared memory segment
 * @return the value read from the shared memory segment
 */
short memread(void);

/**
 * @brief writes to shared memory segment
 * 
 * @param elem element to be written
 */
void memwrite(const short elem);

#endif /* SHAREDMEMORY_H */
