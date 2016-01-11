/*Cree le 8/01/2016 par Jonathan
Libere les differente structure du programme
*/

#include "liberation.h"

void free_Elf32_Shdr(Elf32_Shdr *section_headers)
{
	free(section_headers);
}

void free_ListeSymboles(ListeSymboles *sym_tab)
{
	free(sym_tab->symboles);
	free(sym_tab);
}

void free_Str_Reloc(Str_Reloc *str_reloc)
{
	int i;
	for(i=0;i<str_reloc->nb_Rel;i++)
	{
		free(&str_reloc->Rel[i]);
		free(&str_reloc->Sec_Rel[i]);
	}
	for(i=0;i<str_reloc->nb_Rela;i++)
	{
		free(&str_reloc->Rela[i]);
		free(&str_reloc->Sec_Rela[i]);
	}
	free(str_reloc);
}



