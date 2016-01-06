/*
Creer le 4 janvier 2016 par Jonathan

Lecture d'un fichier elf et affichage du Header
*/

#include "lecture_headers.h"



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




Elf32_Ehdr lecture_Headers(FILE *f)
{

	long long int lec_Cour = 0;
	int octet_cour = 0;
	Elf32_Ehdr headers;
	headers.e_ident[EI_DATA]=(char) B_ENDIAN;

	printf("ELF Header: \n");
	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,4);
	octet_cour += 4;

	if (lec_Cour != 0x7f454c46)
	{
		printf("erreur lecture_Header : 7f45 4c46 (%i)\n", octet_cour);
		printf("Octets lue : %llx\n", lec_Cour);
		exit(1);
	}

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,1);
	octet_cour++;

	if (lec_Cour != 1 && lec_Cour != 2)
	{
		printf("erreur lecture_Header : Classe (%i)\n", octet_cour);
		printf("Octets lue : %llx\n", lec_Cour);
		exit(1);
	}

	printf("  Class: \t\t\t\tELF%llu\n",32*lec_Cour);

	headers.e_ident[EI_MAG0]=0x7F;
	headers.e_ident[EI_MAG1]='E';
	headers.e_ident[EI_MAG2]='L';
	headers.e_ident[EI_MAG3]='F';
	headers.e_ident[EI_CLASS]= 32*lec_Cour;

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,1);
	octet_cour++;

	if (lec_Cour != 1 && lec_Cour != 2)
	{
		printf("erreur lecture_Header : Data (%i)\n", octet_cour);
		printf("Octets lue : %llx\n", lec_Cour);
		exit(1);
	}

	if (lec_Cour == 0x01)
	{
		printf("  Data: \t\t\t\tLITTLE ENDIAN\n");
		headers.e_ident[EI_DATA] = (char) L_ENDIAN;
	}
	else
	{
		printf("  Data: \t\t\t\tBIG ENDIAN\n");
		headers.e_ident[EI_DATA] = (char) B_ENDIAN;
	}

	

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,1);
	octet_cour++;
	if(! (lec_Cour))
	{
		printf("erreur lecture_Header : Version header (%i)\n", octet_cour);
		printf("Octets lue : %llx\n", lec_Cour);
		exit(1);
	}
	else
	{
		printf("  Version header: \t\t\t%llu (current)\n",lec_Cour);
	}

	headers.e_ident[EI_VERSION]=lec_Cour;

	lire_octets(headers.e_ident[EI_DATA],f,9);
	octet_cour +=9;

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,2);
	octet_cour+=2;
	printf("  Type: \t\t\t\t");
	switch(lec_Cour)
	{
		case 0:
			printf("No file type\n");
			break;
		case 1:
			printf("Relocatable file\n");
			break;
		case 2:
			printf("Executable\n");
			break;
		case 3:
			printf("Shared object file\n");
			break;
		case 4:
			printf("Core file\n");
			break;
		case 0xff00:
			printf("Processor-specific\n");
			break;
		case 0xffff:
			printf("Processor-specific\n");
			break;
		default:
			printf("erreur lecture_Header : Type(%i)\n", octet_cour);
			exit(1);
	}

	headers.e_type=lec_Cour;

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,2);
	octet_cour+=2;
	printf("  OS/ABI: \t\t\t\t");
	
	if(lec_Cour==40)
	{
		printf("UNIX - System V\n");
	}
	else
	{
		printf("Machine numero : %llu (incompatible)", lec_Cour);
	}

	headers.e_machine=lec_Cour;

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,4);
	octet_cour+=4;
	if(! (lec_Cour))
	{
		printf("erreur lecture_Header : Version (%i)\n", octet_cour);
		printf("Octets lue : %llx\n", lec_Cour);
		exit(1);
	}
	else
	{
		printf("  Version : \t\t\t\t%llx\n",lec_Cour);
	}
	headers.e_version=lec_Cour;

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,4);
	octet_cour+=4;
	printf("  Entry point address: \t\t\t%llx\n",lec_Cour);

	headers.e_entry=lec_Cour;

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,4);
	octet_cour+=4;
	if(lec_Cour)
	{	
		printf("  Start of program headers: \t\t%llu\n",lec_Cour);
	}
	else
	{
		printf("  Start of program headers: \t\t0 (no program header table)\n");
	}

	headers.e_phoff=lec_Cour;

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,4);
	octet_cour+=4;
	if(lec_Cour)
	{	
		printf("  Start of program section headers: \t%llu\n",lec_Cour);
	}
	else
	{
		printf("  Start of program section headers: \t0 (no program header table)\n");
	}

	headers.e_shoff=lec_Cour;

	
	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,4);
	octet_cour+=4;

	headers.e_flags=lec_Cour;
	printf("  Flags: \t\t\t\t%llx\n",lec_Cour);

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,2);
	octet_cour+=2;

	headers.e_ehsize=lec_Cour;
	printf("  Size of this header: \t\t\t%llu (bytes)\n", lec_Cour);

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,2);
	octet_cour+=2;

	headers.e_phentsize=lec_Cour;
	printf("  Size of this program header: \t\t%llu (bytes)\n", lec_Cour);

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,2);
	octet_cour+=2;

	headers.e_phnum=lec_Cour;
	printf("  Number of program headers: \t\t%llu\n", lec_Cour);
	
	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,2);
	octet_cour+=2;
	
	headers.e_shentsize=lec_Cour;
	printf("  Size of section headers: \t\t%llu (bytes)\n", lec_Cour);

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,2);
	octet_cour+=2;
	
	headers.e_shnum=lec_Cour;
	printf("  Number of section headers: \t\t%llu\n", lec_Cour);

	lec_Cour = lire_octets(headers.e_ident[EI_DATA],f,2);
	octet_cour+=2;
	
	if(!lec_Cour)
	{
		headers.e_shstrndx=SHN_UNDEF;
		printf("  Section header string table index: \tSHN_UNDEF\n");

	}
	else
	{
		headers.e_shstrndx=lec_Cour;
		printf("  Section header string table index: \t%llu\n", lec_Cour);
	}

	printf("\n");
	return headers;

}