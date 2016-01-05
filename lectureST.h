/*
Creer le 5 janvier 2016 par Thai Binh
Lecture de la table des symboles
*/


/*
fonction qui retourne un tableau de symboles
si silent est a 1, la fonction n'affichera pas le résultat sur la sortie standard
*/
Elf32_Sym* lectureSymbolTab(FILE *f, int sectionSymbolTabOffset, int sectionSymbolTabSize, int silent);

/*
Fonction qui retourne la chaine de caractère qui correspond au type du symbole donné en parametre
*/
char* typeSymbole(unsigned char info);

/*
Fonction qui retourne la chaine de caractère qui correspond au "bind" du symbole donné en parametre
*/
char* bindSymbole(unsigned char bind)