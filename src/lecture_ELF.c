/* 
Créé le 5/01/2015 par Jonathan
Programme principal de la partie 1
*/
#include "lecture_headers.h"
#include "afficher_section.h"
#include "lectureST.h"
#include "lectureSH.h"
#include "affichage_relocation.h"
#include "liberation.h"

#define OPTION_ALL 					0x001f
#define OPTION_FILE_HEADER			(1 << 0)
#define OPTION_SECTION_HEADERS		(1 << 1)
#define OPTION_HEADERS				0x0003
#define OPTION_SYMS					(1 << 2)
#define OPTION_RELOCS				(1 << 3)
#define OPTION_SECTION_DISPLAY		(1 << 4)
#define OPTION_HELP					(1 << 5)

void print_usage() {
	printf("Usage: lecture_ELF <option(s)> elf-file\n");
	printf(" Display information about the contents of ELF format files\n");
	printf(" Options are:\n");
	printf("  -a                 Equivalent to: -h -S -s -x -r\n");
	printf("  -h                 Display the ELF file header\n");
	printf("  -S                 Display the sections' header\n");
	printf("  -e                 Equivalent to: -h -S\n");
	printf("  -s                 Display the symbol table\n");
	printf("  -r                 Display the relocations (if present)\n");
	printf("  -x <number|name>   Dump the contents of section <number|name> as bytes. If no name/number, it will be asked during execution.\n");
	printf("  -H                 Display this information\n\n");
}

int main(int argc, char *argv[]) {
	Elf32_Ehdr elfHeaders;
	SectionsHeadersList shList;
	ListeSymboles symbolsTable;
	Str_Reloc relocationTable;
	char* fileName;
	char* sectionToDisplay = NULL;
	FILE* f;
	int fileNamePos, i;
	uint16_t options = 0;

	if(argc < 2) {
		print_usage();
		return 0;
	}

	if(argv[1][0]=='-') {
		fileNamePos = argc-1;
		i = 1;
	}
	else {
		fileNamePos = 1;
		i = 2;
	}

	fileName = argv[fileNamePos];
	f = fopen(fileName, "r");
	if (f == NULL) {
		printf("Fichier introuvable: %s\n", fileName);
		return 0;
	}

	while (i < argc) 
	{
		if (argv[i][0] == '-') 
		{
			switch(argv[i][1]) 
			{
				case 'a':
					options = options | OPTION_ALL;
					break;
				case 'h':
					options = options | OPTION_FILE_HEADER;
					break;
				case 'S':
					options = options | OPTION_SECTION_HEADERS;
					break;
				case 'e':
					options = options | OPTION_HEADERS;
					break;
				case 's':
					options = options | OPTION_SYMS;
					break;
				case 'r':
					options = options | OPTION_RELOCS;
					break;
				case 'x':
					options = options | OPTION_SECTION_DISPLAY;
					if (i+1<argc && i+1!=fileNamePos && argv[i+1][0] != '-')
						sectionToDisplay = argv[++i];
					break;
				case 'H':
					print_usage();
					return 0;
					break;
				default:
					printf("Unrecognized option: -%c\n\n", argv[i][1]);
					print_usage();
					return 0;
					break;
			}
		}
		else 
		{
			if (i != fileNamePos) 
			{
				printf("Unrecognized option: %s\n\n", argv[i]);
				print_usage();
				return 0;
			}
		}
		i++;
	}
	if (!options)
		options = OPTION_ALL;

	elfHeaders = lecture_Headers(f, (!(options & OPTION_FILE_HEADER)));

	shList = readSectionsHeadersFromFile(f, elfHeaders, (!(options & OPTION_SECTION_HEADERS)));

	if(options & OPTION_SECTION_DISPLAY) 
	{
		if(sectionToDisplay == NULL)
			afficher_section(f, elfHeaders, shList, 0, NULL);
		else 
			afficher_section(f, elfHeaders, shList, 0, sectionToDisplay);
	}

	symbolsTable = lectureSymbolTab(f, elfHeaders, shList, (!(options & OPTION_SYMS)));

	relocationTable = affichage_relocation(f, elfHeaders, shList, symbolsTable, (!(options & OPTION_RELOCS)));

	fclose(f);
	free_Str_Reloc(relocationTable);
	free_SectionsHeadersList(sectionsHeadersTable);
	free_ListeSymboles(symbolsTable);
	
	return 0;
}
