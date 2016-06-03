
#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#define EXIT_ERROR (-1)

void meminit(int size);

void memrmv(void);

void memattach(void);

void memdetach(void);

short memread(void);

void memwrite(const short elem);

#endif /* SHAREDMEMORY_H */
