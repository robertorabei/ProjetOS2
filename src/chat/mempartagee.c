#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>

#include "mempartagee.h"

pthread_mutex_t shared_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

void flushBuffer(MessageBuffer *buffer) {
    fwrite(buffer->message, sizeof(char), buffer->endIndexExclusive, stdout);
    buffer->endIndexExclusive = 0;
    fflush(stdout);
}

void appendToBuffer(MessageBuffer *buffer, const char *message) {
    size_t msgLen = strlen(message);
    size_t spaceLeft = BUFFER_MAX - buffer->endIndexExclusive;

    pthread_mutex_lock(&shared_buffer_mutex);

    while (msgLen > 0) {
        if (msgLen <= spaceLeft) {
            strncpy(&buffer->message[buffer->endIndexExclusive], message, msgLen);
            buffer->endIndexExclusive += msgLen;
            msgLen = 0;
        } else {
            strncpy(&buffer->message[buffer->endIndexExclusive], message, spaceLeft);
            buffer->endIndexExclusive = 0;
            flushBuffer(buffer);
            message += spaceLeft;
            msgLen -= spaceLeft;
            spaceLeft = BUFFER_MAX;
        }
    }

    pthread_mutex_unlock(&shared_buffer_mutex);
}
MessageBuffer* createSharedMemory(void) {
   MessageBuffer* messageBuffer = mmap(NULL, sizeof(MessageBuffer), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   if (messageBuffer == NULL) {
      perror("mmap()");
      return NULL;
   }

   messageBuffer->endIndexExclusive = 0;

   return messageBuffer;
}

void freeSharedMemory(MessageBuffer* messageBuffer) {
   munmap(messageBuffer, sizeof(MessageBuffer));
}
