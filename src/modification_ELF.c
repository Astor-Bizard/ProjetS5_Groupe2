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

#define SILENT 1 

#define TEXT 0
#define DATA 1


Elf32_Addr recuper_donnees(char argv[], char nom[42])
{
	int taille,i=0;
	char nb[42];
	Elf32_Addr addr;
	
	
	while(argv[i]!='=')
	{
		nom[i]=argv[i];
		i++;
	}

	i++;
	taille=i;

	nom[i]='\0';

	if(argv[i] != '0' && argv[i+1] != 'x')
	{
		printf("Erreur d'argument, forme : .text=0x58 (par ex)\n");
		exit(1);
	}
	
	while(argv[i]!='\0')
	{
		nb[i-taille] = argv[i];
		i++;
	}
	addr = strtol(nb,NULL,16);
	printf("Nom : %s\n",nom);
	printf("Addr : %06x\n", addr);
	return addr;
}



Table_Donnees remplirDonnees(int argc,char *argv[],
		Elf32_Ehdr Old_elfHeaders,SectionsHeadersList Old_section_headers)
{

	char nom[42];
	int i;
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
		exit(1);
	}

	for(i=0;i<tab_donnees.nbSecRel;i++)
	{
		addr = recuper_donnees(argv[i+3],nom);
		
		tab_donnees.table_Num_Addr[i]=index_Shdr(nom, Old_section_headers); 
		tab_donnees.table_Addr[i]=addr;
		printf("tab_donnees.table_Addr[i] : %06x\n", tab_donnees.table_Addr[i]);

	}
//	tab_donnees.table_Num_Addr[TEXT]=index_Shdr(".text", Old_elfHeaders, Old_section_headers);
//	tab_donnees.table_Num_Addr[DATA]=index_Shdr(".data", Old_elfHeaders, Old_section_headers);

	//.text = 0x58
	//.data = 0x1000

//	tab_donnees.table_Addr[TEXT]=0x58;
//	tab_donnees.table_Addr[DATA]=0x1000;

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
	
	if(argc <= 3)
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

	tab_donnees = remplirDonnees(argc,argv,Old_elfHeaders, Old_section_headers);
	rewind(f_read);
	New_section_headers = renumerote_section(f_read,f_write, Old_elfHeaders, Old_section_headers, &New_elfHeaders, tab_donnees);
	afficher_headers(Old_elfHeaders);
	printf("New ");
	afficher_headers(New_elfHeaders);
	displaySectionsHeaders(Old_elfHeaders, Old_section_headers);
	displaySectionsHeaders(New_elfHeaders, New_section_headers);

	rewind(f_read);
	newST = applySymbolsCorrections(f_read, Old_elfHeaders, New_elfHeaders, Old_section_headers, New_section_headers, sym_tab, 0);

	//rewind(f_read);
	//rewind(f_write);
	//writeSymbolsToFile(f_write, New_elfHeaders, New_section_headers, newST);

	reimplantation_R_ARM(tab_donnees,f_read,f_write,Old_elfHeaders, Old_section_headers, str_reloc, newST);

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
