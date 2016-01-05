/*
Cree le 5 janvier 2016 par Astor
Affichage d'une section specifique
*/
#ifndef __AFF_SECTION__
#define __AFF_SECTION__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

// Retourne le numéro de la section demandée, par son nom ou son numéro, -1 si invalide.
int index_Shdr(char str[], FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH);

// Affiche le contenu d'une section désignée par nom ou numéro. Renvoie ce contenu, NULL si la section n'existe pas.
char *afficher_section(char *nom_f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH);

#endif
