/*
Creer le 6 janvier 2016 par Thai Binh
Réimplantation de type R_ARM
*/

#include "reimpl_R_ARM.h"

void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *f, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader,  Elf32_Shdr *tabSH, Str_Reloc tableReloc)
{
	// TODO: Faire tout
	unsigned char *partieText = recuperer_section_num(f, oldElfHeader, tabSH, index_Shdr(".text", f, oldElfHeader, tabSH));
	unsigned char *partieData = recuperer_section_num(f, oldElfHeader, tabSH, index_Shdr(".data", f, oldElfHeader, tabSH));
	unsigned char info;
	Elf32_Addr addrText = NULL;
	Elf32_Addr addrData = NULL;
	int numAddrText = NULL;
	int numAddrData = NULL;
	int i;
	uint32_t addrSymbole;

	for(i=0, i<tableDeDonnees.nbSecRel, i++)
	{
		if (i==0)
		{
			addrText = tableDeDonnees.table_Addr[i];
			numAddrText = tableDeDonnees.table_Num_Addr[i];
		}
		if (i==1)
		{
			addrData = tableDeDonnees.table_Addr[i];
			numAddrData = tableDeDonnees.table_Num_Addr[i];
		}
	}
	
	for (i=0; i<tableReloc.nb_Rel; i++)
	{
		info = 255 & tableReloc.Rel[i].r_info;
		addrSymbole = (16777215<<8 & tableReloc.Rel[i].r_info)>>8;
		switch(info)
		{
			case 2:
			case 5:
			case 8:
				// R_ARM_ABS*
				// (S + A) | T , addresse du symbole + relocation, T = 1 si symbole est du type STT_FUNC sinon 0
				// S = valeur du symbole 
				// A = addend de la relocalisation
				// T = 0
				if(tableReloc.Sec_Rel[i] == index_Shdr(".rel.text", f, oldElfHeader, tabSH))
				{
					partieText[tableReloc.Rel[i].r_offset] = addrSymbole;
				}
				if(tableReloc.Sec_Rel[i] == index_Shdr(".rel.data", f, oldElfHeader, tabSH))
				{
					partieData[tableReloc.Rel[i].r_offset] = addrSymbole;
				}
				break;
			case 28:
			case 29:
				// R_ARM_CALL & R_ARM_JUMP24
				// ((S+A) | T) - P
				// P correspond au qqchose dérivé de r_offset du REL (en clair faut juste redécaler sur offset)
				if(tableReloc.Sec_Rel[i] == index_Shdr(".rel.text", f, oldElfHeader, tabSH))
				{
					partieText[tableReloc.Rel[i].r_offset] = addrSymbole - tableReloc.Rel[i].r_offset;
				}
				if(tableReloc.Sec_Rel[i] == index_Shdr(".rel.data", f, oldElfHeader, tabSH))
				{
					partieData[tableReloc.Rel[i].r_offset] = addrSymbole - tableReloc.Rel[i].r_offset;
				}
				break;
			default:
				printf("ERREUR DE LECTURE DE REL ! ! ! ! ! ! ! ! ! ! ! !\n");
				break;
		}
	}

}
