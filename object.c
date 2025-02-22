#include "common.h"
#include "object.h"
#include "table.h"
#include "memory.h"
#include "vm.h"

static Obj *allocateObject(size_t size, ObjType type) {
    Obj *object = (Obj*) reallocate(NULL, 0, size);
    object->type = type;
    return object;
}

ObjFunction *newFunction() {
    ObjFunction *function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    initChunk(&function->chunk);
    function->name = NULL;
    function->upvalueCount = 0;
    return function;
}

ObjClosure *newClosure(ObjFunction *function) {
    ObjUpvalue **upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount);
    for (int i = 0; i < function->upvalueCount; i++) {
        upvalues[i] = NULL;
    }

    ObjClosure *closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
    closure->function = function;
    closure->upvalueCount = function->upvalueCount;
    closure->upvalues = upvalues;
    return closure;
}

static uint32_t hashString(const char *key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

static ObjString *allocateString(const char *chars, int length, uint32_t hash) {
    ObjString *newStringObj = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    newStringObj->length = length;
    newStringObj->hash = hash;
    newStringObj->chars = chars;

    push(OBJ_VAL(newStringObj));
    tableSet(&vm.strings, newStringObj, NIL_VAL);
    pop();
    return newStringObj;
}


ObjString *copyString(const char *chars, int length) {
    uint32_t hash = hashString(chars, length);

    ObjString *interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) return interned;

    char *heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';

    return allocateString(heapChars, length, hash);
}