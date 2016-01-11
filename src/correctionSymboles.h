#ifndef __CORRIGER_SYMBOLES__
#define __CORRIGER_SYMBOLES__

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include "lecture_headers.h"
#include "lectureSH.h"
#include "lectureST.h"

#define ByteSwap16(n) (((((unsigned int) n) << 8) & 0xFF00) | ((((unsigned int) n) >> 8) & 0x00FF))
#define ByteSwap32(n) (((((unsigned long) n) << 24) & 0xFF000000) | ((((unsigned long) n) <<  8) & 0x00FF0000) | ((((unsigned long) n) >>  8) & 0x0000FF00) | ((((unsigned long) n) >> 24) & 0x000000FF))

size_t fwrite32(FILE* f, int mode, uint32_t value);

size_t fwrite16(FILE* f, int mode, uint16_t value);

size_t fwrite8(FILE* f, int mode, uint8_t value);

ListeSymboles corrigerSymboles(FILE* oldFile, FILE* newFile, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader, Elf32_Shdr* originalSH, Elf32_Shdr* newSH, ListeSymboles oldST, int silent);

void ecrireNouveauxSymboles(FILE* newFile, Elf32_Ehdr newElfHeader, Elf32_Shdr* newSH, ListeSymboles newST);

#endif