#include "vm.h"


InterpretResult interpret(char *source) {
    compile(source);
    return 0;
}