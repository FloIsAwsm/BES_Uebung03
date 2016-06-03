
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
