#ifndef MEM_PARTAGEE_H__
#define MEM_PARTAGEE_H__

#include <stddef.h>

#define BUFFER_MAX ((size_t)4096)

typedef struct {
    char message[BUFFER_MAX];
    size_t endIndexExclusive;
} MessageBuffer;

void FlushBuffer(MessageBuffer* buffer);

void appendToBuffer(MessageBuffer *buffer, const char *message);

MessageBuffer* createSharedMemory(void);

void freeSharedMemory(MessageBuffer* buffer);

#endif
