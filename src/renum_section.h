/*Creer le 6/01/2015 par Jonathan
Renumerote les section dans un ELF et la table des réimplantations
*/

#ifndef __RENUM_SECTION__
#define __RENUM_SECTION__

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "afficher_section.h"
#include "lectureST.h"
#include "affichage_relocation.h"


/*Struture pour stocker les different valeur passer en parametre
*/

typedef struct 
{
	int nbSecRel;
	Elf32_Addr *table_Addr;
	Elf32_Word *table_Nom_Addr;
}Table_Donnees;


/* Copie une chaine de 32 OCTETS dans une chaine source plus grande à partir de id_dest
Fait pas le con Jimmy
*/
void CopieOctet(unsigned char *dest,Elf32_Word *src, Elf32_Addr id_dest);

/*Compte le nombre de section de type Rel dans la structure section_headers*/

int nbSecRel(Elf32_Ehdr *elfHeaders, Elf32_Shdr *section_headers);

/* renumerote et corrige les section de type Rel ecrit le headers et les sections modifier ou non dans f_write*/
Elf32_Shdr renumerote_section(FILE *f_read, FILE *f_write,Elf32_Ehdr *elfHeaders, Elf32_Shdr *section_headers, ListeSymboles sym_tab,Str_Reloc str_reloc);






#endif