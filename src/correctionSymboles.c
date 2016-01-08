/*
Crée le 6 janvier 2016 par Loïc
Correction des symboles
*/
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include "lecture_headers.h"
#include "lectureSH.h"
#include "lectureST.h"

size_t fwrite32(FILE* f, int mode, uint32_t value) {
	uint32_t bytes;
	if(mode==L_ENDIAN)
		bytes = ByteSwap32(value);
	else
		bytes = value;
	return fwrite(&bytes, 4, 1, f);
}

size_t fwrite16(FILE* f, int mode, uint16_t value) {
	uint16_t bytes;
	if(mode==L_ENDIAN)
		bytes = ByteSwap16(value);
	else
		bytes = value;
	return fwrite(&bytes, 2, 1, f);
}

size_t fwrite8(FILE* f, int mode, uint8_t value) {
	return fwrite(&value, 1, 1, f);
}

ListeSymboles corrigerSymboles(FILE* oldFile, FILE* newFile, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader, Elf32_Shdr* originalSH, Elf32_Shdr* newSH, ListeSymboles oldST, int silent) {
	ListeSymboles newST;
	int i, j;
	char* originalName;
	char* oldSectionNames = fetchSectionNames(oldFile, oldElfHeader, originalSH);
	char* newSectionNames = fetchSectionNames(newFile, newElfHeader, newSH);
	char* symbolNames;
	unsigned char info;
	unsigned char bind;
	
	newST.symboles = (Elf32_Sym*) malloc(sizeof(Elf32_Sym)*oldST.nbSymboles);
	if(newST.symboles == NULL)
	{
		printf("Erreur d'allocation\n");
		newST.symboles = NULL;
		newST.nbSymboles = 0;
		return newST;
	}

	if (!silent)
	{
		printf("New symbol table '.symtab' contains %d entries:\n", oldST.nbSymboles);
		printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

		i = 0;
		while(strcmp(getSectionNameBis(newSectionNames, newSH[i]), ".symtab"))
		{
			i++;
		}
		symbolNames = fetchSymbolNames(newFile, newSH, i);
	}

	for(j=0; j<oldST.nbSymboles; j++)
	{
		newST.symboles[j].st_name = oldST.symboles[j].st_name;
		newST.symboles[j].st_size = oldST.symboles[j].st_size;
		newST.symboles[j].st_info = oldST.symboles[j].st_info;
		newST.symboles[j].st_other = oldST.symboles[j].st_other;

		originalName = getSectionNameBis(oldSectionNames, originalSH[oldST.symboles[j].st_shndx]);
		i = 0;
		while (strcmp(originalName, getSectionNameBis(newSectionNames, newSH[i]))!=0 && i<newElfHeader.e_shnum) 
		{
			i++;
		}
		free(originalName);
		if (i==newElfHeader.e_shnum) 
		{
			printf("Erreur: Section introuvable dans le nouveau fichier.\n");
			newST.symboles = NULL;
			newST.nbSymboles = 0;
			return newST;
		}
		newST.symboles[j].st_shndx = i;

		newST.symboles[j].st_value = oldST.symboles[j].st_value + newSH[i].sh_addr;

		info = 15 & newST.symboles[j].st_info;
		bind = 15<<4 & newST.symboles[j].st_info;

		if (!silent)
		{
			if(newST.symboles[j].st_shndx == 0)
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  UND %s\n", j, newST.symboles[j].st_value, newST.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(newST.symboles[j].st_other), getSymbolNameBis(symbolNames, newST.symboles[j]));
			}
			else
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  %3d %s\n", j, newST.symboles[j].st_value, newST.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(newST.symboles[j].st_other), newST.symboles[j].st_shndx, getSymbolNameBis(symbolNames, newST.symboles[j]));
			}
		}
	}

	newST.nbSymboles = j;

	return newST;
}

void ecrireNouveauxSymboles(FILE* newFile, Elf32_Ehdr newElfHeader, Elf32_Shdr* newSH, ListeSymboles newST, int silent) {
	int i = 0;
	uint32_t writingOffset;
	char* newSectionNames = fetchSectionNames(newFile, newElfHeader, newSH);

	while(strcmp(getSectionNameBis(newSectionNames, newSH[i]), ".symtab"))
	{
		i++;
	}
	writingOffset = newSH[i].sh_offset;

	fseek(newFile, writingOffset, 0);
	for(i = 0; i<newST.nbSymboles; i++)
	{
		fwrite32(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_name);
		fwrite32(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_value);
		fwrite32(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_size);
		fwrite8(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_info);
		fwrite8(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_other);
		fwrite16(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_shndx);
	}
}