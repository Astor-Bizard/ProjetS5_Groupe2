/*
Crée le 4 janvier 2016 par Loïc
Lecture de la table des sections
*/
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "lecture_headers.h"

void sectionTypeString(uint32_t sh_type, char* typeString) {
	switch(sh_type) {
		case SHT_NULL:
			typeString = "NULL";
			break;
		case SHT_PROGBITS:
			typeString = "PROGBITS";
			break;
		case SHT_SYMTAB:
			typeString = "SYMTAB";
			break;
		case SHT_STRTAB:
			typeString = "STRTAB";
			break;
		case SHT_RELA:
			typeString = "RELA";
			break;
		case SHT_HASH:
			typeString = "HASH";
			break;
		case SHT_DYNAMIC:
			typeString = "DYNAMIC";
			break;
		case SHT_NOTE:
			typeString = "NOTE";
			break;
		case SHT_NOBITS:
			typeString = "NOBITS";
			break;
		case SHT_REL:
			typeString = "REL";
			break;
		case SHT_SHLIB:
			typeString = "SHLIB";
			break;
		case SHT_DYNSYM:
			typeString = "DYNSYM";
			break;
		case SHT_LOPROC:
			typeString = "LOPROC";
			break;
		case SHT_HIPROC:
			typeString = "HIPROC";
			break;
		case SHT_LOUSER:
			typeString = "LOUSER";
			break;
		case SHT_HIUSER:
			typeString = "HIUSER";
			break;
		default:
			typeString = "# ERR #";
			break;
	}
}

char** getSectionsNames(FILE* f, uint16_t sectionHeaderCount, uint32_t tableSize, Elf32_Off tableOffset) {
	int sLength; // Taille courante du tampon
	char c; // Caractère lu

	// Initialisation du tableau de chaines contenant les noms des sections. Il contient 'sectionHeaderCount' chaines, plus une chaine vide.
	char** names = (char**) malloc(sizeof(char*)*sectionHeaderCount+1);
	if (names==NULL) {
		printf("Erreur lors de l'allocation initiale de la table des noms.");
		return NULL;
	}

	// Initialisation du tampon qui sert à récuperer chaque chaine.
	char* tampon = (char*) malloc(tableSize*sizeof(char));
	if (tampon==NULL) {
		printf("Erreur lors de l'allocation du tampon de remplissage de la table des noms.");
		free(names);
		return NULL;
	}

	fseek(f, tableOffset); // On se place au depart de la table des noms dans le fichier
	// On cherche autant de chaines qu'il y a de Section Header, plus la chaine vide au debut du tableau.
	for(i=0; i<sectionHeaderCount+1; i++) { 
		sLength = 0;

		c = fgetc(f);
		while(c != '\0') { // Tant qu'on ne change pas de chaine
			tampon[sLength] = c;
			sLength++;
			c = fgetc(f);
		}
		tampon[sLength] = c;
		sLength++;

		// Allocation de la chaine dans le tableau, de la même longueur que le tampon(longueur courante).
		names[i] = (char*) malloc(sLength*sizeof(char));
		if (names[i]==NULL) {
			printf("Erreur lors de l'allocation d'une entrée de la table des noms.");
			// On libère les objets
			free(tampon);
			for(j=0; j<i; j++)
				free(names[i]);
			free(names);

			return NULL;
		}

		for(j=0; j<sLength; j++)
			names[i][j] = tampon[j];
	}
	free(tampon);

	return &names;
}

Elf32_Shdr* lectureSectionHeader(FILE *f, Elf32_Off sectionHeaderOffset, uint16_t sectionHeaderSize, uint16_t sectionHeaderCount, uint16_t sectionHeaderStringTableIndex, unsigned char mode_elf, int silent) {
	int i, j;
	char[10] type;

	// Allocation de la table des en-têtes de section
	Elf32_Shdr* shTable = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr)*sectionHeaderCount);
	if (shTable==NULL) {
		printf("Erreur lors de l'allocation initiale de shTable.");
		return NULL;
	}
	
	if (!silent) { 
		printf("Section Headers: \n");
		printf("[Nr] Name              Type            Addr     Off    Size   ES Flg      Lk Inf Al\n");
	}

	fseek(f, sectionHeaderOffset);
	for(i=0; i<sectionHeaderCount; i++) {
		shTable[i].sh_name = (uint32_t) lire_octets(mode_elf, f, 4);
		shTable[i].sh_type = (uint32_t) lire_octets(mode_elf, f, 4);
		shTable[i].sh_flags = (uint32_t) lire_octets(mode_elf, f, 4);
		shTable[i].sh_addr = (Elf32_Addr) lire_octets(mode_elf, f, 4);
		shTable[i].sh_offset = (Elf32_Off) lire_octets(mode_elf, f, 4);
		shTable[i].sh_size = (uint32_t) lire_octets(mode_elf, f, 4,);
		shTable[i].sh_link = (uint32_t) lire_octets(mode_elf, f, 4);
		shTable[i].sh_info = (uint32_t) lire_octets(mode_elf, f, 4);
		shTable[i].sh_addralign = (uint32_t) lire_octets(mode_elf, f, 4);
		shTable[i].sh_entsize = (uint32_t) lire_octets(mode_elf, f, 4);
	}

	char** names = getSectionsNames(f, sectionHeaderCount, shTable[sectionHeaderStringTableIndex].sh_size, shTable[sectionHeaderStringTableIndex].sh_offset);

	if (!silent) {
		for(i=0; i<sectionHeaderCount; i++) {
			sectionTypeString(shTable[i].sh_type, &type);

			printf("[%2d] %s %s %8x %6x %6x %02x %08d %2d %3d %2d\n", i, names[shTable[i].sh_name], type, shTable[i].sh_addr, shTable[i].sh_offset, shTable[i].sh_size, shTable[i].sh_entsize, shTable[i].sh_flags, shTable[i].sh_link, shTable[i].sh_info, shTable[i].sh_addralign);
		}
	}

	return shTable;
}
