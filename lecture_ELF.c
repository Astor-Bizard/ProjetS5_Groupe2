/* 
Creer le 5/01/2015 par Jonathan
Programme principal de la partie 1

*/

#include "lecture_headers.h"
#include "afficher_section.h"
#include "lectureSH.h"


int main(int argc, char *argv[])
{


	Elf32_Ehdr elfHeaders;

	if(argc != 2)
	{
		printf("Erreur : il faut transmettre en argument un nom de fichier")
		exit(1);
	}

	FILE* f = fopen(argv[1], "r");

	elfHeaders = lecture_Headers(f);

	
	fclose(f);


	return 0;



}
