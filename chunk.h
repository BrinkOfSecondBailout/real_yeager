#ifndef chunk_h
#define chunk_h

#include "common.h"
#include "value.h"

typedef struct {
    int count;
    int capacity;
    int *lines;
    uint8_t *code;
    ValueArray constants;
} Chunk;

#endif