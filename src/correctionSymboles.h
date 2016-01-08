#ifndef __AFF_SECTION__
#define __AFF_SECTION__

size_t ecrire32b(FILE* f, int mode, uint32_t value);

size_t ecrire16b(FILE* f, int mode, uint16_t value);

size_t ecrire8b(FILE* f, int mode, uint8_t value);

ListeSymboles corrigerSymboles(FILE* oldFile, FILE* newFile, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader, Elf32_Shdr* originalSH, Elf32_Shdr* newSH, ListeSymboles oldST);

void ecrireNouveauxSymboles(FILE* newFile, Elf32_Ehdr newElfHeader, Elf32_Shdr* newSH, ListeSymboles newST, int silent);

#endif