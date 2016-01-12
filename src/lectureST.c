/*
Creer le 5 janvier 2016 par Thai Binh
Lecture de la table des symboles
*/
#include "lectureST.h"

ListeSymboles lectureSymbolTab(FILE *f, Elf32_Ehdr elfHeader, SectionsHeadersList shList, int silent)
{
	uint32_t sectionSymbolTabSize;
	uint32_t sectionSymbolTabOffset;
	int i = 0;
	int j = 0;
	ListeSymboles listeSymboles;

	char* nomSection = getSectionNameBis(shList.names, shList.headers[i]);
	while(strcmp(nomSection, ".symtab"))
	{
		free(nomSection);
		i++;
		nomSection = getSectionNameBis(shList.names, shList.headers[i]);
	}
	free(nomSection);
	sectionSymbolTabSize = shList.headers[i].sh_size;
	sectionSymbolTabOffset = shList.headers[i].sh_offset;

	listeSymboles.symboles = (Elf32_Sym*) malloc(sizeof(Elf32_Sym)*(sectionSymbolTabSize/16));
	if(listeSymboles.symboles == NULL)
	{
		printf("Erreur d'allocation\n");
		listeSymboles.symboles = NULL;
		listeSymboles.names = NULL;
		listeSymboles.nbSymboles = 0;
		return listeSymboles;
	}
	listeSymboles.names = fetchSymbolNames(f, shList, i);

	fseek(f, sectionSymbolTabOffset, SEEK_SET);
	for(i = 0; i<sectionSymbolTabSize; i=i+16)
	{
		listeSymboles.symboles[j].st_name = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		listeSymboles.symboles[j].st_value = (Elf32_Addr) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		listeSymboles.symboles[j].st_size = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		listeSymboles.symboles[j].st_info = (unsigned char) lire_octets(elfHeader.e_ident[EI_DATA], f, 1);
		listeSymboles.symboles[j].st_other = (unsigned char) lire_octets(elfHeader.e_ident[EI_DATA], f, 1);
		listeSymboles.symboles[j].st_shndx = (uint16_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 2);
		j++;
	}
	listeSymboles.nbSymboles = j;

	if (!silent)
		afficherTableSymboles(ListeSymboles listeSymboles);

	return listeSymboles;
}

void afficherTableSymboles(ListeSymboles listeSymboles) {
	char* symbolString;
	unsigned char bind, info;
	int j;

	printf("\nSymbol table '.symtab' contains %d entries:\n", listeSymboles.nbSymboles);
	printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

	for(j=0; j<listeSymboles.nbSymboles; j++)
	{
		info = 15 & listeSymboles.symboles[j].st_info;
		bind = 15<<4 & listeSymboles.symboles[j].st_info;

		symbolString = getSymbolNameBis(listeSymboles.names, listeSymboles.symboles[j]);

		if(listeSymboles.symboles[j].st_shndx == 0)
		{
			printf("   %3d: %08x %5d %-7s %-6s %-7s  UND %s\n", j, listeSymboles.symboles[j].st_value, listeSymboles.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(listeSymboles.symboles[j].st_other), symbolString);
		}
		else
		{
			printf("   %3d: %08x %5d %-7s %-6s %-7s  %3d %s\n", j, listeSymboles.symboles[j].st_value, listeSymboles.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(listeSymboles.symboles[j].st_other), listeSymboles.symboles[j].st_shndx, symbolString);
		}
		free(symbolString);
	}
}

char* typeSymbole(unsigned char info)
{
	char* typeSymboleChar;

	switch(info)
	{
		case STT_NOTYPE:
			typeSymboleChar = "NOTYPE";
			break;
		case STT_OBJECT:
			typeSymboleChar = "OBJECT";
			break;
		case STT_FUNC:
			typeSymboleChar = "FUNC";
			break;
		case STT_SECTION:
			typeSymboleChar = "SECTION";
			break;
		case STT_FILE:
			typeSymboleChar = "FILE";
			break;
		case STT_LOPROC:
			typeSymboleChar = "LOPROC";
			break;
		case STT_HIPROC:
			typeSymboleChar = "HIPROC";
			break;
		default:
			typeSymboleChar = "# ERR #";
			break;
	}
	return typeSymboleChar;
}

char* bindSymbole(unsigned char bind)
{
	char* bindSymboleChar;

	switch(bind>>4)
	{
		case STB_LOCAL:
			bindSymboleChar = "LOCAL";
			break;
		case STB_GLOBAL:
			bindSymboleChar = "GLOBAL";
			break;
		case STB_WEAK:
			bindSymboleChar = "WEAK";
			break;
		case STB_LOPROC:
			bindSymboleChar = "LOPROC";
			break;
		case STB_HIPROC:
			bindSymboleChar = "HIPROC";
			break;
		default:
			bindSymboleChar = "# ERR #";
			break;
	}
	return bindSymboleChar;
}

char* visionSymbole(unsigned char vis)
{
	char* visionSymboleChar;

	switch(vis)
	{
		case 0:
			visionSymboleChar = "DEFAULT";
			break;
		default:
			visionSymboleChar = "# ERR #";
			break;
	}
	return visionSymboleChar;
}

char* fetchSymbolNames(FILE* f, SectionsHeadersList shList, int symbolTabIndex) 
{
	int i;

	char* symbols = (char*) malloc(sizeof(char)*shList.headers[symbolTabIndex+1].sh_size);
	if (symbols==NULL) {
		printf("Erreur lors de l'allocation initiale de la table des noms.");
		return NULL;
	}

	fseek(f, shList.headers[symbolTabIndex+1].sh_offset, 0);
	for(i=0; i<shList.headers[symbolTabIndex+1].sh_size; i++)
		symbols[i] = fgetc(f);
	
	return symbols;
}

char* getSymbolName(char* symbols, uint32_t symbolIndex) 
{
	int i = 0;

	while(symbols[symbolIndex+i] != '\0')
		i++;
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
	symbolName[i] = '\0';

	return symbolName;
}

char* getSymbolNameBis(char* symbols, Elf32_Sym symbol) 
{
	return getSymbolName(symbols, symbol.st_name);
}
