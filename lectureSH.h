/*
Crée le 5 janvier 2016 par Loïc
Lecture de la table des sections
*/
#ifndef _LECTURESH
#define _LECTURESH

#define SHT_ARM_DEBUGOVERLAY 0x70000004
#define SHT_ARM_OVERLAYSECTION 0x70000005

char* sectionTypeString(uint32_t sh_type);

/*char** getSectionsNames(FILE* f, Elf32_Ehdr elfHeader, Elf32_Shdr* sectionsHeaders);*/
char* fetchSectionNames(FILE* f, Elf32_Ehdr elfHeader, Elf32_Shdr* sectionsHeaders);

char* getSectionName(char* names, uint32_t nameIndex);
char* getSectionNameBis(char* names, Elf32_Shdr sectionHeader);

Elf32_Shdr* lectureSectionHeader(FILE *f, Elf32_Ehdr elfHeader, int silent);

#endif