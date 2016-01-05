/*
Cree le 5 janvier 2016 par Astor
Affichage d'une section specifique
*/
#include <elf.h>

// Retourne le numéro de la section demandée, par son nom ou son numéro, -1 si invalide.
int index_Shdr(char str[], FILE *f, int ShdrCount, int ShdrStrIndex);

// Affiche le contenu d'une section désignée par nom ou numéro. Renvoie ce contenu, NULL si la section n'existe pas.
char *afficher_section(char *nom_f, Elf32_Shdr *tabSH, int ShdrCount, int ShdrStrIndex);
