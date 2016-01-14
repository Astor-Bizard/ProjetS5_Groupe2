/*
Crée le 5 janvier 2016 par Loïc
Lecture de la table des sections
*/
#ifndef _ECRITURESH
#define _ECRITURESH

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include "lecture_headers.h"
#include "lectureSH.h"
#include "correctionSymboles.h"

/*
* Ecrit une liste de section dans un fichier ELF
* @param f Le fichier ouvert en écriture
* @param elfHeader Le header du fichier
* @param shList La liste à recopier
*/
void writeSectionsHeadersToFile(FILE *f, Elf32_Ehdr elfHeader, SectionsHeadersList shList);

#endif
