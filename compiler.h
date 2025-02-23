#ifndef compiler_h
#define compiler_h

#include "common.h"
#include "scanner.h"
#include "value.h"

typedef enum {
    TYPE_SCRIPT,
    TYPE_FUNCTION,
    TYPE_INITIALIZER
} FunctionType;

typedef struct {
    Token name;
    int depth;
    bool isCaptured;
} Local;

typedef struct {
    int index;
    bool isLocal;
} Upvalue;

typedef struct Compiler {
    struct Compiler *enclosing;
    ObjFunction *function;
    FunctionType type;

    Local locals[UINT8_COUNT];
    int localCount;
    Upvalue upvalues[UINT8_COUNT];
    int scopeDepth;
} Compiler;

typedef struct {
    Token previous;
    Token current;
    bool hadError;
    bool panicMode;
} Parser;

ObjFunction *compile(char *source);

#endif