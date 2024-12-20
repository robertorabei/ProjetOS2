#include <stdio.h>

#include "buffer.h"

// Fonction de vidage

void FlushBuffer(SuspendedMessages* suspendedMessages) {
   fwrite((char*)suspendedMessages->message, sizeof(char), suspendedMessages->dataEnd, stdout);
   suspendedMessages->dataEnd= 0;
}