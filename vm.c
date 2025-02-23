#include "object.h"
#include "vm.h"

static void resetStack() {
    vm.stackTop = vm.stack;
    vm.frameCount = 0;
    vm.openUpvalues = 0;
}

void initVm() {
    resetStack();
    initTable(&vm.globals);
    initTable(&vm.strings);
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

static Value peek(int depth) {
    return vm.stackTop[-1 - depth];
}

static void runtimeError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    // print out stack trace starting from top to bottom
    for (int i = vm.frameCount - 1; i >= 0; i--) {
        CallFrame *frame = &vm.frames[i];
        ObjFunction *function = frame->closure->function;
        size_t instruction = frame->ip - function->chunk.code - 1;
        fprintf(stderr, "[line %d] in ", function->chunk.lines[instruction]);
        if (function->name == NULL) {
            fprintf(stderr, "script\n");
        } else {
            fprintf(stderr, "%s()\n", function->name->chars);
        }
    }
    resetStack();
}

static bool call(ObjClosure *closure, int argCount) {
    if (argCount != closure->function->arity) {
        runtimeError("Expected %d arguments, got %d.", closure->function->arity, argCount);
        return false;
    }

    if (vm.frameCount == FRAMES_MAX) {
        runtimeError("Stack overflow.");
        return false;
    }

    CallFrame *frame = &vm.frames[vm.frameCount++];
    frame->closure = closure;
    frame->ip = closure->function->chunk.code;
    frame->slots = vm.stackTop - argCount - 1;
    return true;
}

static void concatenate() {
    ObjString *b = AS_STRING(peek(0));
    ObjString *a = AS_STRING(peek(1));

    int length = a->length + b->length;
    char *chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString *result = takeString(chars, length);
    pop();
    pop();
    push(OBJ_VAL(result));
}

static bool isFalsey(Value value) {
    // Only NIL and false are false
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void closeUpvalues(Value *last) {
    
}

static InterpretResult run() {
    CallFrame *frame = &vm.frames[vm.frameCount - 1];

    #define READ_BYTE()                 (*frame->ip++)
    #define READ_CONSTANT()             (frame->closure->function->chunk.constants.values[READ_BYTE()])
    #define READ_STRING()               AS_STRING(READ_CONSTANT())
    #define READ_SHORT()                (frame->ip += 2, (uint16_t)((frame->ip[-2] << 8 | frame->ip[-1])))
    #define BINARY_OP(returnedValueType, op) \
        do { \
            if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {   \
                runtimeError("Operands must be numbers.");      \
                return INTERPRET_RUNTIME_ERROR;                 \
            }                                                   \
            double b = AS_NUMBER(pop());                        \
            double a = AS_NUMBER(pop());                        \
            push(returnedValueType(a op b));                    \
        } while (false)
    
    for (;;) {
        uint8_t instruction;
        switch(instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_NIL: {
                push(NIL_VAL);
                break;
            }
            case OP_TRUE: {
                push(BOOL_VAL(true));
                break;
            }
            case OP_FALSE: {
                push(BOOL_VAL(false));
                break;
            }
            case OP_POP: {
                pop();
                break;
            }
            case OP_GET_LOCAL: {
            }
            case OP_SET_LOCAL: {
            }
            case OP_GET_GLOBAL: {
            }
            case OP_DEFINE_GLOBAL: {
            }
            case OP_SET_GLOBAL: {
            }
            case OP_GET_UPVALUE: {
            }
            case OP_SET_UPVALUE: {
            }
            case OP_GET_PROPERTY: {
            }
            case OP_SET_PROPERTY: {
            }
            case OP_GET_SUPER: {
            }
            case OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a, b)));
                break;
            }
            case OP_GREATER: {
                BINARY_OP(BOOL_VAL, >);
                break;
            }
            case OP_LESS: {
                BINARY_OP(BOOL_VAL, <);
                break;
            }
            case OP_ADD: {
                if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                    concatenate();
                } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                } else {
                    runtimeError("Operands must be two numbers or two strings");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SUBTRACT: {
                BINARY_OP(NUMBER_VAL, -);
                break;
            }
            case OP_MULTIPLY: {
                BINARY_OP(NUMBER_VAL, *);
                break;
            }
            case OP_DIVIDE: {
                BINARY_OP(NUMBER_VAL, /);
                break;
            }
            case OP_NOT: {
                push(BOOL_VAL(isFalsey(pop())));
                break;
            }
            case OP_NEGATE: {
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            }
            case OP_PRINT: {
                printValue(pop());
                printf("\n");
                break;
            }
            case OP_JUMP: {
            }
            case OP_JUMP_IF_FALSE: {
            }
            case OP_CALL: {
            }
            case OP_INVOKE: {
            }
            case OP_SUPER_INVOKE: {
            }
            case OP_CLOSURE: {
            }
            case OP_CLOSE_UPVALUE: {
            }
            case OP_LOOP: {
            }
            case OP_RETURN: {
                Value result = pop();
                closeUpvalues(frame->slots);
                vm.frameCount--;
                if (vm.frameCount == 0) {
                    pop();
                    return INTERPRET_OK;
                }

                vm.stackTop = frame->slots;
                push(result);
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
            case OP_CLASS: {
            }
            case OP_INHERIT: {
            }
            case OP_METHOD: {
            }
        }
    }

    #undef READ_BYTE
    #undef READ_SHORT
    #undef READ_CONSTANT
    #undef READ_STRING
    #undef BINARY_OP

}

InterpretResult interpret(char *source) {
    ObjFunction *function = compile(source);
    if (function == NULL) return INTERPRET_COMPILE_ERROR;

    push(OBJ_VAL(function));
    ObjClosure *closure = newClosure(function);
    pop();
    push(OBJ_VAL(closure));
    call(closure, 0);

    return run();
}