/*Cree le 8/01/2016 par Jonathan
Libere les differente structure du programme
*/

#include "liberation.h"

void free_SectionsHeadersList(SectionsHeadersList shList)
{
	free(shList.headers);
	free(shList.names);
	shList.size = 0;
}

void free_ListeSymboles(ListeSymboles sym_tab)
{
	free(sym_tab.symboles);
	free(sym_tab.names);
	sym_tab.nbSymboles = 0;
}

void free_Str_Reloc(Str_Reloc str_reloc)
{
	free(str_reloc.Rel);
	free(str_reloc.Sec_Rel);
}

void free_Tab_Donnees(Table_Donnees tab_donnees)
{
	free(tab_donnees.table_Addr);
	free(tab_donnees.table_Num_Addr);
}
