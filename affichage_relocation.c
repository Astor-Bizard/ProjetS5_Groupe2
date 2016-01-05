#include <stdio.h>
#include <stdin.h>
#include "lecture_header.h"
#include "afficher_section.h"



long long unsigned int lire_octets_charT(char *tableau, int mode, int debut, int nombre)
{
	int i;
	int k=1;
	long long unsigned int R=0;
	if(mode==2)
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
	int addr = lire_octets_charT(section,header.e_idata[5],0,4);

}

// trouve toutes les sections de relocations et les affiche.
void affichage_relocation(Elf32_Ehdr header,Elf_32Shdr* table_section)
{
	int i;
	int j;
	for(i=0;i<header.shnum)
	{
		// on vérifie toutes les sections
		// si ce sont des sections de relocations:
		if(table_section[i][0]=='.' 
			&& table_section[i][1]=='r' 
			&& table_section[i][2]=='e' 
			&& table_section[i][3]=='l' 
			&& table_section[i][4]=='a' )

		{
			afficher_sectionRA(f,table_section,header,i);
		}
		else if (table_section[i][0]=='.' 
			&& table_section[i][1]=='r' 
			&& table_section[i][2]=='e' 
			&& table_section[i][3]=='l' )
		{
			afficher_sectionR(f,table_section,header,i);
		}
	}
}

			
