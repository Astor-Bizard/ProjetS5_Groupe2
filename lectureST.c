/*
Creer le 5 janvier 2016 par Thai Binh
Lecture de la table des symboles
*/
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "lecture_header.h"
#include "lectureSH.h"


Elf32_Sym* lectureSymbolTab(FILE *f, int sectionSymbolTabOffset, int sectionSymbolTabSize, int silent)
{
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
		printf("Num:      Value    Size Type     Other Name\n");
	}

	int j = 0;

	for(int i=0; i<sectionSymbolTabSize; i=i+16)
	{
		symbolTab[j].st_name = (uint32_t) lire_octets(BIG_ENDIAN,f,4);
		symbolTab[j].st_value = (Elf32_Addr) lire_octets(BIG_ENDIAN,f,4);
		symbolTab[j].st_size = (uint32_t) lire_octets(BIG_ENDIAN,f,4);
		symbolTab[j].st_info = (unsigned char) lire_octets(BIG_ENDIAN,f,1);
		symbolTab[j].st_other = (unsigned char) lire_octets(BIG_ENDIAN,f,1);
		symbolTab[j].st_shndx = (uint16_t) lire_octets(BIG_ENDIAN,f,2);

		if (!silent)
		{
			printf("%2d %8x %d %s %d %x %s\n", j, symbolTab[j].st_value, symbolTab[j].st_size, typeSymbole(symbolTab[j].st_info), 
				symbolTab[j].st_other, symbolTab[j].st_shndx, symbolTab[j].st_name);
		}

	}

	return symbolTab;

}

char* typeSymbole(unsigned char info)
{
	char *typeSymbole[128];

	switch(info)
		{
			case 0:
				typeSymbole = "NOTYPE";
				break;
			case 1:
				typeSymbole = "OBJECT";
				break;
			case 2:
				typeSymbole = "FUNC";
				break;
			case 3:
				typeSymbole = "SECTION";
				break;
			case 4:
				typeSymbole = "FILE";
				break;
			case 13:
				typeSymbole = "LOPROC";
				break;
			case 15:
				typeSymbole = "HIRPOC";
				break;
			default:
				typeSymbole = "UNDEFINED";
				break;
		}
	return typeSymbole;
}

