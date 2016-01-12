/*
Crée le 5 janvier 2016 par Loïc
Lecture de la table des sections
*/
#ifndef _LECTURESH
#define _LECTURESH

#define SHT_ARM_DEBUGOVERLAY 0x70000004
#define SHT_ARM_OVERLAYSECTION 0x70000005
#define FLAG_STRING_LENGTH 4

typedef struct {
	Elf32_Shdr* headers;
	char* names;
	int size;
} SectionsHeadersList;

/*
* Fait le lien entre l'identifiant numérique d'un type de section et son nom.
* @param sh_type Un entier, généralement issu du champ du même nom d'un en-tête de section
* @return Le nom du type
*/
char* typeNameFromValue(uint32_t sh_type);

/*
* Récupère la table des nom des sections('.shstrtab') sous la forme d'une chaine. 
* @param f Le fichier dans lequel se trouve les noms de sections, ouvert en lecture
* @param elfHeader Le header du fichier en lecture
* @param shList La liste des en-têtes de sections du fichier en lecture
* @return Une chaine de caractères composée de plusieurs sous-chaines. Copie conforme de la section '.shstrtab'
*/
char* fetchSectionNames(FILE* f, Elf32_Ehdr elfHeader, SectionsHeadersList shList);

/*
* Récupère le nom d'une section en se basant sur le champ sh_name de son en-tête.
* @param names La chaine de caractères contenant les nom de sections, récupérée au préalable avec 'fetchSectionNames(...)'
* @param sh_name L'index de depart de la sous-chaine du nom à récuperer. (correspond normalement au champ sh_name d'un en-tête de section)
* @return La chaine de caractère correspondant au nom pointé par sh_name
*/
char* getSectionName(char* names, uint32_t sh_name);

/*
* Récupère le nom d'une section en se basant sur son en-tête.
* @param names La chaine de caractères contenant les nom de sections, récupérée au préalable avec 'fetchSectionNames(...)'
* @param sectionHeader L'entête de la section dont on cherche le nom
* @return La chaine de caractère correspondant au nom de la section fournie
*/
char* getSectionNameBis(char* names, Elf32_Shdr sectionHeader);

/*
* Traduit un ensenmble de fanions d'en-tête de section en une chaine de caractère contenant leurs initiales
* @param flags L'ensemble de fanions à traduire
* @return Une chaine de caractères contenant les initiales des fanions présents dans l'ensemble fourni
*/
char* sectionFlagsTranslation(uint32_t flags);

/*
* Affiche la liste des en-têtes de section.
* @param elfHeader Le header du fichier
* @param shList La liste des en-têtes de section
*/
void displaySectionsHeaders(Elf32_Ehdr elfHeader, SectionsHeadersList shList);

/*
* Récupère la liste des en-têtes de section.
* @param f Le fichier ouvert en lecture
* @param elfHeader Le header du fichier en lecture
* @param silent Si silent est à 1, aucun affichage ne sera produit en dehors des erreurs
* @return Une liste contenant tout les en-têtes de section du fichier f, dans le même ordre que ce dernier, ainsi que leurs noms
*/
SectionsHeadersList readSectionsHeadersFromFile(FILE *f, Elf32_Ehdr elfHeader, int silent);

#endif
