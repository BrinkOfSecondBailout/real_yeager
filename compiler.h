#ifndef compiler_h
#define compiler_h

#include "common.h"
#include "value.h"

typedef struct {
    uint8_t code;
} Chunk;

typedef struct {
    Chunk chunk;
    Value constants[];
} Compiler;

#endif