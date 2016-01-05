/*
Creer le 5 janvier 2016 par Thai Binh
Lecture de la table des symboles
*/


#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "Lecture_elf.h"
#include "lectureSH.h"


int lectureSymbolTab(FILE *f, int sectionSymbolTabOffset, int sectionSymbolTabSize)
{
	
	Elf32_Sym *symboleCourant = (Elf32_Sym) malloc(sizeof(Elf32_Sym)*(sectionSymbolTabSize/16);

	if(symboleCourant == NULL)
	{
		printf("Erreur d'allocation\n");
		return;
	}

	printf("Symbol Table: \n");
	printf("Num:      Value    Size Type     Other Name\n");

	int j = 0;

	for(int i=0; i<sectionSymbolTabSize; i=i+16)
	{

		

		symboleCourant[j].st_name = (uint32_t) lire_octets(BIG_ENDIAN,f,4);
		symboleCourant[j].st_value = (Elf32_Addr) lire_octets(BIG_ENDIAN,f,4);
		symboleCourant[j].st_size = (uint32_t) lire_octets(BIG_ENDIAN,f,4);
		symboleCourant[j].st_info = (unsigned char) lire_octets(BIG_ENDIAN,f,1);
		symboleCourant[j].st_other = (unsigned char) lire_octets(BIG_ENDIAN,f,1);
		symboleCourant[j].st_shndx = (uint16_t) lire_octets(BIG_ENDIAN,f,2);

		

		printf("%2d %8x %d %d %d %x %s\n", j, symboleCourant[j].st_value, symboleCourant[j].st_size, symboleCourant[j].st_info, 
			symboleCourant[j].st_other, symboleCourant[j].st_shndx, symboleCourant[j].st_name);


	}

}

char* typeSymbole(unsigned char info)
{
	char* typeSymbole[128];

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
		}
	return typeSymbole;
}

