#include "rwlock.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* *argv) {
    printf("RWLock vendor is %s\n", RWLockVendor());
    return EXIT_SUCCESS;
}
