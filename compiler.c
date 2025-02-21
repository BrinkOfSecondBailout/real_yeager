#include "compiler.h"
#include "scanner.h"


static void initCompiler(Compiler *compiler, FunctionType type) {
    
    
}

void compile(char *source) {
    initScanner(source);
    Compiler compiler;
    initCompiler(&compiler, TYPE_SCRIPT);
    for (;;) {

    }

}