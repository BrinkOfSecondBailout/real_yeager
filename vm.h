#ifndef vm_h
#define vm_h

#include "common.h"
#include "compiler.h"
#include "table.h"
#include "value.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

typedef struct {
    Value stack[STACK_MAX];
    Value *stackTop;
    Table strings;
    Table globals;

} VM;

extern VM vm;

InterpretResult interpret(char *source);

#endif