#ifndef value_h
#define value_h

#include "object.h"

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
        Obj *obj;
    } as;
} Value;

typedef struct {
    int count;
    int capacity;
    Value *values;
} ValueArray;

// Converts runtime value into raw value
#define AS_OBJ(value)           ((value).as.obj)
#define AS_NUMBER(value)        ((value).as.number)
#define AS_BOOL(value)          ((value).as.boolean)

// Converts raw value into runtime value
#define NUMBER_VAL(value)       ((Value){VAL_NUMBER, {.number = value}})
#define BOOL_VAL(value)         ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL                 ((Value){VAL_NIL, {.number = 0}})
#define OBJ_VAL(object)         ((Value){VAL_OBJ, {.obj = (Obj *)object}})

// Check if runtime value is of a particular type
#define IS_BOOL(value)          ((value).type == VAL_BOOL)
#define IS_OBJ(value)           ((value).type == VAL_OBJ)
#define IS_NUMBER(value)        ((value).type == VAL_NUMBER)
#define IS_NIL(value)           ((value).type == VAL_NIL)

void initValueArray(ValueArray *array);
void writeValueArray(ValueArray *array, Value value);
bool valuesEqual(Value a, Value b);
void printValue(Value value);

#endif