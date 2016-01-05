/*
Creer le 5 janvier 2016 par Thai Binh
Lecture de la table des symboles
*/
#ifndef _LECTURESTH
#define _LECTURESTH

/*
Fonction qui retourne un tableau de symboles
Si silent est a 1, la fonction n'affichera pas le resultat sur la sortie standard
*/
Elf32_Sym* lectureSymbolTab(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr sectionHeader, int silent)

/*
Fonction qui retourne la chaine de caractère qui correspond au type du symbole donné en parametre
*/
char* typeSymbole(unsigned char info);

/*
Fonction qui retourne la chaine de caractère qui correspond au "bind" du symbole donné en parametre
*/
char* bindSymbole(unsigned char bind)

/*
Fonction qui initialise les donnée nessessaires au bon fonctionnement de la lecture
*/
void initSymbolTabUsefullInfo(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr sectionHeader, int *size, int *offset)
#endif