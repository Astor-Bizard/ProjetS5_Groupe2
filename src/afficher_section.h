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

// Affiche le contenu d'une section désignée par nom ou numéro. Renvoie un pointeur sur ce contenu si renvoi non nul (la libération est à la charge de l'utilisateur), NULL si la section n'existe pas ou si renvoi=0.
unsigned char *afficher_section(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH, int renvoi);

// Renvoie un pointeur sur le contenu de la section numero num_sh, NULL si la section n'existe pas. La libération est à la charge de l'utilisateur.
unsigned char *recuperer_section_num(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH, int num_sh);

#endif
