/*
Crée le 5 janvier 2016 par Loïc
Lecture de la table des sections
*/

void sectionTypeString(uint32_t sh_type, char* typeString);

char** getSectionsNames(FILE* f, uint16_t sectionHeaderCount, uint32_t tableSize, Elf32_Off tableOffset);

Elf32_Shdr* lectureSectionHeader(FILE *f, Elf32_Off sectionHeaderOffset, uint16_t sectionHeaderSize, uint16_t sectionHeaderCount, uint16_t sectionHeaderStringTableIndex, unsigned char mode_elf, int silent);
