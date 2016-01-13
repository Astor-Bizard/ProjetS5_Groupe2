/*
Creer le 6 janvier 2016 par Thai Binh
Réimplantation de type R_ARM
*/

#include "reimpl_R_ARM.h"

#define TEXT 0
#define DATA 1

//void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *f, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader,  SectionsHeadersList tabSH, Str_Reloc tableReloc)

void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *oldF, FILE *newF, Elf32_Ehdr oldElfHeader, SectionsHeadersList tabSH, Str_Reloc tableReloc, ListeSymboles symbList)
{
	unsigned char *section;
	unsigned char *sectionARecopier;
	uint16_t info;
	int i;
	int j = 0;
	int valeurSecRel;
	uint16_t addrSymbole;
	Elf32_Addr addrDest;
	uint32_t tailleSection;
	uint32_t offsetSection = 0;
	
	int machin;

	for(i=0; i<tableDeDonnees.nbSecRel; i++)
	{
		offsetSection = 0;
		addrDest = tableDeDonnees.table_Addr[i];
		valeurSecRel = tableReloc.Sec_Rel[j];
		section = recuperer_section_num(oldF, oldElfHeader, tabSH, valeurSecRel);
		sectionARecopier = recuperer_section_num(oldF, oldElfHeader, tabSH, valeurSecRel-1);
		//ecriture de la section 
		tailleSection = tabSH.headers[valeurSecRel].sh_size;
		fseek(newF, addrDest, SEEK_SET);
		printf("Ecriture de la section : %d a l'addresse %x\n", valeurSecRel, addrDest);
		while(offsetSection<tailleSection)
		{
			fwrite(&sectionARecopier[offsetSection], sizeof(unsigned char),1 ,newF);
			offsetSection = offsetSection + 1;
		}
		while(j<tableReloc.nb_Rel && valeurSecRel==tableReloc.Sec_Rel[j])
		{
			
			info = 255 & tableReloc.Rel[j].r_info;
			addrSymbole = (255<<8 & tableReloc.Rel[j].r_info)>>8;			
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
					
					fseek(oldF, addrDest+tableReloc.Rel[j].r_offset, SEEK_SET);
					fseek(newF, addrDest+tableReloc.Rel[j].r_offset, SEEK_SET);

					machin = (int)lire_octets(oldElfHeader.e_ident[EI_DATA],oldF,4);
					sectionARecopier[tableReloc.Rel[j].r_offset] = symbList.symboles[addrSymbole].st_value + machin;
					//fwrite(&section[tableReloc.Rel[j].r_offset], sizeof(unsigned char), 1, newF);
					printf("Reloc a %x de %x + %x = %x mais j'ai %x\n",addrDest+tableReloc.Rel[j].r_offset, symbList.symboles[addrSymbole].st_value, machin, symbList.symboles[addrSymbole].st_value + machin, sectionARecopier[tableReloc.Rel[j].r_offset]);

					break;					
				case 28:
				case 29:
					// R_ARM_CALL & R_ARM_JUMP24
					// ((S+A) | T) - P
					// P correspond au qqchose dérivé de r_offset du REL (en clair faut juste redécaler sur offset)
					
					fseek(oldF, addrDest+tableReloc.Rel[j].r_offset, SEEK_SET);
					fseek(newF, addrDest+tableReloc.Rel[j].r_offset, SEEK_SET);
					machin = (int)lire_octets(oldElfHeader.e_ident[EI_DATA],oldF,3);
					sectionARecopier[tableReloc.Rel[j].r_offset] = (symbList.symboles[addrSymbole].st_value + (uint16_t) lire_octets(oldElfHeader.e_ident[EI_DATA],oldF,4)) - (addrDest + tableReloc.Rel[j].r_offset);
					
					//fwrite(&section[tableReloc.Rel[j].r_offset], sizeof(unsigned char), 1, newF);
					printf("Reloc a %x de %x + %x - %x = %x mais j'ai %x\n",addrDest+tableReloc.Rel[j].r_offset, symbList.symboles[addrSymbole].st_value, machin, (addrDest + tableReloc.Rel[j].r_offset), symbList.symboles[addrSymbole].st_value + machin - (addrDest + tableReloc.Rel[j].r_offset), sectionARecopier[tableReloc.Rel[j].r_offset]);


					break;
				default:
					printf("Ce n'est pas du type R_ARM_ABS*, ni R_ARM_JUMP24, ou ni R_ARM_CALL\n");
					break;
			}
			j++;
		}
		// Ecriture sur le fichier
		free(section);
	}

}
