#ifndef BUFFER_H
#define BUFFER_H

#include<stddef.h>
#include "server.h"

typedef struct {
    volatile char message[BUFFER_SIZE];
    volatile size_t dataEnd;
} SuspendedMessages;

/**
 * Retire et affiche les messages en attente sur stdout.
 *
 * @param buffer Messages en attente.
 **/
void FlushBuffer(SuspendedMessages* suspendedMessages);


#endif //BUFFER_H