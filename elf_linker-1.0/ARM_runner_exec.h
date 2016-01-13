#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "../elf_linker-1.0/debug.h"
#include "../elf_linker-1.0/arm_simulator_interface.h"
#include "../src/lecture_headers.h"



void run(char *hostname, char *servicename, FILE* f);
void lecture_fichier(FILE *f, uint32_t *address, size_t *size);
