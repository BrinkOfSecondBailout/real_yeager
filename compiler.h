#ifndef compiler_h
#define compiler_h

#include "common.h"
#include "value.h"

typedef enum {
    TYPE_SCRIPT,
    TYPE_FUNCTION
} FunctionType;

typedef struct {
    uint8_t code;
} Chunk;

typedef struct {
    Chunk chunk;
    Value constants[];
} Compiler;

typedef struct {
    Token previous;
    Token current;
} Parser;

void compile(char *source);

#endif