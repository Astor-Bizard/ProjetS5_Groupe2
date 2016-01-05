/*
Creer le 5 janvier 2016 par Thai Binh
Lecture de la table des symboles
*/
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "lecture_headers.h"
#include "lectureSH.h"
#include "lectureST.h"

Elf32_Sym *lectureSymbolTab(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr sectionHeader, int silent)
{
	int sectionSymbolTabSize;
	int sectionSymbolTabOffset;
	initSymbolTabUsefullInfo(sectionHeader, *sectionSymbolTabSize, *sectionSymbolTabOffset);

	fseek(f,sectionSymbolTabOffset,SEEK_SET);

	Elf32_Sym *symbolTab = (Elf32_Sym) malloc(sizeof(Elf32_Sym)*(sectionSymbolTabSize/16);
	if(symbolTab == NULL)
	{
		printf("Erreur d'allocation\n");
		return;
	}

	if (!silent)
	{
		printf("Symbol Table: \n");
		printf("Num:      Value    Size Type    BIND   Other Name\n");
	}
	int j = 0;

	for(int i=0; i<sectionSymbolTabSize; i=i+16)
	{
		symbolTab[j].st_name = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA],f,4);
		symbolTab[j].st_value = (Elf32_Addr) lire_octets(elfHeader.e_ident[EI_DATA],f,4);
		symbolTab[j].st_size = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA],f,4);
		symbolTab[j].st_info = (unsigned char) lire_octets(elfHeader.e_ident[EI_DATA],f,1);
		symbolTab[j].st_other = (unsigned char) lire_octets(elfHeader.e_ident[EI_DATA],f,1);
		symbolTab[j].st_shndx = (uint16_t) lire_octets(elfHeader.e_ident[EI_DATA],f,2);

		unsigned char info = 1<<4 & symbolTab[j].st_info;
		unsigned char bind = 1&0xf & symbolTab[j].st_info;

		if (!silent)
		{
			printf("%2d %8x %d %s %s %d %x %s\n", j, symbolTab[j].st_value, symbolTab[j].st_size, typeSymbole(info), bindSymbole(bind),
				symbolTab[j].st_other, symbolTab[j].st_shndx, symbolTab[j].st_name);
		}

	}

	return symbolTab;
}

char *typeSymbole(unsigned char info)
{
	char typeSymbole[128];

	switch(info)
	{
		case STT_NOTYPE:
			typeSymbole = "NOTYPE";
			break;
		case STT_OBJECT:
			typeSymbole = "OBJECT";
			break;
		case STT_FUNC:
			typeSymbole = "FUNC";
			break;
		case STT_SECTION:
			typeSymbole = "SECTION";
			break;
		case STT_FILE:
			typeSymbole = "FILE";
			break;
		case STT_LOPROC:
			typeSymbole = "LOPROC";
			break;
		case STT_HIPROC:
			typeSymbole = "HIPROC";
			break;
		default:
			typeSymbole = "# ERR #";
			break;
	}
	return typeSymbole;
}

char* bindSymbole(unsigned char bind)
{
	char bindSymbole[128];

	switch(bind)
	{
		case STB_LOCAL:
			bindSymbole = "LOCAL";
			break;
		case STB_GLOBAL:
			bindSymbole = "GLOBAL";
			break;
		case STB_WEAK:
			bindSymbole = "WEAK";
			break;
		case STB_LOPROC:
			bindSymbole = "LOPROC";
			break;
		case STB_HIPROC:
			bindSymbole = "HIPROC";
			break;
		default:
			bindSymbole = "# ERR #";
			break;
	}
	return bindSymbole;
}

void initSymbolTabUsefullInfo(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr sectionHeader, int *size, int *offset)
{
	int i=0;
	char** names;
	names = getSectionsNames(f, elfHeader, sectionHeader);

	while(names[sectionHeader[i].sh_name] != ".symtab"){}

	size = sectionHeader[i].sh_size;
	offset = sectionHeader[i].sh_offset;

	return;
}