/*
Crée le 4 janvier 2016 par Loïc
Lecture de la table des sections
*/
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "lecture_headers.h"
#include "lectureSH.h"

int main(int argc, char *argv[]) {
	FILE* f = fopen(argv[1], "r");

	Elf32_Ehdr elfHeader = lecture_Headers(f);

	Elf32_Shdr* sectionsHeaders = lectureSectionHeader(f, elfHeader.e_shoff, elfHeader.e_shentsize, elfHeader.e_shnum, int elfHeader.e_shstrndx, elfHeader.e_ident[EI_DATA], 0);

	fclose(f);

	return 0;
}