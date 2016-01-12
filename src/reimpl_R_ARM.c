/*
Creer le 6 janvier 2016 par Thai Binh
Réimplantation de type R_ARM
*/

#include "reimpl_R_ARM.h"

#define TEXT 0
#define DATA 1

//void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *f, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader,  SectionsHeadersList tabSH, Str_Reloc tableReloc)

void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *f, Elf32_Ehdr oldElfHeader, SectionsHeadersList tabSH, Str_Reloc tableReloc)
{
	unsigned char *section;
	unsigned char info;
	int i;
	int j = 0;
	int valeurSecRel;
	unsigned char addrSymbole;
	Elf32_Addr addrDest;
	printf("DEBUT\n");
	for(i=0; i<tableDeDonnees.nbSecRel; i++)
	{
		printf("Section %d nb rel : %d section dans rel : %d \n",i, tableReloc.nb_Rel, tableReloc.Sec_Rel[j]);
		valeurSecRel = Sec_Rel[j];
		while(j<tableReloc.nb_Rel && valeurSecRel==tableReloc.Sec_Rel[j])
		{
			printf("Reloc %d\n",j);
			info = 255 & tableReloc.Rel[j].r_info;
			addrSymbole = (255<<8 & tableReloc.Rel[j].r_info)>>8;
			section = recuperer_section_num(f, oldElfHeader, tabSH, i);
			addrDest = tableDeDonnees.table_Addr[i];
			section = malloc(sizeof(unsigned char)*(tabSH.headers[i].sh_size/16));
			switch(info)
			{
				case 2:
				case 5:
				case 8:
					// R_ARM_ABS*
					// (S + A) | T , addresse du symbole + relocation, T = 1 si symbole est du type STT_FUNC sinon 0
					// S = valeur du symbole 
					// A = addend de la relocalisation
					// T = 
					printf("2-5-8\n");
					fseek(f, addrDest+tableReloc.Rel[j].r_offset, SEEK_SET);
					section[tableReloc.Rel[j].r_offset] = addrSymbole + (uint16_t)lire_octets(oldElfHeader.e_ident[EI_DATA],f,2);
					j++;
					break;					
				case 28:
				case 29:
					// R_ARM_CALL & R_ARM_JUMP24
					// ((S+A) | T) - P
					// P correspond au qqchose dérivé de r_offset du REL (en clair faut juste redécaler sur offset)
					printf("28-29\n");
					fseek(f, addrDest+tableReloc.Rel[j].r_offset, SEEK_SET);
					section[tableReloc.Rel[j].r_offset] = (addrSymbole + (uint16_t) lire_octets(oldElfHeader.e_ident[EI_DATA],f,2)) - tableReloc.Rel[j].r_offset;
					j++;
					break;
				default:
					printf("Ce n'est pas du type R_ARM_ABS*, ni R_ARM_JUMP24, ou ni R_ARM_CALL\n");
					j++;
					break;
			}
		}
		// Ecriture sur le fichier
		fseek(f, addrDest, SEEK_SET);
		fwrite(section, sizeof(unsigned char)*(tabSH.headers[i].sh_size/16), 1, f);
		free(section);
	}

}
