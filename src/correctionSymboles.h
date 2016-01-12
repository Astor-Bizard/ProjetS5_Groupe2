#ifndef __CORRIGER_SYMBOLES__
#define __CORRIGER_SYMBOLES__

#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include "lecture_headers.h"
#include "afficher_section.h"
#include "lectureSH.h"
#include "lectureST.h"

#define ByteSwap16(n) (((((unsigned int) n) << 8) & 0xFF00) | ((((unsigned int) n) >> 8) & 0x00FF))
#define ByteSwap32(n) (((((unsigned long) n) << 24) & 0xFF000000) | ((((unsigned long) n) <<  8) & 0x00FF0000) | ((((unsigned long) n) >>  8) & 0x0000FF00) | ((((unsigned long) n) >> 24) & 0x000000FF))

/*
* Ecrit un mot de 4 octets dans un fichier selon l'endianness indiquée
* @param f Le fichier dans lequel écrire, ouvert en écriture
* @param mode Le mode d'écriture, L_ENDIAN pour du little endian, autre chose pour du big endian
* @param value Le mot de 4 octets à écrire
* @return Voir man fwrite.
*/
size_t fwrite32(FILE* f, int mode, uint32_t value);

/*
* Ecrit un mot de 2 octets dans un fichier selon l'endianness indiquée
* @param f Le fichier dans lequel écrire, ouvert en écriture
* @param mode Le mode d'écriture, L_ENDIAN pour du little endian, autre chose pour du big endian
* @param value Le mot de 2 octets à écrire
* @return Voir man fwrite.
*/
size_t fwrite16(FILE* f, int mode, uint16_t value);

/*
* Ecrit un mot de 1 octets dans un fichier selon l'endianness indiquée (=> Alias de 'fwrite(value, 1, 1, f)')
* @param f Le fichier dans lequel écrire, ouvert en écriture
* @param mode Le mode d'écriture, L_ENDIAN pour du little endian, autre chose pour du big endian
* @param value Le mot de 1 octets à écrire
* @return Voir man fwrite.
*/
size_t fwrite8(FILE* f, int mode, uint8_t value);

/*
* Corrige la table des symboles selon les modifications de la table des en-têtes de section effectuées précedemment.
* st_shndx est ajusté en fonction de la renumérotation des sections, st_value est ajuste en fonction de la nouvelle adresse des sections.
* @param oldFile Le fichier original, ouvert en lecture
* @param oldElfHeader Le header du fichier original
* @param newElfHeader Le header du nouveau fichier
* @param oldSectionsHeaders La table des en-têtes de sections du fichier original
* @param newSectionHeaders La table des en-têtes de sections du nouveau fichier
* @param oldSymbolsTable La table des symboles du fichier original
* @param silent Si silent est à 1, aucun affichage ne sera produit en dehors des erreurs
* @return La table des symboles corrigée
*/
ListeSymboles applySymbolsCorrections(FILE* oldFile, Elf32_Ehdr oldElfHeader, Elf32_Ehdr newElfHeader, SectionsHeadersList oldSHList, SectionsHeadersList newSHList, ListeSymboles oldSymbolsTable, int silent);

/*
* Ecrit la table des symboles dans un fichier
* @param file Le fichier dans lequel écrire la table, ouvert en écriture
* @param elfHeader Le header du fichier
* @param shList La liste des en-têtes de sections du fichier
* @param symbolsTable La table des symboles à écrire
*/
void writeSymbolsToFile(FILE* file, Elf32_Ehdr elfHeader, SectionsHeadersList shList, ListeSymboles symbolsTable);

#endif