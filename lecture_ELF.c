/* 
Creer le 5/01/2015 par Jonathan
Programme principal de la partie 1

*/

#include "lecture_headers.h"
#include "afficher_section.h"
#include "lectureST.h"
#include "lectureSH.h"
#include "affichage_relocation.h"


int main(int argc, char *argv[])
{

	Elf32_Ehdr elfHeaders;
	Elf32_Shdr *section_headers;
	FILE* f;

	if(argc != 2)
	{
		printf("Erreur : il faut transmettre en argument un nom de fichier")
		exit(1);
	}

	f = fopen(argv[1], "r");
	elfHeaders = lecture_Headers(f);
	fclose(f);

	f = fopen(argv[1], "r");
	section_headers = lectureSectionHeader(f,elfHeaders,0);
	fclose(f);

	afficher_section(argv[1], section_headers,elfHeaders.e_shnum, elfHeaders.e_shstrndx);

	return 0;



}
