#ifndef AFFICHAGE_RELOCATION
#define AFFICHAGE_RELOCATION

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "lectureSH.h"
#include "lectureST.h"
#include "lecture_headers.h"
#include "afficher_section.h"

//long long int lire_octets_charT(int mode, FILE *f, int nb_octet);


typedef struct 
{
	int nb_Rel;
	int nb_Rela;
	Elf32_Rel *Rel;
	int *Sec_Rel;
	Elf32_Rela *Rela;
	int *Sec_Rela;
}Str_Reloc;

//Affiche les informations de relocation, et renvoie la structure de relocation
Str_Reloc affichage_relocation(FILE* f,Elf32_Ehdr header,Elf32_Shdr* table_section, ListeSymboles table_symbol,int silent);


//lit une suite d'octet dans un certain mode
long long unsigned int lire_octets_charT(unsigned char *tableau, int hdr_mode, int debut, int nombre);

// affiche le type de la relocation
void type_relocation(int info);

// affiche le nom de la section de relocation
void print_section(unsigned long long int addr,Elf32_Shdr* table_section,Elf32_Ehdr header, char* SectionNames);

//affiche la valeur et le nom d'un symbol
void print_symbol(int sym, ListeSymboles table_symbol,Elf32_Ehdr header, char* SymbolNames);

// affiche les infos de relocation Rel
void afficher_sectionR(FILE *f,Elf32_Shdr* table_section,Elf32_Ehdr header,int numS, char* SectionNames,Str_Reloc *RETOUR, ListeSymboles table_symbol, char* SymbolNames, int silent);

//affiche les infos de relocation Rela
//void afficher_sectionRA(char *f,Elf32_Shdr* table_section,Elf32_Ehdr header,int numS, char* SectionNames,Str_Reloc *RETOUR, ListeSymboles table_symbol, char* SymbolNames);

#endif
