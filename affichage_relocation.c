//05/01/2016 Gabriel

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "lectureSH.h"
#include "lecture_headers.h"
#include "afficher_section.h"
#include "affichage_relocation.h"



long long unsigned int lire_octets_charT(unsigned char *tableau, int hdr_mode, int debut, int nombre)
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
    return R;
}

void type_relocation(int info)
{
    switch(info)
    {
        case 0:
            printf("R_ARM_NONE");
            break;
        case 1:
            printf("R_ARM_PC24");
            break;
        case 2:
            printf("R_ARM_ABS32");
            break;
        case 3:
            printf("R_ARM_REL32");
            break;
        case 4:
            printf("R_ARM_LDR_PC_G0");
            break;
        case 5:
            printf("R_ARMABS16");
            break;
        case 6:
            printf("R_ARM_ABS12");
            break;
        case 7:
            printf("R_ARM_THM_ABS5");
            break;
        case 8:
            printf("R_ARM_ABS8");
            break;
        case 9:
            printf("R_ARM_SBREL32");
            break;
        case 10:
            printf("R_ARM_THM_CALL");
            break;
        case 11:
            printf("R_ARM_THM_PC8");
            break;
        case 12:
            printf("R_ARM_BREL_ADJ");
            break;
        case 13:
            printf("R_ARM_TLS_DESC");
            break;
        case 14:
            printf("R_ARM_THM_SWI8");
            break;    
        case 15:
            printf("R_ARM_XPC25");
            break;
        case 16:
            printf("R_ARM_THM_XPC22");
            break;
        case 17:
            printf("R_ARM_DTPMOD32");
            break;
        case 18:
            printf("R_ARM_TLS_DTPOFF32");
            break;
        case 19:
            printf("R_ARM_TLS_TPOFF32");
            break;
        case 20:
            printf("R_ARM_COPY");
            break;
        case 21:
            printf("R_ARM_GLOB_DAT");
            break;
        case 22:
            printf("R_ARM_JUMP_SLOT");
            break;
        case 23:
            printf("R_ARM_RELATIVE");
            break;
        case 24:
            printf("R_ARM_GOTOFF32");
            break;
        case 25:
            printf("R_ARM_BASE_PREL");
            break;
        case 26:
            printf("R_ARM_GOT_PREL");
            break;
        case 27:
            printf("R_ARM_PLT32");
            break;
        case 28:
            printf("R_ARM_CALL");
            break;
        case 29:
            printf("R_ARM_JUMP24");
            break;
        case 30:
            printf("R_ARM_THM_JUMP24");
            break;
       
        
    }
}



//affiche une section de relocation
void afficher_sectionR(char *f,Elf32_Shdr* table_section,Elf32_Ehdr header,int numS, char* SectionNames)
{
	int i;
	unsigned char *section = afficher_section_num(f,header,table_section, numS);
	int addr;
	int info;
    
    char *nom_section= getSectionNameBis(SectionNames,table_section[numS]);
        
	printf("Section de relocalisation 'nomSection' à l'adresse de décalage contient %i entrées:\n",
			table_section[numS].sh_size);
    //table_section[numS].sh_name
	printf("  Décalage \t  Info \t  Type\t  Val.-sym\t Noms-symboles\n");
	for(i=0; i<table_section[i].sh_size/8; i++)
	{
		addr = lire_octets_charT(section,header.e_ident[EI_DATA],i*8,4);
		info = lire_octets_charT(section,header.e_ident[EI_DATA],i*8 + 4,4);
		printf("%xllu\t%xllu\t",addr,info);
                type_relocation(info);
                printf("Type\t Valeur_Symbol\t");
                
                
                printf("%s",nom_section);
                
                //on affiche la traductions des infos.
                printf("\n");
	}
}

//affiche une section de relocation_A
void afficher_sectionRA(char *f,Elf32_Shdr* table_section,Elf32_Ehdr header,int numS, char* SectionNames)
{
    afficher_sectionR(f,table_section,header,numS, SectionNames);
}

// trouve toutes les sections de relocations et les affiche.
void affichage_relocation(char* f,Elf32_Ehdr header,Elf32_Shdr* table_section)
{
	int i=0,j=1;
	char* SectionNames;
    FILE *fichier ;
    fichier = fopen(f,"r");

	SectionNames = fetchSectionNames(fichier,header, table_section);
    int nchar_nomSection = table_section[header.e_shstrndx].sh_size;


	while(i<header.e_shnum && j < nchar_nomSection)
	{
		
		// on vérifie toutes les sections
		// si ce sont des sections de relocations:
		if(j >= 5
            && SectionNames[j-5]==0 
            && SectionNames[j-4]=='.' 
			&& SectionNames[j-3]=='r' 
			&& SectionNames[j-2]=='e' 
			&& SectionNames[j-1]=='l' 
			&& SectionNames[j]=='a' )

		{
			afficher_sectionRA(f,table_section,header,i,SectionNames);
		}
		else if (j >=4
            && SectionNames[j-4]==0 
            && SectionNames[j-3]=='.' 
			&& SectionNames[j-2]=='r' 
			&& SectionNames[j-1]=='e' 
			&& SectionNames[j]=='l' )
		{
			afficher_sectionR(f,table_section,header,i,SectionNames);
		}
        if( SectionNames[j]==0 )
        {
            i++;
        }
	}
}

			
