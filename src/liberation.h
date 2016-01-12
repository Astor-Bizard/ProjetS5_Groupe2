/*Cree le 8/01/2016 par Jonathan
Libere les differente structure du programme
*/

#ifndef LIBERATION
#define LIBERATION

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "lectureST.h"
#include "lectureSH.h"
#include "affichage_relocation.h"
#include "renum_section.h"

void free_SectionsHeadersList(SectionsHeadersList shList);
void free_ListeSymboles(ListeSymboles sym_tab);
void free_Str_Reloc(Str_Reloc str_reloc);
void free_Tab_Donnees(Table_Donnees tab_donnees);

#endif
