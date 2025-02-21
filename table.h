#ifndef table_h
#define table_h

#include "object.h"

typedef struct {
    ObjString *key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry *entries;
} Table;

ObjString *tableFindString(Table *table, const char *chars, int length, uint32_t hash);

#endif