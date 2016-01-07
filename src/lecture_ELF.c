/* 
Creer le 5/01/2015 par Jonathan
Programme principal de la partie 1

*/

#include "lecture_headers.h"
#include "afficher_section.h"
#include "lectureST.h"
#include "lectureSH.h"
#include "affichage_relocation.h"

void print_usage() {
	printf("Usage: lecture_ELF <option(s)> elf-file(s)\n");
	printf(" Display information about the contents of ELF format files\n");
	printf(" Options are:\n");
	printf("  -a --all               Equivalent to: -h -l -S -s -r -d -V -A -I\n");
	printf("  -h --file-header       Display the ELF file header\n");
	printf("  -l --program-headers   Display the program headers\n");
	printf("  -S --section-headers   Display the sections' header\n");
	printf("  -g --section-groups    Display the section groups\n");
	printf("  -t --section-details   Display the section details\n");
	printf("  -e --headers           Equivalent to: -h -l -S\n");
	printf("  -s --syms              Display the symbol table\n");
	printf("  -r --relocs            Display the relocations (if present)\n");
	printf("  -x --hex-dump=<number|name> Dump the contents of section <number|name> as bytes\n");
	printf("  -H --help              Display this information\n");
}

int main(int argc, char *argv[])
{

	Elf32_Ehdr elfHeaders;
	Elf32_Shdr *section_headers;
	ListeSymboles sym_tab;
	FILE* f;

	if(argc < 2) {
		print_usage();
		exit(1);
	}

	f = fopen(argv[1], "r");
	elfHeaders = lecture_Headers(f);
	fclose(f);

	f = fopen(argv[1], "r");
	section_headers = lectureSectionHeader(f,elfHeaders,0);
	fclose(f);

	afficher_section(argv[1], elfHeaders, section_headers,0);

	f = fopen(argv[1], "r");
	sym_tab = lectureSymbolTab(f,elfHeaders,section_headers,0);
	fclose(f);

	affichage_relocation(argv[1],elfHeaders,section_headers,sym_tab);

	return 0;
}