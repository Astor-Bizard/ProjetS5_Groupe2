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
}

void r_arm_abs16(FILE* oldF, FILE *newF, Elf32_Ehdr oldElfHeader, ListeSymboles symbList, Str_Reloc tableReloc, int index, Elf32_Addr addrDest, unsigned char *sectionARecopier) {
	uint16_t addrSymbole = (255<<8 & tableReloc.Rel[index].r_info)>>8;
	uint16_t addend = (uint16_t) lire_octets(oldElfHeader.e_ident[EI_DATA], oldF, 2);
	uint16_t valeurSymb = (uint16_t) symbList.symboles[addrSymbole].st_value;
	uint16_t res = valeurSymb + addend;
	fwrite16(newF, oldElfHeader.e_ident[EI_DATA], ByteSwap16(res));
}

void r_arm_abs8(FILE* oldF, FILE *newF, Elf32_Ehdr oldElfHeader, ListeSymboles symbList, Str_Reloc tableReloc, int index, Elf32_Addr addrDest, unsigned char *sectionARecopier) {
	uint16_t addrSymbole = (255<<8 & tableReloc.Rel[index].r_info)>>8;
	uint8_t addend = (uint8_t) lire_octets(oldElfHeader.e_ident[EI_DATA], oldF, 1);
	uint8_t valeurSymb = (uint8_t) symbList.symboles[addrSymbole].st_value;
	uint8_t res = valeurSymb + addend;
	fwrite8(newF, oldElfHeader.e_ident[EI_DATA], res);
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
}

void r_arm_call(FILE* oldF, FILE *newF, Elf32_Ehdr oldElfHeader, ListeSymboles symbList, Str_Reloc tableReloc, int index, Elf32_Addr addrDest, unsigned char *sectionARecopier) {
	r_arm_jump24(oldF, newF, oldElfHeader, symbList, tableReloc, index, addrDest, sectionARecopier);
}

void reimplantation_R_ARM(FILE *oldF, FILE *newF, Elf32_Ehdr oldElfHeader, SectionsHeadersList oldSH, SectionsHeadersList newSH, Str_Reloc tableReloc, ListeSymboles symbList)
{
	unsigned char *section = NULL;
	Elf32_Shdr sectionHeader;
	int sectionID = -1;
	int i, newID;
	uint16_t relType;

	for(i=0; i<tableReloc.nb_Rel; i++)
	{
		if(oldSH.headers[tableReloc.Sec_Rel[i]].sh_info != sectionID) 
		{
			sectionID = oldSH.headers[tableReloc.Sec_Rel[i]].sh_info;

			if(section!=NULL)
				free(section);
			sectionHeader = oldSH.headers[sectionID];
			section = recuperer_section_num(oldF, oldSH, sectionID);

			newID = index_Shdr(oldSH.names+sectionHeader.sh_name, newSH);
			fseek(newF, newSH.headers[newID].sh_addr, SEEK_SET);
			fwrite(section, sectionHeader.sh_size, 1, newF);
		}

		fseek(oldF, sectionHeader.sh_offset + tableReloc.Rel[i].r_offset, SEEK_SET);
		fseek(newF, newSH.headers[newID].sh_addr + tableReloc.Rel[i].r_offset, SEEK_SET);

		relType = 0xFF & tableReloc.Rel[i].r_info;
		switch(relType)
		{
			case R_ARM_ABS32:
				r_arm_abs32(oldF, newF, oldElfHeader, symbList, tableReloc, i, newSH.headers[newID].sh_addr, section);
				break;
			case R_ARM_ABS16:
				r_arm_abs16(oldF, newF, oldElfHeader, symbList, tableReloc, i, newSH.headers[newID].sh_addr, section);
				break;
			case R_ARM_ABS8:
				r_arm_abs8(oldF, newF, oldElfHeader, symbList, tableReloc, i, newSH.headers[newID].sh_addr, section);
				break;
			case R_ARM_CALL:
				r_arm_call(oldF, newF, oldElfHeader, symbList, tableReloc, i, newSH.headers[newID].sh_addr, section);
				break;
			case R_ARM_JUMP24:
				r_arm_jump24(oldF, newF, oldElfHeader, symbList, tableReloc, i, newSH.headers[newID].sh_addr, section);
				break;
			default:
				printf("ERREUR: Type de relocation inconnu !\n");
				break;
		}
	}
	if(section!=NULL)
		free(section);
	printf("%d reimplantations effectuées\n", i);
}