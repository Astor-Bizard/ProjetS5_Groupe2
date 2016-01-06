/*
Creer le 5 janvier 2016 par Thai Binh
Lecture de la table des symboles
*/
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include "lecture_headers.h"
#include "lectureSH.h"
#include "lectureST.h"

Elf32_Sym* lectureSymbolTab(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *sectionHeader, int silent)
{
	uint32_t sectionSymbolTabSize;
	uint32_t sectionSymbolTabOffset;
	char* names = fetchSectionNames(f, elfHeader, sectionHeader);

	initSymbolTabUsefullInfo(names, sectionHeader, &sectionSymbolTabSize, &sectionSymbolTabOffset);
	fseek(f,sectionSymbolTabOffset,SEEK_SET);

	Elf32_Sym *symbolTab = (Elf32_Sym*) malloc(sizeof(Elf32_Sym)*(sectionSymbolTabSize/16));

	if(symbolTab == NULL)
	{
		printf("Erreur d'allocation\n");
		return NULL;
	}

	if (!silent)
	{
		printf("Symbol Table: \n");
		printf("Num:    Value Size Type    Bind   Vis     Ndx Name\n");
	}
	int j = 0;
	int i;
	for(i = 0; i<sectionSymbolTabSize; i=i+16)
	{
		symbolTab[j].st_name = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA],f,4);
		symbolTab[j].st_value = (Elf32_Addr) lire_octets(elfHeader.e_ident[EI_DATA],f,4);
		symbolTab[j].st_size = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA],f,4);
		symbolTab[j].st_info = (unsigned char) lire_octets(elfHeader.e_ident[EI_DATA],f,1);
		symbolTab[j].st_other = (unsigned char) lire_octets(elfHeader.e_ident[EI_DATA],f,1);
		symbolTab[j].st_shndx = (uint16_t) lire_octets(elfHeader.e_ident[EI_DATA],f,2);

		unsigned char info = 15 & symbolTab[j].st_info;
		unsigned char bind = 15<<4 & symbolTab[j].st_info;

		if (!silent)
		{
			if(symbolTab[j].st_shndx == 0)
			{
				printf("%3d  %08x %4d %-7s %-6s %-7s UND %x\n", j, symbolTab[j].st_value, symbolTab[j].st_size, typeSymbole(info), bindSymbole(bind),
					visionSymbole(symbolTab[j].st_other), symbolTab[j].st_name);
			}
			else
				printf("%3d  %08x %4d %-7s %-6s %-7s %3d %x\n", j, symbolTab[j].st_value, symbolTab[j].st_size, typeSymbole(info), bindSymbole(bind),
					visionSymbole(symbolTab[j].st_other), symbolTab[j].st_shndx, symbolTab[j].st_name);
		}
		j++;

	}

	return symbolTab;
}

char* typeSymbole(unsigned char info)
{
	char* typeSymbole =  (char*) malloc(sizeof(char)*10);
	if (typeSymbole==NULL) {
		printf("Erreur lors de l'allocation d'une chaine de type.");
		return NULL;
	}

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
	char* bindSymbole =  (char*) malloc(sizeof(char)*10);
	if (bindSymbole==NULL) {
		printf("Erreur lors de l'allocation d'une chaine de type.");
		return NULL;
	}
	
	switch(bind>>4)
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

char* visionSymbole(unsigned char vis)
{
	char* visSymbole =  (char*) malloc(sizeof(char)*10);
	if (visSymbole==NULL) {
		printf("Erreur lors de l'allocation d'une chaine de type.");
		return NULL;
	}

	switch(vis)
	{
		case 0:
			visSymbole = "DEFAULT";
			break;
		default:
			visSymbole = "# ERR #";
			break;
	}
	return visSymbole;
}

void initSymbolTabUsefullInfo(char* names, Elf32_Shdr *sectionHeader, uint32_t *size, uint32_t *offset)
{
	int i=0;

	//while(!strcmp(names[sectionHeader[i].sh_name], ".symtab")){}
	while(strcmp(getSectionNameBis(names,sectionHeader[i]), ".symtab"))
	{
		i++;
	}
	*size = sectionHeader[i].sh_size;
	*offset = sectionHeader[i].sh_offset;
}
