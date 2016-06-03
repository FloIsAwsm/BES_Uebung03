<<<<<<< HEAD

#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

/**
 * 
 */
#define EXIT_ERROR (-1)

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param size [description]
 */
void meminit(int size);

/**
 * @brief [brief description]
 * @details [long description]
 */
void memrmv(void);

/**
 * @brief [brief description]
 * @details [long description]
 */
void memattach(void);

/**
 * @brief [brief description]
 * @details [long description]
 */
void memdetach(void);

/**
 * @brief [brief description]
 * @details [long description]
 * @return [description]
 */
short memread(void);

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param elem [description]
 */
void memwrite(const short elem);

#endif /* SHAREDMEMORY_H */
=======

#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#define EXIT_ERROR (-1)

void meminit(int size);

void memrmv(void);

void memattach(void);

void memdetach(void);

int memread(void);

void memwrite(const int elem);

void LogByte(char * file, int c);

void WriteFile(int c);

#endif /* SHAREDMEMORY_H */
>>>>>>> 26266e67af58fca944a0a45f43003f8640241cec
