/*
Creer le 4 janvier 2016 par Jonathan

Lecture d'un fichier elf et affichage du Header
*/

#ifndef LECTURE_HEADERS
#define LECTURE_HEADERS

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#define L_ENDIAN 1
#define B_ENDIAN 2


/*
	lit nb_octet du fichier f, en little endian si mode = 1, 
	en big endian si mode = 2
*/
long long int lire_octets(char mode, FILE *f, int nb_octet);

/* Prends en parametre un fichier en formet ELF32 ouvert en mode lecture
et lit toute la partie Headers du fichier. Si silent!=0 aucun affichage produit en dehors des erreurs.
*/
Elf32_Ehdr lecture_Headers(FILE *f, int silent);

void afficher_headers(Elf32_Ehdr headers);

#endif