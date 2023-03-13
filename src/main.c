#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    (void)argv;
    if (argc != 2) {
        fprintf(stderr, "Usage: mazegenerator seed\n");
        exit(1);
    }
}
