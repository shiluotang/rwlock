#include "rwlock.h"

#include <stdlib.h>
#include <stdio.h>

#include <windows.h>

#define DUMP_MACRO(X) printf(#X " = 0x%lX\n", (long) (X))

#ifdef _WIN32
#   define _WIN32_DEFINED "yes"
#else
#   define _WIN32_DEFINED "no"
#endif
#ifdef _WIN64
#   define _WIN64_DEFINED "yes"
#else
#   define _WIN64_DEFINED "no"
#endif

int main(int argc, char* *argv) {
    DUMP_MACRO(WINVER);
    DUMP_MACRO(_WIN32_WINNT);
    printf("RWLock vendor is %s\n", RWLockVendor());
    printf("_WIN32 is defined: %s\n", _WIN32_DEFINED);
    printf("_WIN64 is defined: %s\n", _WIN64_DEFINED);
    return EXIT_SUCCESS;
}
