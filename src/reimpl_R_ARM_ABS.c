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

char* reimplantation_R_ARM_ABS(char *chaine, Str_Reloc tableReloc, ListeSymboles listeDesSymboles)
{
	// TODO: Faire tout
	int i;
	int incrementeurChaine = 0;
	int incrementeurChaineBis;
	unsigned char info;
	//uint32_t addrSymbole;
	for (i=0; i<tableReloc.nb_Rel; i++)
	{
		
		info = 255 & tableReloc.Rel[i].r_info;
		//addrSymbole = (16777215<<8 & tableReloc.Rel[i].r_info)>>8;

		if( (info == 2) || (info == 5) || (info == 8) )
		{
			// TODO: retirer ces parties de table de relocation qui font rien
			incrementeurChaineBis = incrementeurChaine + 16
			for(incrementeurChaine, incrementeurChaine<incrementeurChaineBis, incrementeurChaine++)
			{
				chaine[incrementeurChaine] = "0";
			}

			// (S + A) | T , addresse du symbole + relocation, T = 1 si symbole est du type STT_FUNC
			// S = valeur du symbole 
			// A = addend de la relocalisation
			// T = 0

			// donc y'a rien a faire, alé aplusse
		}
		else
		{
			incrementeurChaine = incrementeurChaine + 16;
		}
	}
	return chaine;
}
