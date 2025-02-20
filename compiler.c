#include "compiler.h"
#include "scanner.h"

void compile(char *source) {
    if (startScan(source)) {
        printf("success\n");
    } else {
        printf("error\n");
    };
}