#ifndef object_h
#define object_h

#include "chunk.h"

typedef enum {
    OBJ_STRING,
    OBJ_FUNCTION
} ObjType;

typedef struct {
    Obj obj;
    Chunk *chunk;
    int arity;
    int upvalueCount;
    ObjString *name;
} ObjFunction;

typedef struct {
    Obj obj;
    int length;
    char *chars;
    uint32_t hash;
} ObjString;

typedef struct {
    ObjType type;
} Obj;

#define ALLOCATE_OBJ(type, objectType) \
  (type *)allocateObject(sizeof(type), objectType)

#define ALLOCATE(type, count) (type *)reallocate(NULL, 0, sizeof(type) * count)

ObjFunction *newFunction();
ObjString *copyString(const char *chars, int length);


#endif