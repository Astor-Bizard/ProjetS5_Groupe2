/*
Creer le 6 janvier 2016 par Thai Binh
Réimplantation de type R_ARM
*/
#ifndef _REIMPLRARMH
#define _REIMPLRARMH

/*
Fonction principale qui fera la reimplantation de tout les type R_ARM_ABS(8-16-32) et R_ARM_CALL et R_ARM_JUMP24
*/
char* reimplantation_R_ARM(char *chaineSection, Str_Reloc tableReloc, ListeSymboles listeDesSymboles);

#endif