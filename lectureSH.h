/*
Crée le 5 janvier 2016 par Loïc
Lecture de la table des sections
*/

void sectionTypeString(uint32_t sh_type, char* typeString);

char** getSectionsNames(FILE* f, int sectionHeaderCount, uint32_t tableSize, Elf32_Off tableOffset);

Elf32_Shdr* lectureSectionHeader(FILE *f, long int sectionHeaderOffset, int sectionHeaderSize, int sectionHeaderCount, int sectionHeaderStringTableIndex, int silent);
