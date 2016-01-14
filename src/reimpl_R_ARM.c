/*
Creer le 6 janvier 2016 par Thai Binh
Réimplantation de type R_ARM
*/

#include "reimpl_R_ARM.h"

#define TEXT 0
#define DATA 1

//void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *f, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader,  SectionsHeadersList tabSH, Str_Reloc tableReloc)

void r_arm_abs32(FILE* oldF, FILE *newF, Elf32_Ehdr oldElfHeader, ListeSymboles symbList, Str_Reloc tableReloc, int index, Elf32_Addr addrDest, unsigned char *sectionARecopier) {
	// (S + A) | T , addresse du symbole + relocation, T = 1 si symbole est du type STT_FUNC sinon 0
	// S = valeur du symbole 
	// A = addend de la relocalisation
	// T = 0
	uint16_t addrSymbole = (255<<8 & tableReloc.Rel[index].r_info)>>8;
	uint32_t addend = (uint32_t) lire_octets(oldElfHeader.e_ident[EI_DATA], oldF, 4);
	uint32_t valeurSymb = (uint32_t) symbList.symboles[addrSymbole].st_value;
	uint32_t res = valeurSymb + addend;
	fwrite32(newF, oldElfHeader.e_ident[EI_DATA], ByteSwap32(res));
	printf("addresse Symbole : %8x\n", addrSymbole);
	printf("Reloc a %8x de %8x + %8x = %8x\n", addrDest+tableReloc.Rel[index].r_offset, valeurSymb, addend, valeurSymb + addend);
}

void r_arm_abs16(FILE* oldF, FILE *newF, Elf32_Ehdr oldElfHeader, ListeSymboles symbList, Str_Reloc tableReloc, int index, Elf32_Addr addrDest, unsigned char *sectionARecopier) {
	uint16_t addrSymbole = (255<<8 & tableReloc.Rel[index].r_info)>>8;
	uint16_t addend = (uint16_t) lire_octets(oldElfHeader.e_ident[EI_DATA], oldF, 2);
	uint16_t valeurSymb = (uint16_t) symbList.symboles[addrSymbole].st_value;
	uint16_t res = valeurSymb + addend;
	fwrite16(newF, oldElfHeader.e_ident[EI_DATA], ByteSwap16(res));
	printf("addresse Symbole : %8x\n", addrSymbole);
	printf("Reloc a %8x de %8x + %8x = %8x\n", addrDest+tableReloc.Rel[index].r_offset, valeurSymb, addend, valeurSymb + addend);
}

void r_arm_abs8(FILE* oldF, FILE *newF, Elf32_Ehdr oldElfHeader, ListeSymboles symbList, Str_Reloc tableReloc, int index, Elf32_Addr addrDest, unsigned char *sectionARecopier) {
	uint16_t addrSymbole = (255<<8 & tableReloc.Rel[index].r_info)>>8;
	uint8_t addend = (uint8_t) lire_octets(oldElfHeader.e_ident[EI_DATA], oldF, 1);
	uint8_t valeurSymb = (uint8_t) symbList.symboles[addrSymbole].st_value;
	uint8_t res = valeurSymb + addend;
	fwrite8(newF, oldElfHeader.e_ident[EI_DATA], res);
	printf("addresse Symbole : %8x\n", addrSymbole);
	printf("Reloc a %8x de %8x + %8x = %8x\n", addrDest+tableReloc.Rel[index].r_offset, valeurSymb, addend, valeurSymb + addend);
}

void r_arm_jump24(FILE* oldF, FILE *newF, Elf32_Ehdr oldElfHeader, ListeSymboles symbList, Str_Reloc tableReloc, int index, Elf32_Addr addrDest, unsigned char *sectionARecopier) {
	// ((S+A) | T) - P
	// P correspond au qqchose dérivé de r_offset du REL (en clair faut juste redécaler sur offset)
	uint16_t addrSymbole = (255<<8 & tableReloc.Rel[index].r_info)>>8;
	uint32_t addend = (uint32_t) lire_octets(oldElfHeader.e_ident[EI_DATA], oldF, 4);
	uint32_t keep = addend & 0xFF000000;
	addend = (addend & 0x00FFFFFF) << 2;
	uint32_t valeurSymb = (uint32_t) symbList.symboles[addrSymbole].st_value;
	uint32_t p = (uint32_t) addrDest + (uint32_t) tableReloc.Rel[index].r_offset; 
	uint32_t res = ((((((valeurSymb + addend) - p) & 0x0FFFFFFE) >> 2) & 0x00FFFFFF) | keep);
	fwrite32(newF, oldElfHeader.e_ident[EI_DATA], ByteSwap32(res));
	printf("addresse Symbole : %8x\n", addrSymbole);
	printf("Reloc a %8x de %8x + %8x - %8x = %8x\n", addrDest+tableReloc.Rel[index].r_offset, valeurSymb, addend, p, ((((((valeurSymb + addend) - p) & 0x03FFFFFE) >> 2) & 0x00FFFFFF) | keep));
}

void r_arm_call(FILE* oldF, FILE *newF, Elf32_Ehdr oldElfHeader, ListeSymboles symbList, Str_Reloc tableReloc, int index, Elf32_Addr addrDest, unsigned char *sectionARecopier) {
	r_arm_jump24(oldF, newF, oldElfHeader, symbList, tableReloc, index, addrDest, sectionARecopier);
}

void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *oldF, FILE *newF, Elf32_Ehdr oldElfHeader, SectionsHeadersList tabSH, Str_Reloc tableReloc, ListeSymboles symbList)
{
	unsigned char *sectionARecopier;
	uint16_t info;
	int i;
	int j = 0;
	int valeurSecRel;
	Elf32_Addr addrDest;
	uint32_t tailleSection;
	uint32_t offsetSection = 0;


	for(i=0; i<symbList.nbSymboles; i++)
	{
		printf("Pour i = %d -> %8x\n", i, symbList.symboles[i].st_value);
	}	

	for(i=0; i<tableDeDonnees.nbSecRel; i++)
	{
		offsetSection = 0;
		addrDest = tableDeDonnees.table_Addr[i];
		valeurSecRel = tableReloc.Sec_Rel[j];
		sectionARecopier = recuperer_section_num(oldF, tabSH, valeurSecRel-1);
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

			printf("Deplacement vers %x + %x = %x\n", tabSH.headers[valeurSecRel-1].sh_offset, tableReloc.Rel[j].r_offset, tabSH.headers[valeurSecRel-1].sh_offset+tableReloc.Rel[j].r_offset);
			fseek(oldF, tabSH.headers[valeurSecRel-1].sh_offset + tableReloc.Rel[j].r_offset, SEEK_SET);
			fseek(newF, addrDest + tableReloc.Rel[j].r_offset, SEEK_SET);

			switch(info)
			{
				case R_ARM_ABS32:
					r_arm_abs32(oldF, newF, oldElfHeader, symbList, tableReloc, j, addrDest, sectionARecopier);
					break;		
				case R_ARM_ABS16:
					r_arm_abs16(oldF, newF, oldElfHeader, symbList, tableReloc, j, addrDest, sectionARecopier);
					break;		
				case R_ARM_ABS8:
					r_arm_abs8(oldF, newF, oldElfHeader, symbList, tableReloc, j, addrDest, sectionARecopier);
					break;					
				case R_ARM_CALL:
					r_arm_call(oldF, newF, oldElfHeader, symbList, tableReloc, j, addrDest, sectionARecopier);
					break;
				case R_ARM_JUMP24:
					r_arm_jump24(oldF, newF, oldElfHeader, symbList, tableReloc, j, addrDest, sectionARecopier);
					break;
				default:
					printf("Ce n'est pas du type R_ARM_ABS*, ni R_ARM_JUMP24, ou ni R_ARM_CALL\n");
					break;
			}
			j++;
		}
		// Ecriture sur le fichier
		free(sectionARecopier);
	}
}