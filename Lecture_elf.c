/*
Creer le 4 janvier 2016 par Jonathan

Lecture d'un fichier elf et affichage du Header



*/
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>

#define BIG_ENDIAN 2
#define LITTLE_ENDIAN 1

long long int lireoctets(int mode, FILE *f, int nb_octet);
int lecture_Headers(FILE *f);

int main(int argc, char *argv[])
{
	FILE *f;
	int mode;

	if (argc != 1)
	{
		printf("Nb argument incorrect ! \n1 argument requis : nom du fichier lue \n");
		return 1;
	}

	f = fopen(argv[1],"r");

	mode = lecture_Headers(f);
	




	return 0;


}

/*
	lit nb_octet du fichier f, en little endian si mode = 1, 
	en big endian si mode = 2
*/

long long int lire_octets(int mode, FILE *f, int nb_octet)
{
	int i;
	long long int retour = 0;

	int k=1;
	for(i=0 ; i<nb_octet;i++)
	{
		k = k * 128;
	}

	for(i=0 ; i<nb_octet;i++)
	{
		if(mode == BIG_ENDIAN) 
		{
			retour = retour * 128 + fgetc(f);
		}
		else if (mode == LITTLE_ENDIAN)
		{
			retour = retour / 128 + fgetc(f) * k;
		}
	}
	return retour;
}



int lecture_Headers(FILE *f)
{

	long long int lec_Cour = 0;
	int octet_cour = 0;
	int mode = BIG_ENDIAN;
	
	printf("ELF Header: \n");
	lec_Cour = lire_octets(BIG_ENDIAN,f,4);
	octet_cour += 4;

	if (lec_Cour != Ox7f454c46)
	{
		printf("erreur lecture_Header : 7f45 4c46 (%i)", octet_cour);
		exit(1);
	}

	lec_Cour = lire_octets(BIG_ENDIAN,f,1);
	octet_cour++;

	if (lec_Cour != 1 && lec_Cour != 2)
	{
		printf("erreur lecture_Header : Classe (%i)", octet_cour);
		exit(1);
	}
	printf("  Class: \tELF%llu\n",32*lec_Cour);

	lec_Cour = lire_octets(BIG_ENDIAN,f,1);
	octet_cour++;

	if (lec_Cour != 1 && lec_Cour != 2)
	{
		printf("erreur lecture_Header : Data (%i)", octet_cour);
		exit(1);
	}

	if (Lec_Cour == 1)
	{
		prinf("  Data: \t LITTLE ENDIAN\n");
		mode = LITTLE_ENDIAN
	}
	else
	{
		prinf("  Data: \t BIG ENDIAN\n");
	}

	lec_Cour = lire_octets(mode,f,1);
	octet_cour++;
	if(! (Lec_Cour))
	{
		printf("erreur lecture_Header : Version header (%i)", octet_cour);
		exit(1);
	}
	else
	{
		printf("  Version header:\t Version courante:(%llu)\n",lec_Cour);
	}

	lire_octets(mode,f,9);
	octet_cour +=9;

	lec_Cour = lire_octets(mode,f,2);
	octet_cour+=2;
	printf("\n  Type: \t");
	switch(lec_Cour)
	{
		case 0:
			printf("No file type");
			break;
		case 1:
			printf("Relocatable file");
			break;
		case 2:
			printf("Executable");
			break;
		case 3:
			printf("Shared object file");
			break;
		case 4:
			printf("Core file");
			break;
		case 0xff00:
			printf("Processor-specific");
			break;
		case 0xffff:
			printf("Processor-specific");
			break;
		default:
			printf("erreur lecture_Header : Version (%i)", octet_cour);
			exit(1);
	}
	printf("\n");

	lec_Cour = lire_octets(mode,f,2);
	octet_cour+=2;
	printf("  Machine:\t");
	switch(lec_Cour)
	{
		case 0:
			printf("No machine\n");
			break;
		case 1:
			printf("AT&T WE 32100\n");
			break;
		case 3:
			printf("SPARC\n");
			break;
		case 4:
			printf("Intel 80386\n");
			break;
		case 5:
			printf("Motorola 68000\n");
			break;
		case 6:
			printf("Motorola 88000\n");
			break;
		case 7:
			printf("Intel 80860\n");
			break;
		case 8:
			printf("MIPS RS3000\n");
			break;
	}

	lec_Cour = lire_octets(mode,f,4);
	octet_cour+=4;
	if(! (Lec_Cour))
	{
		printf("erreur lecture_Header : Version (%i)", octet_cour);
		exit(1);
	}
	else
	{
		printf("  Version :\t Version courante:(%llu)\n",lec_Cour);
	}

	lec_Cour = lire_octets(mode,f,4);
	octet_cour+=4;
	printf("  Entry point address: \t%llu\n",lec_Cour);

	lec_Cour = lire_octets(mode,f,4);
	octet_cour+=4;
	if(Lec_Cour)
	{	
		printf("  Start of program headers: \t%llu\n",lec_Cour);
	}
	else
	{
		printf("  Start of program headers: \t0(no programme header table)\n");
	}

	lec_Cour = lire_octets(mode,f,4);
	octet_cour+=4;
	if(Lec_Cour)
	{	
		printf("  Start of program section headers: \t%llu\n",lec_Cour);
	}
	else
	{
		printf("  Start of program section headers: \t0(no programme header table)\n");
	}




	return mode;

}
