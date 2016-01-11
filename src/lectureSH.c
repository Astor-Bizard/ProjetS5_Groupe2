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

char* typeNameFromValue(uint32_t sh_type) {
	char* typeName;

	switch(sh_type) {
		case SHT_NULL:
			typeName = "NULL";
			break;
		case SHT_PROGBITS:
			typeName = "PROGBITS";
			break;
		case SHT_SYMTAB:
			typeName = "SYMTAB";
			break;
		case SHT_STRTAB:
			typeName = "STRTAB";
			break;
		case SHT_RELA:
			typeName = "RELA";
			break;
		case SHT_HASH:
			typeName = "HASH";
			break;
		case SHT_DYNAMIC:
			typeName = "DYNAMIC";
			break;
		case SHT_NOTE:
			typeName = "NOTE";
			break;
		case SHT_NOBITS:
			typeName = "NOBITS";
			break;
		case SHT_REL:
			typeName = "REL";
			break;
		case SHT_SHLIB:
			typeName = "SHLIB";
			break;
		case SHT_DYNSYM:
			typeName = "DYNSYM";
			break;
		case SHT_LOPROC:
			typeName = "LOPROC";
			break;
		case SHT_HIPROC:
			typeName = "HIPROC";
			break;
		case SHT_LOUSER:
			typeName = "LOUSER";
			break;
		case SHT_HIUSER:
			typeName = "HIUSER";
			break;
		case SHT_ARM_EXIDX:
			typeName = "ARM_EXIDX";
			break;
		case SHT_ARM_PREEMPTMAP:
			typeName = "ARM_PREEMPTMAP";
			break;
		case SHT_ARM_ATTRIBUTES:
			typeName = "ARM_ATTRIBUTES";
			break;
		case SHT_ARM_DEBUGOVERLAY:
			typeName = "ARM_DEBUGOVERLAY";
			break;
		case SHT_ARM_OVERLAYSECTION:
			typeName = "ARM_OVERLAYSECTION";
			break;
		default:
			typeName = "# ERR #";
			break;
	}

	return typeName;
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

char* getSectionName(char* names, uint32_t sh_name) {
	int i = 0;

	while(names[sh_name+i] != '\0')
		i++;
	i++;

	char* sectionName = (char*) malloc(sizeof(char)*i);
	if (names==NULL) {
		printf("\nErreur lors de l'allocation initiale d'un nom de section.\n");
		return NULL;
	}

	i = 0;
	while(names[sh_name+i] != '\0') {
		sectionName[i] = names[sh_name+i];
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

void displaySectionsHeaders(FILE* f, Elf32_Ehdr elfHeader, Elf32_Shdr* shTable) {
	int i;
	char* sectionName;
	char* flags;
	char* names = fetchSectionNames(f, elfHeader, shTable);

	printf("There are %d section headers, starting at offset 0x%x:\n\n", elfHeader.e_shnum, elfHeader.e_shoff);
	printf("Section Headers:\n");
	printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
	
	for(i=0; i<elfHeader.e_shnum; i++) {
		sectionName = getSectionName(names, shTable[i].sh_name);
		if(strlen(sectionName)>17)
			sectionName[17] = '\0';
		flags = sectionFlagsTranslation(shTable[i].sh_flags);

		printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %3s %2d %3d %2d\n", i, sectionName, typeNameFromValue(shTable[i].sh_type), shTable[i].sh_addr, shTable[i].sh_offset, shTable[i].sh_size, shTable[i].sh_entsize, flags, shTable[i].sh_link, shTable[i].sh_info, shTable[i].sh_addralign);
		
		free(flags);
		free(sectionName);
	}
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");

	free(names);
}

Elf32_Shdr* readSectionsHeadersFromFile(FILE *f, Elf32_Ehdr elfHeader, int silent) {
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
		displaySectionsHeaders(f, elfHeader, shTable);
	}

	return shTable;
}
