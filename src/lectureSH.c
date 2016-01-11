/* 
Crée le 4 janvier 2016 par Loïc
Lecture de la table des sections
*/
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include "lecture_headers.h"
#include "lectureSH.h"

char* sectionTypeString(uint32_t sh_type) {
	char* typeString =  (char*) malloc(sizeof(char)*10);
	if (typeString==NULL) {
		printf("\nErreur lors de l'allocation d'une chaine de type.\n");
		return NULL;
	}

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
		case SHT_ARM_EXIDX:
			typeString = "ARM_EXIDX";
			break;
		case SHT_ARM_PREEMPTMAP:
			typeString = "ARM_PREEMPTMAP";
			break;
		case SHT_ARM_ATTRIBUTES:
			typeString = "ARM_ATTRIBUTES";
			break;
		case SHT_ARM_DEBUGOVERLAY:
			typeString = "ARM_DEBUGOVERLAY";
			break;
		case SHT_ARM_OVERLAYSECTION:
			typeString = "ARM_OVERLAYSECTION";
			break;
		default:
			typeString = "# ERR #";
			break;
	}

	return typeString;
}

char* fetchSectionNames(FILE* f, Elf32_Ehdr elfHeader, Elf32_Shdr* shTable) {
	int i;

	char* names = (char*) malloc(sizeof(char)*shTable[elfHeader.e_shstrndx].sh_size);
	if (names==NULL) {
		printf("\nErreur lors de l'allocation initiale de la table des noms.\n");
		return NULL;
	}

	fseek(f, shTable[elfHeader.e_shstrndx].sh_offset, 0);
	for(i=0; i<shTable[elfHeader.e_shstrndx].sh_size; i++)
		names[i] = fgetc(f);
	
	return names;
}

char* getSectionName(char* names, uint32_t nameIndex) {
	int i = 1;

	while(names[nameIndex+i] != '\0')
		i++;

	char* sectionName = (char*) malloc(sizeof(char)*i);
	if (names==NULL) {
		printf("\nErreur lors de l'allocation initiale d'un nom de section.\n");
		return NULL;
	}

	i = 0;
	while(names[nameIndex+i] != '\0') {
		sectionName[i] = names[nameIndex+i];
		i++;
	}
	sectionName[i] = '\0';

	return sectionName;
}

char* getSectionNameBis(char* names, Elf32_Shdr sectionHeader) {
	return getSectionName(names, sectionHeader.sh_name);
}

char* sectionFlagsTranslation(uint32_t flags) {
	char* flagsString = (char*) malloc(sizeof(char)*FLAG_STRING_LENGTH);
	if (flagsString==NULL) {
		printf("\nErreur lors d'allocation de flagsString.\n");
		return NULL;
	}

	int i = 0;
	if (flags&SHF_WRITE) {
		flagsString[i++] = 'W';
	}
	if (flags&SHF_ALLOC) {
		flagsString[i++] = 'A';
	}
	if (flags&SHF_EXECINSTR) {
		flagsString[i++] = 'X';
	}
	if (flags&SHF_MERGE) {
		flagsString[i++] = 'M';
	}
	if (flags&SHF_STRINGS) {
		flagsString[i++] = 'S';
	}
	if (flags&SHF_INFO_LINK) {
		flagsString[i++] = 'I';
	}
	if (flags&SHF_LINK_ORDER) {
		flagsString[i++] = 'L';
	}
	if (flags&SHF_GROUP) {
		flagsString[i++] = 'G';
	}
	if (flags&SHF_TLS) {
		flagsString[i++] = 'T';
	}
	if (flags&SHF_EXCLUDE) {
		flagsString[i++] = 'E';
	}
	if (flags&SHF_OS_NONCONFORMING) {
		flagsString[i++] = 'O';
	}
	if (flags&SHF_MASKOS) {
		flagsString[i++] = 'o';
	}
	if (flags&SHF_MASKPROC) {
		flagsString[i++] = 'p';
	}
	flagsString[i] = '\0';

	return flagsString;
}

void afficherTableSections(FILE* f, Elf32_Ehdr elfHeader, Elf32_Shdr* shTable) {
	char* type;
	char* nom;
	char* flags;
	int i;

	char* names = fetchSectionNames(f, elfHeader, shTable);

	printf("There are %d section headers, starting at offset 0x%x:\n\n", elfHeader.e_shnum, elfHeader.e_shoff);
	printf("Section Headers:\n");
	printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
	
	for(i=0; i<elfHeader.e_shnum; i++) {
		type = sectionTypeString(shTable[i].sh_type);
		nom = getSectionName(names, shTable[i].sh_name);
		flags = sectionFlagsTranslation(shTable[i].sh_flags);

		if(strlen(nom)>17)
			nom[17] = '\0';
		printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %3s %2d %3d %2d\n", i, nom, type, shTable[i].sh_addr, shTable[i].sh_offset, shTable[i].sh_size, shTable[i].sh_entsize, sectionFlagsTranslation(shTable[i].sh_flags), shTable[i].sh_link, shTable[i].sh_info, shTable[i].sh_addralign);
		free(nom);
		free(flags);
	}
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");

	free(names);
}

Elf32_Shdr* lectureSectionHeader(FILE *f, Elf32_Ehdr elfHeader, int silent) {
	int i;

	// Allocation de la table des en-têtes de section
	Elf32_Shdr* shTable = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr)*elfHeader.e_shnum);
	if (shTable==NULL) {
		printf("\nErreur lors de l'allocation initiale de shTable.\n");
		return NULL;
	}

	fseek(f, elfHeader.e_shoff, 0);
	for(i=0; i<elfHeader.e_shnum; i++) {
		shTable[i].sh_name = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		shTable[i].sh_type = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		shTable[i].sh_flags = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		shTable[i].sh_addr = (Elf32_Addr) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		shTable[i].sh_offset = (Elf32_Off) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		shTable[i].sh_size = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		shTable[i].sh_link = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		shTable[i].sh_info = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		shTable[i].sh_addralign = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
		shTable[i].sh_entsize = (uint32_t) lire_octets(elfHeader.e_ident[EI_DATA], f, 4);
	}

	if (!silent) {
		afficherTableSections(f, elfHeader, shTable);
	}

	return shTable;
}
