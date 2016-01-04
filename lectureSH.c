/*
Creer le 4 janvier 2016 par Loïc
Lecture de la table des sections
*/
#include <stdlib.h>
#include <stdio.h>
#include "Lecture_elf.h"

typedef struct {
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Word sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} sectionHeader;

int lectureSectionHeader(FILE *f, int sectionHeaderOffset, int sectionHeaderSize, int sectionHeaderCount)
{
	sectionHeader* section_courante;
	
	printf("Section Headers: \n");
	printf("[Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

	for(int i=0; i<sectionHeaderCount; i++)
	{
		section_courante = (sectionHeader*) malloc(sizeof(sectionHeader));

		section_courante = lire_octets(BIG_ENDIAN,f,40);

		// TODO: afficher chaque parametre

		free(section_courante);
	}

	// Affichage SH fini
}
