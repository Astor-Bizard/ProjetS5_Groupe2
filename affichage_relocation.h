#ifndef AFFICHAGE_RELOCATION
#define AFFICHAGE_RELOCATION

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "lectureSH.h"
#include "lecture_headers.h"
#include "afficher_section.h"

//long long int lire_octets_charT(int mode, FILE *f, int nb_octet);


typedef struct 
{
	int nb_Rel;
	int nb_Rela;
	Elf32_Rel *Rel;
	Elf32_Rela *Rela;
}Str_Reloc;


void affichage_relocation(char* f,Elf32_Ehdr header,Elf32_Shdr* table_section);

#endif
