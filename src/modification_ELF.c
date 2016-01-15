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
#include "reimpl_R_ARM.h"
#include "ecritureSH.h"

#define SILENT 1 

#define TEXT 0
#define DATA 1


Elf32_Addr recuperer_donnees(char argv[], char nom[])
{
	int taille,i=0;
	char nb[42];
	Elf32_Addr addr;
	
	
	while(argv[i]!='=')
	{
		nom[i]=argv[i];
		i++;
	}
	nom[i]='\0';

	i++;
	taille=i;


	if(argv[i] != '0' && argv[i+1] != 'x')
	{
		printf("Erreur d'argument. Forme attendue : <section>=0x<addr>\n");
		exit(1);
	}

	i+=2;
	while(argv[i]!='\0')
	{
		nb[i-taille-2] = argv[i];
		i++;
	}
	nb[i-taille-2]='\0';

	addr = strtol(nb,NULL,16);
	return addr;
}



Table_Donnees remplirDonnees(int argc,char *argv[],
		Elf32_Ehdr Old_elfHeaders,SectionsHeadersList Old_section_headers)
{

	int i;
	char nom[42];
	Elf32_Addr addr;
	Table_Donnees tab_donnees;


	tab_donnees.nbSecRel = argc-3;
	tab_donnees.table_Addr = malloc(sizeof(Elf32_Addr)*tab_donnees.nbSecRel);
	if (tab_donnees.table_Addr==NULL) {
		printf("\nErreur lors de l'allocation initiale de tab_donnees.table_Addr.\n");
		exit(1);
	}
	tab_donnees.table_Num_Addr = malloc(sizeof(int)*tab_donnees.nbSecRel);
	if (tab_donnees.table_Num_Addr==NULL) {
		printf("\nErreur lors de l'allocation initiale de tab_donnees.table_Num_Addr.\n");
		free(tab_donnees.table_Addr);
		exit(1);
	}

	for(i=0;i<tab_donnees.nbSecRel;i++)
	{
		addr = recuperer_donnees(argv[i+3],nom);
		
		tab_donnees.table_Num_Addr[i]=index_Shdr(nom, Old_section_headers); 
		tab_donnees.table_Addr[i]=addr;
	}

	return tab_donnees;
}

int main(int argc, char *argv[])
{
	Elf32_Ehdr Old_elfHeaders, New_elfHeaders;;
	SectionsHeadersList Old_section_headers, New_section_headers;
	ListeSymboles sym_tab,newST;
	Str_Reloc str_reloc;
	Table_Donnees tab_donnees;
	FILE *f_read, *f_write;
	
	if(argc < 3)
	{
		printf("Erreur nombre d'argument :\n\t./modification_ELF <fichier_Elf_lecture> <fichier_Elf_ecriture>\n");
		return 1;
	}

	f_read = fopen(argv[1], "r");
	if (f_read == NULL) {
		printf("Fichier introuvable: %s\n", argv[1]);
		return 1;
	}

	f_write = fopen(argv[2], "w");
	if (f_write == NULL) {
		printf("Fichier introuvable: %s\n", argv[2]);
		fclose(f_read);
		return 1;
	}

	Old_elfHeaders = lecture_Headers(f_read, SILENT);
	
	rewind(f_read);
	Old_section_headers = readSectionsHeadersFromFile(f_read, Old_elfHeaders, SILENT);
	
	rewind(f_read);
	sym_tab = lectureSymbolTab(f_read, Old_elfHeaders, Old_section_headers, SILENT);

	rewind(f_read);
	str_reloc = affichage_relocation(f_read, Old_elfHeaders, Old_section_headers, sym_tab, SILENT);
	
	printf("-----Fin de la lecture de %s-----\n",argv[1]);	

	tab_donnees = remplirDonnees(argc,argv,Old_elfHeaders, Old_section_headers);
	rewind(f_read);
	New_section_headers = renumerote_section(f_read, f_write, Old_elfHeaders, Old_section_headers, &New_elfHeaders, tab_donnees);
	printf("New ");
	afficher_headers(New_elfHeaders);
	displaySectionsHeaders(New_elfHeaders, New_section_headers);
	
	rewind(f_read);
	newST = applySymbolsCorrections(f_read, Old_elfHeaders, New_elfHeaders, Old_section_headers, New_section_headers, sym_tab, SILENT);

	ecrire_nouvelles_sections(f_read, f_write, New_section_headers, Old_section_headers); 

	writeSectionsHeadersToFile(f_write, New_elfHeaders, New_section_headers);

	reimplantation_R_ARM(f_read, f_write, Old_elfHeaders, Old_section_headers, New_section_headers, str_reloc, newST);

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
