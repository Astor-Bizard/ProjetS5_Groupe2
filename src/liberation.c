/*Cree le 8/01/2016 par Jonathan
Libere les differente structure du programme
*/

#include "liberation.h"

void free_Elf32_Shdr(Elf32_Shdr *section_headers)
{
	free(section_headers);
}

void free_ListeSymboles(ListeSymboles sym_tab)
{
	free(sym_tab.symboles);
}

void free_Str_Reloc(Str_Reloc str_reloc)
{
	free(str_reloc.Rel);
	free(str_reloc.Sec_Rel);
}

void free_Tab_Donnees(Tab_Donnees tab_donnees)
{
	free(tab_donnees.table_Addr);
	free(tab_donnees.table_Num_Addr);
}
