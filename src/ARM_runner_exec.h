#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "arm_simulator_interface.h"
#include "lecture_headers.h"



void run(char *hostname, char *servicename, FILE* f);
void lecture_fichier(FILE *f, uint32_t *address, size_t *size);
