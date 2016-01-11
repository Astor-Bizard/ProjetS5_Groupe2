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

ListeSymboles corrigerSymboles(FILE* oldFile, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader, Elf32_Shdr* originalSH, Elf32_Shdr* newSH, ListeSymboles oldST, int silent) {
	ListeSymboles newST;
	int i, j;
	char* originalName;
	char* sectionNames = fetchSectionNames(oldFile, oldElfHeader, originalSH);
	char* symbolNames;
	char* newName;
	char* nomSection;
	unsigned char info;
	unsigned char bind;
	
	// Allocation de la nouvelle table des symboles, de la même longueur de l'ancienne
	newST.symboles = (Elf32_Sym*) malloc(sizeof(Elf32_Sym)*oldST.nbSymboles);
	if(newST.symboles == NULL)
	{
		printf("Erreur d'allocation\n");
		newST.symboles = NULL;
		newST.nbSymboles = 0;
		return newST;
	}

	// Récupération des noms de symboles si on compte les afficher
	if (!silent)
	{
		printf("New symbol table '.symtab' contains %d entries:\n", oldST.nbSymboles);
		printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

		i = 0;
		nomSection = getSectionNameBis(sectionNames, newSH[i]);
		while(strcmp(nomSection, ".symtab"))
		{
			free(nomSection);
			i++;
			nomSection = getSectionNameBis(sectionNames, newSH[i]);
		}
		free(nomSection);
		symbolNames = fetchSymbolNames(oldFile, newSH, i);
	}

	// Copie et correction de oldST vers newST
	for(j=0; j<oldST.nbSymboles; j++)
	{
		newST.symboles[j].st_name = oldST.symboles[j].st_name;
		newST.symboles[j].st_size = oldST.symboles[j].st_size;
		newST.symboles[j].st_info = oldST.symboles[j].st_info;
		newST.symboles[j].st_other = oldST.symboles[j].st_other;

		// Recherche du nouvel id de la section du symbole courant
		originalName = getSectionNameBis(sectionNames, originalSH[oldST.symboles[j].st_shndx]); // Nom de la section affiliée au symbole courant dans l'ancien fichier
		i = 0;
		newName = getSectionNameBis(sectionNames, newSH[i]);
		while (strcmp(originalName, newName)!=0 && i<newElfHeader.e_shnum) // Recherche de l'id de la section du même nom dans le nouveau fichier
		{
			free(newName);
			i++;
			newName = getSectionNameBis(sectionNames, newSH[i]);
		}
		free(newName);
		if (i==newElfHeader.e_shnum) 
		{
			printf("Erreur: Section introuvable dans le nouveau fichier.\n");
			newST.symboles = NULL;
			newST.nbSymboles = 0;
			return newST;
		}
		newST.symboles[j].st_shndx = i;

		// Nouvelle valeur du symbole = Ancienne valeur + Adresse de la section parente
		newST.symboles[j].st_value = oldST.symboles[j].st_value + newSH[i].sh_addr;

		// Affichage si necessaire
		if (!silent)
		{
			info = 15 & newST.symboles[j].st_info;
			bind = 15<<4 & newST.symboles[j].st_info;
			char* nomSymbole = getSymbolNameBis(symbolNames, newST.symboles[j]);

			if(newST.symboles[j].st_shndx == 0)
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  UND %s\n", j, newST.symboles[j].st_value, newST.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(newST.symboles[j].st_other), nomSymbole);
			}
			else
			{
				printf("   %3d: %08x %5d %-7s %-6s %-7s  %3d %s\n", j, newST.symboles[j].st_value, newST.symboles[j].st_size, typeSymbole(info), bindSymbole(bind), visionSymbole(newST.symboles[j].st_other), newST.symboles[j].st_shndx, nomSymbole);
			}
			free(nomSymbole);
		}
	}

	newST.nbSymboles = j;

	free(originalName);
	free(sectionNames);
	free(symbolNames);
	return newST;
}

void ecrireNouveauxSymboles(FILE* newFile, Elf32_Ehdr newElfHeader, Elf32_Shdr* newSH, ListeSymboles newST) {
	int i = 0;
	uint32_t writingOffset;
	char* newSectionNames = fetchSectionNames(newFile, newElfHeader, newSH);
	char* nomSection;

	printf("MARQUE 1\n");
	// Recherche de la table des symboles dans le nouveau fichier pour récupérer son offset
	nomSection = getSectionNameBis(newSectionNames, newSH[i]);
	while(strcmp(nomSection, ".symtab"))
	{
		free(nomSection);
		i++;
		nomSection = getSectionNameBis(newSectionNames, newSH[i]);
	}
	free(nomSection);
	writingOffset = newSH[i].sh_offset;

	printf("MARQUE 2\n");

	// Réecriture de la table des symboles dans le nouveau fichier.
	fseek(newFile, writingOffset, 0);
	for(i = 0; i<newST.nbSymboles; i++)
	{
		printf("MARQUE 3a:%d\n", i);
		fwrite32(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_name);
		fwrite32(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_value);
		fwrite32(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_size);
		fwrite8(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_info);
		fwrite8(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_other);
		fwrite16(newFile, newElfHeader.e_ident[EI_DATA], newST.symboles[i].st_shndx);
		printf("MARQUE 3b:%d\n", i);
	}

	printf("MARQUE 4\n");

	free(newSectionNames);
}