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
	int i, j;
	char* nameSearch;
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
	i = 0;
	nameSearch = getSectionNameBis(oldSHList.names, oldSHList.headers[i]);
	while(strcmp(nameSearch, ".symtab"))
	{
		free(nameSearch);
		i++;
		nameSearch = getSectionNameBis(oldSHList.names, oldSHList.headers[i]);
	}
	free(nameSearch);
	newSymbolsTable.names = fetchSymbolNames(oldFile, oldSHList, i);

	for(j=0; j<oldSymbolsTable.nbSymboles; j++)
	{
		newSymbolsTable.symboles[j].st_name = oldSymbolsTable.symboles[j].st_name;
		newSymbolsTable.symboles[j].st_size = oldSymbolsTable.symboles[j].st_size;
		newSymbolsTable.symboles[j].st_info = oldSymbolsTable.symboles[j].st_info;
		newSymbolsTable.symboles[j].st_other = oldSymbolsTable.symboles[j].st_other;

		// Recherche du nouvel id de la section du symbole courant
		originalName = getSectionNameBis(oldSHList.names, oldSHList.headers[oldSymbolsTable.symboles[j].st_shndx]); 
		newSymbolsTable.symboles[j].st_shndx = index_Shdr(originalName, newFile, newElfHeader, newSHList);
		free(originalName);

		// Nouvelle valeur du symbole = Ancienne valeur + Adresse de la section parente
		newSymbolsTable.symboles[j].st_value = oldSymbolsTable.symboles[j].st_value + newSHList.headers[i].sh_addr;

		// Affichage si necessaire
		if (!silent)
		{
			info = 15 & newSymbolsTable.symboles[j].st_info;
			bind = 15<<4 & newSymbolsTable.symboles[j].st_info;
			symbolName = getSymbolNameBis(newSymbolsTable.names, newSymbolsTable.symboles[j]);

			if(newSymbolsTable.symboles[j].st_shndx == 0)
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  UND %s\n", j, newSymbolsTable.symboles[j].st_value, newSymbolsTable.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(newSymbolsTable.symboles[j].st_other), nomSymbole);
			}
			else
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  %3d %s\n", j, newSymbolsTable.symboles[j].st_value, newSymbolsTable.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(newSymbolsTable.symboles[j].st_other), newSymbolsTable.symboles[j].st_shndx, nomSymbole);
			}
			free(nomSymbole);
		}
	}

	newSymbolsTable.nbSymboles = j;
	return newSymbolsTable;
}

void writeSymbolsToFile(FILE* file, Elf32_Ehdr elfHeader, SectionsHeadersList shList, ListeSymboles symbolsTable) {
	int i;
	uint32_t writingOffset;
	char* sectionName;

	printf("MARQUE 1\n");
	// Recherche de la table des symboles dans le nouveau fichier pour récupérer son offset
	i = index_Shdr(".symtab", file, elfHeader, shList);
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