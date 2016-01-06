#ifndef LECTURE_HEADERS
#define LECTURE_HEADERS

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "lectureSH.h"
#include "lecture_headers.h"
#include "afficher_section.h"

//long long int lire_octets_charT(int mode, FILE *f, int nb_octet);
void affichage_relocation(char* f,Elf32_Ehdr header,Elf_32Shdr* table_section);

#endif