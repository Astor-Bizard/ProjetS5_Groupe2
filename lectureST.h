/*
Creer le 5 janvier 2016 par Thai Binh
Lecture de la table des symboles
*/


/*
fonction qui retourne un tableau de symbole
*/
Elf32_Sym* lectureSymbolTab(FILE *f, int sectionSymbolTabOffset, int sectionSymbolTabSize);

/*
Fonction qui retourne la chaine de caractère qui correspond au type du symbole donné en parametre
*/
char* typeSymbole(unsigned char info)