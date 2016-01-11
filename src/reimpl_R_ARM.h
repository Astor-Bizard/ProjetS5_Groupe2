/*
Creer le 6 janvier 2016 par Thai Binh
Réimplantation de type R_ARM
*/
#ifndef _REIMPLRARMH
#define _REIMPLRARMH

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
Fonction principale qui fera la reimplantation de tout les type R_ARM_ABS(8-16-32) et R_ARM_CALL et R_ARM_JUMP24
*/
char* reimplantation_R_ARM(char *chaineSection, Str_Reloc tableReloc, ListeSymboles listeDesSymboles);

#endif