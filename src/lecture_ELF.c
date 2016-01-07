/* 
Crée le 5/01/2015 par Jonathan
Programme principal de la partie 1
*/
#include "lecture_headers.h"
#include "afficher_section.h"
#include "lectureST.h"
#include "lectureSH.h"
#include "affichage_relocation.h"

#define OPTION_ALL 					0x001f
#define OPTION_FILE_HEADER			(1 << 0)
#define OPTION_SECTION_HEADERS		(1 << 1)
#define OPTION_HEADERS				0x0003
#define OPTION_SYMS					(1 << 2)
#define OPTION_RELOCS				(1 << 3)
#define OPTION_HEX_DUMP				(1 << 4)
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
	printf("  -H                 Display this information\n");
}

int main(int argc, char *argv[]) {
	Elf32_Ehdr elfHeaders;
	Elf32_Shdr *section_headers;
	ListeSymboles sym_tab;
	char* fileName;
	char* hex_param;
	FILE* f;
	int i;
	uint16_t options = 0;

	if(argc < 2) {
		print_usage();
		exit(1);
	}

	if(argv[1][0]!='-') {
		fileName = argv[argc-1];
		i = 1;
	}
	else {
		fileName = argv[1];
		i = 2;
	}

	while (i < argc) {
		if (argv[i][0] == '-') {
			switch(argv[i][1]) {
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
					options = options | OPTION_HEX_DUMP;
					if (argv[i+1][0] != '-')
						hex_param = argv[i++];
					else
						hex_param = NULL;
					break;
				case 'H':
					print_usage();
					break;
				default:
					printf("Unrecognized option: -%c\n\n", argv[i][1]);
					print_usage();
					break;
			}
		}
		i++;
	}
	if (!options)
		options = OPTION_ALL;

	f = fopen(fileName, "r");
	elfHeaders = lecture_Headers(f);
	fclose(f);

	f = fopen(fileName, "r");
	section_headers = lectureSectionHeader(f, elfHeaders, (!(options & OPTION_SECTION_HEADERS)));
	fclose(f);

	if(options & OPTION_HEX_DUMP) {
		if(hex_param == NULL) {
			f = fopen(fileName, "r");
			afficher_section(f, elfHeaders, section_headers, 0);
			fclose(f);
		}
		else {
			printf("Not working yet.\n");
		}
	}

	f = fopen(fileName, "r");
	sym_tab = lectureSymbolTab(f, elfHeaders, section_headers, (!(options & OPTION_SYMS)));
	fclose(f);

	if(options & OPTION_RELOCS) {
		f = fopen(fileName, "r");
		affichage_relocation(f, elfHeaders, section_headers, sym_tab);
		fclose(f);
	}

	return 0;
}
