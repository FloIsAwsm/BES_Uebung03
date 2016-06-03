
#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#define EXIT_ERROR (-1)

void meminit(int size);

void memrmv(void);

void memattach(void);

void memdetach(void);

<<<<<<< HEAD
int memread(void);

void memwrite(const int elem);

void LogByte(char * file, int c);

void WriteFile(int c);
=======
short memread(void);

void memwrite(const short elem);
>>>>>>> 2985f36c5a3b591503a9a8e8f93faa922bf40823

#endif /* SHAREDMEMORY_H */
