/* Cree le 8/01/2015 par Jonathan
Programme principal de la phase 2
*/




#include "lecture_headers.h"
#include "afficher_section.h"
#include "lectureST.h"
#include "lectureSH.h"
#include "affichage_relocation.h"
#include "renum_section.h"


#define SILENT 1 



int main(int argc, char *argv[])
{
	Elf32_Ehdr Old_elfHeaders;
	Elf32_Shdr *Old_section_headers;
	Elf32_Ehdr New_elfHeaders;
	Elf32_Shdr New_section_headers;
	ListeSymboles sym_tab;
	Str_Reloc str_reloc;
	
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
	Old_section_headers = lectureSectionHeader(f_read, Old_elfHeaders, SILENT);
	

	rewind(f_read);
	sym_tab = lectureSymbolTab(f_read, Old_elfHeaders, Old_section_headers, SILENT);

	rewind(f_read);
	str_reloc = affichage_relocation(f_read, Old_elfHeaders, Old_section_headers, sym_tab, SILENT);
	/*
	for(i=0;i<Old_elfHeaders.e_shnum;i++)
	{
		printf("Nom : %06x\n",Old_section_headers[i].sh_offset);
	}
	*/
	printf("-----Fin de la lecture de %s-----\n",argv[1]);
	rewind(f_read);
	renumerote_section(f_read,f_write,Old_elfHeaders, Old_section_headers,
		sym_tab,str_reloc, &New_elfHeaders,&New_section_headers);



	



	printf("-----Fin de l'ecriture dans %s-----\n",argv[2]);
	return 0;
}