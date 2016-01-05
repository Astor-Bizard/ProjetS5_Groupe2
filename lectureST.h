/*
fonction qui retourne un tableau de symbole
*/
Elf32_Sym* lectureSymbolTab(FILE *f, int sectionSymbolTabOffset, int sectionSymbolTabSize);

/*
Fonction qui retourne la chaine de caractère qui correspond au type du symbole donné en parametre
*/
char* typeSymbole(unsigned char info)