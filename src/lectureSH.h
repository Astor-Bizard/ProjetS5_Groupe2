/*
Crée le 5 janvier 2016 par Loïc
Lecture de la table des sections
*/
#ifndef _LECTURESH
#define _LECTURESH

#define SHT_ARM_DEBUGOVERLAY 0x70000004
#define SHT_ARM_OVERLAYSECTION 0x70000005

/*
Associe une chaine de type à l'identifiant numérique fourni. 
*/
char* sectionTypeString(uint32_t sh_type);

/*
Récupère la table des nom des sections sous la forme d'une chaine. 
Le fichier doit être ouvert en lecture, elfHeader et sectionHeaders doivent être définis.
*/
char* fetchSectionNames(FILE* f, Elf32_Ehdr elfHeader, Elf32_Shdr* sectionsHeaders);

/*
Récupère le nom de section commençant à l'index nameIndex
*/
char* getSectionName(char* names, uint32_t nameIndex);

/*
Récupère le nom de la section corresponant au header sectionHeader
*/
char* getSectionNameBis(char* names, Elf32_Shdr sectionHeader);

/*
Récupère la liste des en-têtes de section.
Si silent est à 1, aucun affichage ne sera produit en dehors des erreurs.
Le fichier doit être ouvert en lecture, elfHeader doit être défini.
*/
Elf32_Shdr* lectureSectionHeader(FILE *f, Elf32_Ehdr elfHeader, int silent);

#endif
