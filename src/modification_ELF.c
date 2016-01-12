/* Cree le 8/01/2015 par Jonathan
Programme principal de la phase 2
*/
#include "lecture_headers.h"
#include "afficher_section.h"
#include "lectureST.h"
#include "lectureSH.h"
#include "affichage_relocation.h"
#include "renum_section.h"
#include "correctionSymboles.h"
#include "liberation.h"

#define SILENT 1 

#define TEXT 0
#define DATA 1

int main(int argc, char *argv[])
{
	Elf32_Ehdr Old_elfHeaders;
	SectionsHeadersList Old_section_headers;
	Elf32_Ehdr New_elfHeaders;
	SectionsHeadersList New_section_headers;
	ListeSymboles sym_tab;
	ListeSymboles newST;
	Str_Reloc str_reloc;
	Table_Donnees tab_donnees;

	FILE *f_read, *f_write;
	if(argc != 3)
	{
		printf("Erreur nombre d'argument :\n\tmodification_ELF fichier_Elf_lecture fichier_Elf_ecriture");
	}

	f_read = fopen(argv[1], "r");
	if (f_read == NULL) {
		printf("Fichier introuvable: %s\n", argv[1]);
		return 0;
	}

	f_write = fopen(argv[2], "w");
	if (f_write == NULL) {
		printf("Fichier introuvable: %s\n", argv[2]);
		return 0;
	}
	
	Old_elfHeaders = lecture_Headers(f_read, SILENT);
	
	rewind(f_read);
	Old_section_headers = readSectionsHeadersFromFile(f_read, Old_elfHeaders, SILENT);
	
	rewind(f_read);
	sym_tab = lectureSymbolTab(f_read, Old_elfHeaders, Old_section_headers, SILENT);

	rewind(f_read);
	str_reloc = affichage_relocation(f_read, Old_elfHeaders, Old_section_headers, sym_tab, SILENT);
	
	printf("-----Fin de la lecture de %s-----\n",argv[1]);

	tab_donnees.nbSecRel = 2;
	tab_donnees.table_Addr = malloc(sizeof(Elf32_Addr)*tab_donnees.nbSecRel);
	if (tab_donnees.table_Addr==NULL) {
		printf("\nErreur lors de l'allocation initiale de tab_donnees.table_Addr.\n");
		exit(1);
	}
	tab_donnees.table_Num_Addr = malloc(sizeof(int)*tab_donnees.nbSecRel);
	if (tab_donnees.table_Num_Addr==NULL) {
		printf("\nErreur lors de l'allocation initiale de tab_donnees.table_Num_Addr.\n");
		exit(1);
	}

	tab_donnees.table_Num_Addr[TEXT]=index_Shdr(".text", f_read, Old_elfHeaders, Old_section_headers);
	tab_donnees.table_Num_Addr[DATA]=index_Shdr(".data", f_read, Old_elfHeaders, Old_section_headers);

	//.text = 0x58
	//.data = 0x1000

	tab_donnees.table_Addr[TEXT]=0x58;
	tab_donnees.table_Addr[DATA]=0x1000;

	rewind(f_read);
	New_section_headers = renumerote_section(f_read,f_write, Old_elfHeaders, Old_section_headers, &New_elfHeaders, tab_donnees);
	
	afficher_headers(Old_elfHeaders);
	printf("New ");
	afficher_headers(New_elfHeaders);
	displaySectionsHeaders(f_read, Old_elfHeaders, Old_section_headers);
	displaySectionsHeaders(f_read, New_elfHeaders, New_section_headers);

	rewind(f_read);
	newST = applySymbolsCorrections(f_read, Old_elfHeaders, New_elfHeaders, Old_section_headers, New_section_headers, sym_tab, 0);

	//rewind(f_read);
	//rewind(f_write);
	//writeSymbolsToFile(f_write, New_elfHeaders, New_section_headers, newST);

	printf("-----Fin de l'ecriture dans %s-----\n", argv[2]);

	fclose(f_read);
	fclose(f_write);

	free_Tab_Donnees(tab_donnees);
	free_Str_Reloc(str_reloc);
	free_SectionsHeadersList(Old_section_headers);
	free_SectionsHeadersList(New_section_headers);
	free_ListeSymboles(sym_tab);
	free_ListeSymboles(newST);

	return 0;
}
