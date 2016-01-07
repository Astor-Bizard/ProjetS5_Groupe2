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
	long long unsigned int R=0;
	if(hdr_mode == B_ENDIAN)
	{
		for(i=0;i<nombre;i++)
		{
			R = (R<<8) | tableau[debut+i];
		}
	}
	else
	{
		for(i=0; i<nombre; i++)
		{
			R =  R | (tableau[debut+i] << (i*8));
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
        default:
            printf("Case: default");
    }
}

void print_section(unsigned long long int addr,Elf32_Shdr* table_section,Elf32_Ehdr header, char* SectionNames)
{
    int i;
    for(i=0; i< header.e_shnum;i++)
    {
        if(table_section[i].sh_offset <= addr && addr <= table_section[i].sh_offset + table_section[i].sh_size)
        {
            printf("%s",SectionNames + table_section[i].sh_name);
        }
    }
}


//affiche une section de relocation
void afficher_sectionR(char *f,Elf32_Shdr* table_section,Elf32_Ehdr header,int numS, char* SectionNames,Str_Reloc RETOUR)
{
	int i;
	unsigned char *section = afficher_section_num(f,header,table_section, numS);
	unsigned long long int addr;
	unsigned long long int info;
    unsigned char type;
    //unsigned char sym;
    
    char *nom_section= getSectionNameBis(SectionNames,table_section[numS]);
      
	printf("\nSection de relocalisation '%s' à l'adresse de décalage %08x contient %i entrées:\n",
            nom_section,
            table_section[numS].sh_offset,
			(int) table_section[numS].sh_size/8);
	printf("  Décalage \t  Info \t\t  Type\t  Val.-sym\t Noms-symboles\n");
	for(i=0; i<(int) table_section[numS].sh_size/8; i++)
	{
		addr = lire_octets_charT(section, header.e_ident[EI_DATA], i*8, 4);
		info = lire_octets_charT(section, header.e_ident[EI_DATA], i*8 +4,4);
        type = info;
        //sym = info >>8;

		printf("%08llx\t%08llx\t",addr,info);
        type_relocation(type);
        printf("\tType\t Valeur_Symbol\t");
        //print_symbol(sym,);
        print_section(addr, table_section, header, SectionNames);  
        //on affiche les infos.
        printf("\n");
        RETOUR.nb_Rel ++;
        RETOUR.Rel = realloc(RETOUR.Rel,sizeof(Elf32_Rela)*(RETOUR.nb_Rel));
        RETOUR.Rel[RETOUR.nb_Rel-1].r_offset = addr;
        RETOUR.Rel[RETOUR.nb_Rel-1].r_info = info;
        RETOUR.Sec_Rel = realloc(RETOUR.Sec_Rel,sizeof(int)*RETOUR.nb_Rel);
        RETOUR.Sec_Rel[RETOUR.nb_Rel-1]=numS;
	}
    printf("\n");
}

//affiche une section de relocation_A
void afficher_sectionRA(char *f,Elf32_Shdr* table_section,Elf32_Ehdr header,int numS, char* SectionNames,Str_Reloc RETOUR)
{
    int i;
    unsigned char *section = afficher_section_num(f,header,table_section, numS);
    unsigned long long int addr;
    unsigned long long int info;
    unsigned long long int addend;
    unsigned char type;
    //unsigned charsym;
    
    char *nom_section= getSectionNameBis(SectionNames,table_section[numS]);
      
   

    printf("\nSection de relocalisation '%s' à l'adresse de décalage %08x contient %i entrées:\n",
            nom_section,
            table_section[numS].sh_offset,
            (int) table_section[numS].sh_size/12);
    printf("  Décalage \t  Info \t\t  Type\t  Val.-sym\t Noms-symboles\n");
    for(i=0; i<(int) table_section[numS].sh_size/12; i++)
    {
        addr = lire_octets_charT(section, header.e_ident[EI_DATA], i*12, 4);
        info = lire_octets_charT(section, header.e_ident[EI_DATA], i*12 +4,4);
        addend = lire_octets_charT(section, header.e_ident[EI_DATA], i*12 +8,4);
        
        type = info;
        //sym = info >>8;

        printf("%08llx\t%08llx\t",addr,info);
        type_relocation(type);
        printf("\tType\t Valeur_Symbol\t");
        print_section(addr, table_section, header, SectionNames);  
        //on affiche les infos.
        printf("\n");
        RETOUR.nb_Rela ++;
        RETOUR.Rela = realloc(RETOUR.Rela,sizeof(Elf32_Rela)*(RETOUR.nb_Rela));
        RETOUR.Rela[RETOUR.nb_Rela-1].r_offset = addr;
        RETOUR.Rela[RETOUR.nb_Rela-1].r_info = info;
        RETOUR.Rela[RETOUR.nb_Rela-1].r_addend = addend;
        RETOUR.Sec_Rela = realloc(RETOUR.Sec_Rela,sizeof(int)*RETOUR.nb_Rela);
        RETOUR.Sec_Rela[RETOUR.nb_Rela-1]=numS;
    }
    printf("\n");
}

// trouve toutes les sections de relocations et les affiche.
Str_Reloc affichage_relocation(char* f,Elf32_Ehdr header,Elf32_Shdr* table_section)
{
    Str_Reloc RETOUR;
    RETOUR.nb_Rel=0;
    RETOUR.nb_Rela=0;
    RETOUR.Rel=NULL;
    RETOUR.Rela=NULL;

	int i=0;
	char* SectionNames;
    char* CurrentSectionName;
    FILE *fichier ;
    fichier = fopen(f,"r");
	SectionNames = fetchSectionNames(fichier,header, table_section);

	while(i<header.e_shnum)
	{
        CurrentSectionName = SectionNames+table_section[i].sh_name;
		// on vérifie toutes les sections
		// si ce sont des sections de relocations:
		if( CurrentSectionName[0]=='.' 
			&& CurrentSectionName[1]=='r' 
			&& CurrentSectionName[2]=='e' 
			&& CurrentSectionName[3]=='l' 
			&& CurrentSectionName[4]=='a' )
		{
            //printf("Nom de la section courante:%s n°%i\n",CurrentSectionName,i);
			afficher_sectionRA(f,table_section,header,i,SectionNames, RETOUR);
		}
		else if (CurrentSectionName[0]=='.' 
            && CurrentSectionName[1]=='r' 
            && CurrentSectionName[2]=='e' 
            && CurrentSectionName[3]=='l' )
		{
            //printf("Nom de la section courante:%s n°%i\n",CurrentSectionName,i);
			afficher_sectionR(f,table_section,header,i,SectionNames, RETOUR);
		}
        i++;
	}
    return RETOUR;
}

			
