#ifndef object_h
#define object_h

#include "chunk.h"

// Take an Obj and check what type of Object it is
#define OBJ_TYPE(value)             ((AS_OBJ(value)->type))

// Retrieve the ObjType* from a value's obj* , basically unpacking a runtime object value to get to a raw value and then retrieve the ObjType* value
#define AS_STRING(value)            ((ObjString*)AS_OBJ(value))
#define AS_CLOSURE(value)           ((ObjClosure*)AS_OBJ(value))
#define AS_FUNCTION(value)          ((ObjFunction*)AS_OBJ(value))
#define AS_CSTRING(value)           (((ObjString*)AS_OBJ(value))->chars)


typedef enum {
    OBJ_STRING,
    OBJ_FUNCTION,
    OBJ_CLOSURE,
    OBJ_UPVALUE,
} ObjType;

struct Obj {
    ObjType type;
};

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

#define ALLOCATE_OBJ(type, objectType) \
  (type *)allocateObject(sizeof(type), objectType)

#define ALLOCATE(type, count) (type *)reallocate(NULL, 0, sizeof(type) * count)

ObjFunction *newFunction();
ObjClosure *newClosure(ObjFunction *function);
ObjString *copyString(const char *chars, int length);
ObjString *takeString(char *chars, int length);
void printObject(Value value);

#endif