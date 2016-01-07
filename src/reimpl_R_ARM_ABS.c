/*
Creer le 6 janvier 2016 par Thai Binh
Réimplantation de type R_ARM_ABS*
*/

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include "lecture_headers.h"
#include "lectureSH.h"
#include "lectureST.h"
#include "affichage_relocation.h"
#include "reimpl_R_ARM_ABS.h"

void reimplantation_R_ARM_ABS(FILE *f, Str_Reloc tableReloc)
{
	// TODO: Faire tout
	int i;
	for (i=0; i<tableReloc.nb_Rel; i++)
	{
		unsigned char info = 8 & tableReloc.Rel[i].r_info;
		if( (tableReloc.Rel[i].r_info == 2) || (tableReloc.Rel[i].r_info == 5) || (tableReloc.Rel[i].r_info == 8) )
		{
			printf("HOLA une realocation a faire ici qq.");
		}
	}
}
