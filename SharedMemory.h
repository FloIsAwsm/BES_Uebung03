
#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#define EXIT_ERROR (-1)

void initialize();

void remove();

void attach();

void detach();

int memread();

void memwrite();

#endif /* SHAREDMEMORY_H */
