/*
Crée le 6 janvier 2016 par Loïc
Correction des symboles
*/
#include "correctionSymboles.h"

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

ListeSymboles applySymbolsCorrections(FILE* oldFile, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader, SectionsHeadersList oldSHList, SectionsHeadersList newSHList, ListeSymboles oldSymbolsTable, int silent) {
	int i, j, d;
	char* originalName;
	char* symbolName;
	unsigned char info, bind;
	ListeSymboles newSymbolsTable;
	
	// Allocation de la nouvelle table des symboles, de la même longueur de l'ancienne
	newSymbolsTable.symboles = (Elf32_Sym*) malloc(sizeof(Elf32_Sym)*oldSymbolsTable.nbSymboles);
	if(newSymbolsTable.symboles == NULL)
	{
		printf("Erreur d'allocation\n");
		exit(1);
	}

	if (!silent)
	{
		printf("New symbol table '.symtab' contains %d entries:\n", oldSymbolsTable.nbSymboles);
		printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
	}

	// Copie et correction de oldSymbolsTable vers newSymbolsTable
	i = index_Shdr(".symtab", oldElfHeader, oldSHList);
	newSymbolsTable.names = fetchSymbolNames(oldFile, oldSHList, i);
	d = 0;
	for(j=0; j<oldSymbolsTable.nbSymboles; j++)
	{
		newSymbolsTable.symboles[j-d].st_name = oldSymbolsTable.symboles[j].st_name;
		newSymbolsTable.symboles[j-d].st_size = oldSymbolsTable.symboles[j].st_size;
		newSymbolsTable.symboles[j-d].st_info = oldSymbolsTable.symboles[j].st_info;
		newSymbolsTable.symboles[j-d].st_other = oldSymbolsTable.symboles[j].st_other;

		// Recherche du nouvel id de la section du symbole courant
		if (oldSymbolsTable.symboles[j].st_shndx==0) {
			newSymbolsTable.symboles[j-d].st_shndx = 0;
		}
		else {
			originalName = getSectionNameBis(oldSHList.names, oldSHList.headers[oldSymbolsTable.symboles[j].st_shndx]); 
			newSymbolsTable.symboles[j-d].st_shndx = index_Shdr(originalName, newElfHeader, newSHList);
			free(originalName);
		}
		if(newSymbolsTable.symboles[j-d].st_shndx==65535) {
			d++;
			continue;
		}
		// Nouvelle valeur du symbole = Ancienne valeur + Adresse de la section parente
		newSymbolsTable.symboles[j-d].st_value = oldSymbolsTable.symboles[j].st_value + newSHList.headers[i].sh_addr;

		// Affichage si necessaire
		if (!silent)
		{
			info = 15 & newSymbolsTable.symboles[j-d].st_info;
			bind = 15<<4 & newSymbolsTable.symboles[j-d].st_info;
			symbolName = getSymbolNameBis(newSymbolsTable.names, newSymbolsTable.symboles[j-d]);

			if(newSymbolsTable.symboles[j-d].st_shndx == 0)
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  UND %s\n", j, newSymbolsTable.symboles[j-d].st_value, newSymbolsTable.symboles[j-d].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(newSymbolsTable.symboles[j-d].st_other), symbolName);
			}
			else
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  %3d %s\n", j, newSymbolsTable.symboles[j-d].st_value, newSymbolsTable.symboles[j-d].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(newSymbolsTable.symboles[j-d].st_other), newSymbolsTable.symboles[j-d].st_shndx, symbolName);
			}
			free(symbolName);
		}
	}

	newSymbolsTable.nbSymboles = j-d;
	return newSymbolsTable;
}

void writeSymbolsToFile(FILE* file, Elf32_Ehdr elfHeader, SectionsHeadersList shList, ListeSymboles symbolsTable) {
	int i;
	uint32_t writingOffset;

	printf("MARQUE 1\n");
	// Recherche de la table des symboles dans le nouveau fichier pour récupérer son offset
	i = index_Shdr(".symtab", elfHeader, shList);
	writingOffset = shList.headers[i].sh_offset;

	printf("MARQUE 2\n");

	// Réecriture de la table des symboles dans le nouveau fichier.
	fseek(file, writingOffset, 0);
	for(i = 0; i<symbolsTable.nbSymboles; i++)
	{
		printf("MARQUE 3a:%d\n", i);
		fwrite32(file, elfHeader.e_ident[EI_DATA], symbolsTable.symboles[i].st_name);
		fwrite32(file, elfHeader.e_ident[EI_DATA], symbolsTable.symboles[i].st_value);
		fwrite32(file, elfHeader.e_ident[EI_DATA], symbolsTable.symboles[i].st_size);
		fwrite8(file, elfHeader.e_ident[EI_DATA], symbolsTable.symboles[i].st_info);
		fwrite8(file, elfHeader.e_ident[EI_DATA], symbolsTable.symboles[i].st_other);
		fwrite16(file, elfHeader.e_ident[EI_DATA], symbolsTable.symboles[i].st_shndx);
		printf("MARQUE 3b:%d\n", i);
	}

	printf("MARQUE 4\n");
}