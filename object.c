#include "common.h"
#include "object.h"

static Obj *allocateObject(size_t size, ObjType type) {
    Obj *object = (Obj*) reallocate(NULL, 0, size);
    object->type = type;
    return object;
}

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

ObjFunction *newFunction() {
    ObjFunction *function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    initChunk(&function->chunk);
    function->name = NULL;
    function->upvalueCount = 0;
    return function;
}