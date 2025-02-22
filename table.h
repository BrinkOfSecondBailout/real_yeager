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
void initTable(Table *table);
void freeTable(Table *table);
bool tableDelete(Table *table, ObjString *key);
bool tableGet(Table *table, ObjString *key, Value *value);
void tableAddAll(Table *from, Table *to);
bool tableSet(Table *table, ObjString *key, Value value);
ObjString *tableFindString(Table *table, const char *chars, int length, uint32_t hash);

#endif