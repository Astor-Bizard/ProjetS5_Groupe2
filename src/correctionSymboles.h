#ifndef __AFF_SECTION__
#define __AFF_SECTION__

ListeSymboles corrigerSymboles(FILE* oldFile, FILE* newFile, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader, Elf32_Shdr* originalSH, Elf32_Shdr* newSH, ListeSymboles oldST);

#endif