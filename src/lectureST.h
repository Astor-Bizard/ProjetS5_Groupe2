/*
Creer le 5 janvier 2016 par Thai Binh
Lecture de la table des symboles
*/
#ifndef _LECTURESTH_
#define _LECTURESTH_

typedef struct {
	Elf32_Sym* symboles;
	char* names;
	int nbSymboles;
} ListeSymboles;

/*
* Fonction qui retourne un tableau de symboles
* @param f Le fichier ouvert en lecture
* @param elfHeader Le header du fichier en lecture
* @param shList La liste des en-têtes de sections du fichier en lecture
* @param silent Si silent est à 1, aucun affichage ne sera produit en dehors des erreurs
* @return Une ListeSymboles contenant le tableau des symboles et le nombre de symboles, ainsi que leurs noms
*/
ListeSymboles lectureSymbolTab(FILE *f, Elf32_Ehdr elfHeader, SectionsHeadersList shList, int silent);

/*
Fonction qui retourne la chaine de caractère qui correspond au type du symbole donné en parametre
* @param info Octet définissant le type du symbole 
* @return Chaine de caractère qui correspond au type du symbole donné en parametre
*/
char* typeSymbole(unsigned char info);

/*
Fonction qui retourne la chaine de caractère qui correspond au "bind" du symbole donné en parametre
* @param bind Octet définissant la portée du symbole 
* @return Chaine de caractère qui correspond au "bind" du symbole donné en parametre
*/
char* bindSymbole(unsigned char bind);

/*
* Fonction qui retourne la chaine de caractère qui correspond a la vision du symbole donné en parametre
* @param vis Visibilité du symbole
* @return Chaine de caractère qui correspond a la vision du symbole donné en parametre
*/
char* visionSymbole(unsigned char vis);

/*
* Récupère la table des nom des symboles(un '.strtab') sous la forme d'une chaine. 
* @param f Le fichier dans lequel se trouve les noms de symboles, ouvert en lecture
* @param shList Liste des en-têtes de sections
* @param symbolTabIndex Index de la table des symboles dans shTable
* @return Une chaine de caractères composée de plusieurs sous-chaines. Copie conforme de la section '.strtab' liée à '.symtab'
*/
char* fetchSymbolNames(FILE* f, SectionsHeadersList shList, int symbolTabIndex);

/*
* Récupère le nom d'un symbole en se basant sur le champ st_name de son en-tête.
* @param symbols La chaine de caractères contenant les nom de symboles, récupérée au préalable avec 'fetchSymbolNames(...)'
* @param symbolIndex L'index de depart de la sous-chaine du nom à récuperer. (correspond normalement à un champ st_name)
* @return La chaine de caractère correspondant au nom pointé par nameIndex
*/
char* getSymbolName(char* symbols, uint32_t symbolIndex);

/*
* Récupère le nom du symbole fourni.
* @param symbols La chaine de caractères contenant les nom de sections, récupérée au préalable avec 'fetchSymbolNames(...)'
* @param symbol Le symbole dont on cherche le nom
* @return La chaine de caractère correspondant au nom du symbole fourni
*/
char* getSymbolNameBis(char* symbols, Elf32_Sym symbol);

#endif