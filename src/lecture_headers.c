/*
Creer le 4 janvier 2016 par Jonathan

Lecture d'un fichier elf et affichage du Header
*/

#include "lecture_headers.h"

#define MEMORISER(N,E)	E = lire_octets(headers.e_ident[EI_DATA],f,N);

long long int lire_octets(char mode, FILE *f, int nb_octet)
{
	int i;
	long long int retour = 0;
	for(i=0 ; i<nb_octet;i++)
	{
		if(mode == B_ENDIAN) 
		{
			retour = (retour << 8) | fgetc(f);			
		}
		else if (mode == L_ENDIAN)
		{
			
			retour = retour | (fgetc(f) << 8*i);
		}
	}
	return retour;
}


Elf32_Ehdr lecture_Headers(FILE *f, int silent)
{
	long long int lec_Cour = 0;
	Elf32_Ehdr headers;
	headers.e_ident[EI_DATA]=(char) B_ENDIAN;
	int i;

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,4);

	if (lec_Cour != 0x7f454c46)
	{
		printf("Erreur lecture_Header : 7f45 4c46\n");
		printf("Octets lus : %llx\n", lec_Cour);
		exit(1);
	}

	MEMORISER(1,headers.e_ident[EI_CLASS]);
	if (headers.e_ident[EI_CLASS] != 1 && headers.e_ident[EI_CLASS] != 2)
	{
		printf("Erreur lecture_Header : Classe\n");
		printf("Octets lus : %x\n", headers.e_ident[EI_CLASS]);
		exit(1);
	}

	headers.e_ident[EI_MAG0]=0x7F;
	headers.e_ident[EI_MAG1]='E';
	headers.e_ident[EI_MAG2]='L';
	headers.e_ident[EI_MAG3]='F';

	MEMORISER(1,headers.e_ident[EI_DATA]);
	if (headers.e_ident[EI_DATA] != 1 && headers.e_ident[EI_DATA] != 2)
	{
		printf("Erreur lecture_Header : Data\n");
		printf("Octets lus : %x\n", headers.e_ident[EI_DATA]);
		exit(1);
	}

	if (headers.e_ident[EI_DATA] == 0x01)
		headers.e_ident[EI_DATA] = (char) L_ENDIAN;
	else
		headers.e_ident[EI_DATA] = (char) B_ENDIAN;

	MEMORISER(1,headers.e_ident[EI_VERSION]);
	if(! (headers.e_ident[EI_VERSION]))
	{
		printf("Erreur lecture_Header : Version header\n");
		printf("Octets lus : %x\n", headers.e_ident[EI_VERSION]);
		exit(1);
	}

	lire_octets(headers.e_ident[EI_DATA],f,9);
	for(i=7;i<EI_NIDENT;i++) headers.e_ident[i]=0x0;

	MEMORISER(2,headers.e_type);
	switch(headers.e_type)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 0xff00:
		case 0xffff:
			break;
		default:
			printf("Erreur lecture_Header : Type\n");
			exit(1);
	}

	MEMORISER(2,headers.e_machine);

	MEMORISER(4,headers.e_version);
	if(! (headers.e_version))
	{
		printf("Erreur lecture_Header : Version\n");
		printf("Octets lus : %llx\n", lec_Cour);
		exit(1);
	}

	MEMORISER(4,headers.e_entry);

	MEMORISER(4,headers.e_phoff);

	MEMORISER(4,headers.e_shoff);

	MEMORISER(4,headers.e_flags);

	MEMORISER(2,headers.e_ehsize);

	MEMORISER(2,headers.e_phentsize);

	MEMORISER(2,headers.e_phnum);

	MEMORISER(2,headers.e_shentsize);

	MEMORISER(2,headers.e_shnum);

	MEMORISER(2,headers.e_shstrndx);

	if(!silent)
	{
		afficher_headers(headers);
	}

	return headers;

}


void afficher_headers(Elf32_Ehdr headers)
{
	int i;
	printf("ELF Header:\n  Magic:   ");
	for(i=0;i<EI_NIDENT;i++) printf("%02x ",headers.e_ident[i]);
	printf("\n  Class:                             ELF%d\n",headers.e_ident[EI_CLASS]*32);
	printf("  Data:                              2's complement, ");
	if (headers.e_ident[EI_DATA] == L_ENDIAN) printf("little endian\n");
	else printf("big endian\n");

	printf("  Version:                           %d (current)\n",headers.e_ident[EI_VERSION]);

	printf("  OS/ABI:                            ");
	if(headers.e_machine==40)
		printf("UNIX - System V\n");
	else
		printf("Machine numero : %d (incompatible)\n", headers.e_machine);

	printf("  ABI Version:                       0\n");

	printf("  Type:                              ");
	switch(headers.e_type)
	{
		case 0:
			printf("NONE (No file type)\n");
			break;
		case 1:
			printf("REL (Relocatable file)\n");
			break;
		case 2:
			printf("EXEC (Executable)\n");
			break;
		case 3:
			printf("DYN (Shared object file)\n");
			break;
		case 4:
			printf("CORE (Core file)\n");
			break;
		case 0xff00:
			printf("LOPROC (Processor-specific)\n");
			break;
		case 0xffff:
			printf("HIPROC (Processor-specific)\n");
			break;
	}
	printf("  Machine:                           ARM\n");
	printf("  Version:                           0x%x\n",headers.e_version);
	printf("  Entry point address:               0x%x\n",headers.e_entry);

	printf("  Start of program headers:          %u (bytes into file)\n",headers.e_phoff);

	printf("  Start of section headers:          %u (bytes into file)\n",headers.e_shoff);

	printf("  Flags:                             0x%x",headers.e_flags);
	if(headers.e_flags==0x5000000) printf(", Version5 EABI");
	printf("\n");

	printf("  Size of this header:               %d (bytes)\n", headers.e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n", headers.e_phentsize);
	printf("  Number of program headers:         %d\n", headers.e_phnum);
	printf("  Size of section headers:           %d (bytes)\n", headers.e_shentsize);
	printf("  Number of section headers:         %d\n", headers.e_shnum);

	if(headers.e_shstrndx==SHN_UNDEF)
		printf("  Section header string table index: SHN_UNDEF\n");
	else
		printf("  Section header string table index: %d\n", headers.e_shstrndx);

}
