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
	char* symbolNames;
	int i=0;
	int j = 0;


	while(strcmp(getSectionNameBis(names,sectionHeader[i]), ".symtab"))
	{
		i++;
	}
	sectionSymbolTabSize = sectionHeader[i].sh_size;
	sectionSymbolTabOffset = sectionHeader[i].sh_offset;
	symbolNames = fetchSymbolNames(f,sectionHeader,i);

	//initSymbolTabUsefullInfo(names, sectionHeader, &sectionSymbolTabSize, &sectionSymbolTabOffset, symbolNames);	

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
				printf("%3d  %08x %4d %-7s %-6s %-7s UND %s\n", j, symbolTab[j].st_value, symbolTab[j].st_size, typeSymbole(info), bindSymbole(bind),
					visionSymbole(symbolTab[j].st_other), getSymbolNameBis(symbolNames,symbolTab[j]));
			}
			else
				printf("%3d  %08x %4d %-7s %-6s %-7s %3d %s\n", j, symbolTab[j].st_value, symbolTab[j].st_size, typeSymbole(info), bindSymbole(bind),
					visionSymbole(symbolTab[j].st_other), symbolTab[j].st_shndx, getSymbolNameBis(symbolNames,symbolTab[j]));
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

/*
void initSymbolTabUsefullInfo(char* names, Elf32_Shdr *sectionHeader, uint32_t *size, uint32_t *offset, char *symbolNames)
{
	
}
*/

char* fetchSymbolNames(FILE* f, Elf32_Shdr* shTable, int symbolTabIndex) {
	int i;

	char* symbols = (char*) malloc(sizeof(char)*shTable[symbolTabIndex].sh_size);
	if (symbols==NULL) {
		printf("Erreur lors de l'allocation initiale de la table des noms.");
		return NULL;
	}

	fseek(f, shTable[symbolTabIndex+1].sh_offset, 0);
	for(i=0; i<shTable[symbolTabIndex+1].sh_size; i++)
		symbols[i] = fgetc(f);
	
	return symbols;
}

char* getSymbolName(char* symbols, uint32_t symbolIndex) {
	int i = 1;

	while(symbols[symbolIndex+i] != '\0')
		i++;

	char* symbolName = (char*) malloc(sizeof(char)*i);
	if (symbols==NULL) {
		printf("Erreur lors de l'allocation initiale d'un nom de section.");
		return NULL;
	}

	i = 0;
	while(symbols[symbolIndex+i] != '\0') {
		symbolName[i] = symbols[symbolIndex+i];
		i++;
	}

	return symbolName;
}

char* getSymbolNameBis(char* symbols, Elf32_Sym symbol) {
	return getSymbolName(symbols, symbol.st_name);
}