#ifndef object_h
#define object_h

typedef enum {
    OBJ_STRING,
} ObjType;

typedef struct {
    ObjType type;
} Obj;

#endif