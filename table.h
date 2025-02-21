#ifndef table_h
#define table_h

#include "object.h"

typedef struct {
    ObjString *key;
    Value value;
} Entry;

typedef struct {
    Entry *entries;
} Table;

#endif