#include "rwlock.h"

#include <stdlib.h>
#include <stdio.h>

#include <windows.h>

#define DUMP_MACRO(X) printf(#X " = 0x%lX\n", (long) (X))

int main(int argc, char* *argv) {
    DUMP_MACRO(WINVER);
    DUMP_MACRO(_WIN32_WINNT);
    printf("RWLock vendor is %s\n", RWLockVendor());
    return EXIT_SUCCESS;
}
