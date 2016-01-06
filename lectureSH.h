/*
Crée le 5 janvier 2016 par Loïc
Lecture de la table des sections
*/
#ifndef _LECTURESH
#define _LECTURESH

char* sectionTypeString(uint32_t sh_type);

/*char** getSectionsNames(FILE* f, Elf32_Ehdr elfHeader, Elf32_Shdr* sectionsHeaders);*/
char* fetchSectionNames(FILE* f, Elf32_Ehdr elfHeader, Elf32_Shdr* sectionsHeaders);

char* getSectionName(char* names, uint32_t nameIndex);
char* getSectionNameBis(char* names, Elf32_Shdr sectionHeader);

Elf32_Shdr* lectureSectionHeader(FILE *f, Elf32_Ehdr elfHeader, int silent);

#endif