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
	int *Sec_rel;
	Elf32_Rela *Rela;
	int *Sec_Rela
}Str_Reloc;


Str_Reloc affichage_relocation(char* f,Elf32_Ehdr header,Elf32_Shdr* table_section);

long long unsigned int lire_octets_charT(unsigned char *tableau, int hdr_mode, int debut, int nombre);

void type_relocation(int info);

void print_section(unsigned long long int addr,Elf32_Shdr* table_section,Elf32_Ehdr header, char* SectionNames);

void afficher_sectionR(char *f,Elf32_Shdr* table_section,Elf32_Ehdr header,int numS, char* SectionNames,Str_Reloc RETOUR);

void afficher_sectionRA(char *f,Elf32_Shdr* table_section,Elf32_Ehdr header,int numS, char* SectionNames,Str_Reloc RETOUR);

#endif
