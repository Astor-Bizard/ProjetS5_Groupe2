/*
Creer le 6 janvier 2016 par Thai Binh
Réimplantation de type R_ARM
*/
#ifndef __REIMPLRARMH__
#define __REIMPLRARMH__

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include "lecture_headers.h"
#include "lectureSH.h"
#include "lectureST.h"
#include "affichage_relocation.h"
#include "renum_section.h"
#include "afficher_section.h"

/*
Fonction principale qui fera la reimplantation de tout les types R_ARM_ABS(8-16-32) et R_ARM_CALL et R_ARM_JUMP24
@param tableDeDonnees la structure qui permet de recuperer les addresses des sections .text et .data
@param f le fichier a modifier ouvert en lecture
@param oldElfHeader le header du fichier format elf avant modification par la phase 2
@param tabSH le tableau des sections
@param tableReloc structure de la table des relocations
*/
//void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *f, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader,  Elf32_Shdr *tabSH, Str_Reloc tableReloc);
void reimplantation_R_ARM(Table_Donnees tableDeDonnees, FILE *oldF, FILE *newF, Elf32_Ehdr oldElfHeader, SectionsHeadersList tabSH, Str_Reloc tableReloc);

#endif