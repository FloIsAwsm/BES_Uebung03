
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
