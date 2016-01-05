#ifndef LECTURE_HEADERS
#define LECTURE_HEADERS

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>

long long int lireoctets(int mode, FILE *f, int nb_octet);
Elf32_Ehdr lecture_Headers(FILE *f);

#endif