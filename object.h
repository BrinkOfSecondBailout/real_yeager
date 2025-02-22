#ifndef object_h
#define object_h

#include "chunk.h"

typedef enum {
    OBJ_STRING,
    OBJ_FUNCTION,
    OBJ_CLOSURE,
    OBJ_UPVALUE,
} ObjType;

typedef struct {
    Obj obj;
    Chunk chunk;
    int arity;
    int upvalueCount;
    ObjString *name;
} ObjFunction;

typedef struct {
    Obj obj;
    Value *location;
    Value closed;
    struct ObjUpvalue *next;
} ObjUpvalue;

typedef struct {
    Obj obj;
    ObjFunction *function;
    ObjUpvalue **upvalues;
    int upvalueCount;
} ObjClosure;

struct ObjString {
    Obj obj;
    int length;
    char *chars;
    uint32_t hash;
};

typedef struct {
    ObjType type;
} Obj;

#define ALLOCATE_OBJ(type, objectType) \
  (type *)allocateObject(sizeof(type), objectType)

#define ALLOCATE(type, count) (type *)reallocate(NULL, 0, sizeof(type) * count)

ObjFunction *newFunction();
ObjClosure *newClosure(ObjFunction *function);
ObjString *copyString(const char *chars, int length);


#endif