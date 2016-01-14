/* 
Crée le 14 janvier 2016 par Loïc
Ecriture de la table des sections
*/
#include "ecritureSH.h"

void writeSectionsHeadersToFile(FILE *f, Elf32_Ehdr elfHeader, SectionsHeadersList shList) {
	int i;

	fseek(f, elfHeader.e_shoff, 0);
	for(i=0; i<shList.size; i++) {
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_name);
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_type);
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_flags);
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_addr);
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_offset);
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_size);
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_link);
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_info);
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_addralign);
		fwrite32(f, elfHeader.e_ident[EI_DATA], shList.headers[i].sh_entsize);
	}

	printf("%d en-têtes de sections écrites\n", i);
}