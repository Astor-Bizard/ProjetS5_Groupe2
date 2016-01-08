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

ListeSymboles lectureSymbolTab(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *sectionHeader, int silent)
{
	uint32_t sectionSymbolTabSize;
	uint32_t sectionSymbolTabOffset;
	char* names = fetchSectionNames(f, elfHeader, sectionHeader);
	char* symbolNames;
	int i=0;
	int j=0;
	ListeSymboles listeSymboles;

	while(strcmp(getSectionNameBis(names,sectionHeader[i]), ".symtab"))
	{
		i++;
	}
	sectionSymbolTabSize = sectionHeader[i].sh_size;
	sectionSymbolTabOffset = sectionHeader[i].sh_offset;
	symbolNames = fetchSymbolNames(f,sectionHeader,i);

	fseek(f,sectionSymbolTabOffset,SEEK_SET);

	listeSymboles.symboles = (Elf32_Sym*) malloc(sizeof(Elf32_Sym)*(sectionSymbolTabSize/16));
	if(listeSymboles.symboles == NULL)
	{
		printf("Erreur d'allocation\n");
		listeSymboles.symboles = NULL;
		listeSymboles.nbSymboles = 0;
		return listeSymboles;
	}

	if (!silent)
	{
		printf("Symbol table '.symtab' contains %d entries:\n",sectionSymbolTabSize/16);
		printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
	}
	
	for(i = 0; i<sectionSymbolTabSize; i=i+16)
	{
		listeSymboles.symboles[j].st_name = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA],f,4);
		listeSymboles.symboles[j].st_value = (Elf32_Addr) lire_octets(elfHeader.e_ident[EI_DATA],f,4);
		listeSymboles.symboles[j].st_size = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA],f,4);
		listeSymboles.symboles[j].st_info = (unsigned char) lire_octets(elfHeader.e_ident[EI_DATA],f,1);
		listeSymboles.symboles[j].st_other = (unsigned char) lire_octets(elfHeader.e_ident[EI_DATA],f,1);
		listeSymboles.symboles[j].st_shndx = (uint16_t) lire_octets(elfHeader.e_ident[EI_DATA],f,2);

		unsigned char info = 15 & listeSymboles.symboles[j].st_info;
		unsigned char bind = 15<<4 & listeSymboles.symboles[j].st_info;

		if (!silent)
		{
			if(listeSymboles.symboles[j].st_shndx == 0)
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  UND %s\n", j, listeSymboles.symboles[j].st_value, listeSymboles.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(listeSymboles.symboles[j].st_other), getSymbolNameBis(symbolNames,listeSymboles.symboles[j]));
			}
			else
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  %3d %s\n", j, listeSymboles.symboles[j].st_value, listeSymboles.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(listeSymboles.symboles[j].st_other), listeSymboles.symboles[j].st_shndx, getSymbolNameBis(symbolNames,listeSymboles.symboles[j]));
			}
		}
		j++;
	}

	listeSymboles.nbSymboles = j;

	return listeSymboles;
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

char* fetchSymbolNames(FILE* f, Elf32_Shdr* shTable, int symbolTabIndex) 
{
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

char* getSymbolName(char* symbols, uint32_t symbolIndex) 
{
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

char* getSymbolNameBis(char* symbols, Elf32_Sym symbol) 
{
	return getSymbolName(symbols, symbol.st_name);
}
