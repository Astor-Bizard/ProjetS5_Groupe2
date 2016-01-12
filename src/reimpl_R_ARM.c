/*
Creer le 6 janvier 2016 par Thai Binh
Réimplantation de type R_ARM
*/

#include "reimpl_R_ARM.h"

#define TEXT 0
#define DATA 1

//void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *f, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader,  Elf32_Shdr *tabSH, Str_Reloc tableReloc)

void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *f, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader,  Elf32_Shdr *tabSH, Str_Reloc tableReloc)
{
	int indexText = index_Shdr(".text", f, oldElfHeader, tabSH);
	int indexData = index_Shdr(".data", f, oldElfHeader, tabSH);
	unsigned char *partieText = recuperer_section_num(f, oldElfHeader, tabSH, indexText);
	unsigned char *partieData = recuperer_section_num(f, oldElfHeader, tabSH, indexData);
	unsigned char info;
	Elf32_Addr addrText = NULL;
	Elf32_Addr addrData = NULL;
	int numAddrText = NULL;
	// TODO:utile ?
	//int numAddrData = NULL;
	int i;
	unsigned char addrSymbole;

	partieText = malloc(sizeof(unsigned char)*tabSH[indexText].sh_size/16);

	if(partieText == NULL)
	{
		printf("ERREUR sur l'allocation de la section .text\n");
		return;
	}
	partieData = malloc(sizeof(unsigned char)*tabSH[indexData].sh_size/16);
	if(partieData == NULL)
	{
		printf("ERREUR sur l'allocation de la section .data\n");
		return;
	}

	for(i=0, i<tableDeDonnees.nbSecRel, i++)
	{
		if (i == TEXT)
		{
			addrText = tableDeDonnees.table_Addr[i];
			// TODO:utile ?
			//numAddrText = tableDeDonnees.table_Num_Addr[i];
		}
		if (i == DATA)
		{
			addrData = tableDeDonnees.table_Addr[i];
			// TODO:utile ?
			//numAddrData = tableDeDonnees.table_Num_Addr[i];
		}
	}

	
	for (i=0; i<tableReloc.nb_Rel; i++)
	{
		info = 255 & tableReloc.Rel[i].r_info;
		addrSymbole = (255<<8 & tableReloc.Rel[i].r_info)>>8;
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
					fseek(f,addrSymbole+tableReloc.Rel[i].r_offset, SEEK_SET);
					partieText[tableReloc.Rel[i].r_offset] = addrSymbole + (uint16_t)lire_octets(oldElfHeader.e_ident[EI_DATA],f,2);
				}
				if(tableReloc.Sec_Rel[i] == index_Shdr(".rel.data", f, oldElfHeader, tabSH))
				{
					fseek(f,addrSymbole+tableReloc.Rel[i].r_offset, SEEK_SET);
					partieData[tableReloc.Rel[i].r_offset] = addrSymbole + (uint16_t) lire_octets(oldElfHeader.e_ident[EI_DATA],f,2);
				}
				break;
			case 28:
			case 29:
				// R_ARM_CALL & R_ARM_JUMP24
				// ((S+A) | T) - P
				// P correspond au qqchose dérivé de r_offset du REL (en clair faut juste redécaler sur offset)
				if(tableReloc.Sec_Rel[i] == index_Shdr(".rel.text", f, oldElfHeader, tabSH))
				{
					fseek(f,addrSymbole+tableReloc.Rel[i].r_offset, SEEK_SET);
					partieText[tableReloc.Rel[i].r_offset] = (addrSymbole + (uint16_t) lire_octets(oldElfHeader.e_ident[EI_DATA],f,2)) - tableReloc.Rel[i].r_offset;
				}
				if(tableReloc.Sec_Rel[i] == index_Shdr(".rel.data", f, oldElfHeader, tabSH))
				{
					fseek(f,addrSymbole+tableReloc.Rel[i].r_offset, SEEK_SET);
					partieData[tableReloc.Rel[i].r_offset] = (addrSymbole + (uint16_t) lire_octets(oldElfHeader.e_ident[EI_DATA],f,2)) - tableReloc.Rel[i].r_offset;
				}
				break;
			default:
				printf("ERREUR DE LECTURE DE REL ! ! ! ! ! ! ! ! ! ! ! !\n");
				break;
		}
	}
	// Ecriture sur le fichier
	fseek(f, addrText, SEEK_SET);
	fwrite(&partieText, sizeof(unsigned char)*4, 1, f);
	fseek(f, addrData, SEEK_SET);
	fwrite(&partieData, sizeof(unsigned char)*4, 1, f);
	free(partieText);
	free(partieData);
}
