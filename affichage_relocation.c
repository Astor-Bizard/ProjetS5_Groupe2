//05/01/2016 Gabriel

#include <stdio.h>
#include <stdin.h>
#include "lecture_header.h"
#include "afficher_section.h"



long long unsigned int lire_octets_charT(char *tableau, int hdr_mode, int debut, int nombre)
{
	int i;
	int k=1;
	long long unsigned int R=0;
	if(hdr_mode==2)
	{
		for(i=0;i<nombre;i++)
		{
			R = R*256 + tableau[debut+i];
		}
	}
	else
	{
		for(i=0;i<nombre;i++)
		{
			k=k*256;
		}
		for(i=0; i<nombre; i++)
		{
			R = tableau[debut+i]*k + R/256;
		}
	}
}


//affiche une section de relocation_A
void afficher_sectionRA(char *f,Elf32_Shdr *table_section,Elf32_Ehdr header,int numS)
{

}

//affiche une section de relocation
void afficher_sectionR(char *f,Elf32_Shdr *table_section,Elf32_Ehdr header,int numS)
{
	int i;
	char* section=afficher_section(f,table_section,header.e_shnum, numS);
	int addr;
	int info;

	printf("Section de relocalisation '%s' à l'adresse de décalage contient %i entrées:\n",
			table_section[i], table_section.sh_size);
	printf("  Décalage \t  Info \t  Type\t  Val.-sym\t Noms-symboles\n")
	for(i=0; i<table_section.sh_size; i++)
	{
		addr = lire_octets_charT(section,header.e_idata,i*8,4);
		info = lire_octets_charT(section,header.e_idata,i*8 + 4,4);
		printf("%i\t%i",addr,info);
		

	}
	

}

// trouve toutes les sections de relocations et les affiche.
void affichage_relocation(Elf32_Ehdr header,Elf_32Shdr* table_section)
{
	int i;
	int j;
	char** SectionName;
	SectionName = getSectionNames(f,header.e_shnum,section.sh_size,header.)
	for(i=0;i<header.shnum)
	{
		
		// on vérifie toutes les sections
		// si ce sont des sections de relocations:
		if(SectionName[i][0]=='.' 
			&& SectionName[i][1]=='r' 
			&& SectionName[i][2]=='e' 
			&& SectionName[i][3]=='l' 
			&& SectionName[i][4]=='a' )

		{
			afficher_sectionRA(f,table_section,header,i);
		}
		else if (SectionName[i][0]=='.' 
			&& SectionName[i][1]=='r' 
			&& SectionName[i][2]=='e' 
			&& SectionName[i][3]=='l' )
		{
			afficher_sectionR(f,table_section,header,i);
		}
	}
}

			
