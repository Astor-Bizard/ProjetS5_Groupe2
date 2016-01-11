//05/01/2016 Gabriel

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "lectureSH.h"
#include "lecture_headers.h"
#include "afficher_section.h"
#include "affichage_relocation.h"



long long unsigned int lire_octets_charT(unsigned char *tableau, 
    int hdr_mode, 
    int debut, 
    int nombre)
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
            printf("R_ARM_NONE        ");
            break;
        case 1:
            printf("R_ARM_PC24        ");
            break;
        case 2:
            printf("R_ARM_ABS32       ");
            break;
        case 3:
            printf("R_ARM_REL32       ");
            break;
        case 4:
            printf("R_ARM_LDR_PC_G0   ");
            break;
        case 5:
            printf("R_ARM_ABS16       ");
            break;
        case 6:
            printf("R_ARM_ABS12       ");
            break;
        case 7:
            printf("R_ARM_THM_ABS5    ");
            break;
        case 8:
            printf("R_ARM_ABS8        ");
            break;
        case 9:
            printf("R_ARM_SBREL32     ");
            break;
        case 10:
            printf("R_ARM_THM_CALL    ");
            break;
        case 11:
            printf("R_ARM_THM_PC8     ");
            break;
        case 12:
            printf("R_ARM_BREL_ADJ    ");
            break;
        case 13:
            printf("R_ARM_TLS_DESC    ");
            break;
        case 14:
            printf("R_ARM_THM_SWI8    ");
            break;    
        case 15:
            printf("R_ARM_XPC25       ");
            break;
        case 16:
            printf("R_ARM_THM_XPC22   ");
            break;
        case 17:
            printf("R_ARM_DTPMOD32    ");
            break;
        case 18:
            printf("R_ARM_TLS_DTPOFF3 ");
            break;
        case 19:
            printf("R_ARM_TLS_TPOFF32 ");
            break;
        case 20:
            printf("R_ARM_COPY        ");
            break;
        case 21:
            printf("R_ARM_GLOB_DAT    ");
            break;
        case 22:
            printf("R_ARM_JUMP_SLOT   ");
            break;
        case 23:
            printf("R_ARM_RELATIVE    ");
            break;
        case 24:
            printf("R_ARM_GOTOFF32    ");
            break;
        case 25:
            printf("R_ARM_BASE_PREL   ");
            break;
        case 26:
            printf("R_ARM_GOT_PREL    ");
            break;
        case 27:
            printf("R_ARM_PLT32       ");
            break;
        case 28:
            printf("R_ARM_CALL        ");
            break;
        case 29:
            printf("R_ARM_JUMP24      ");
            break;
        case 30:
            printf("R_ARM_THM_JUMP24  ");
            break;
        default:
            printf("Case: default     ");
    }
}


void print_section(unsigned long long int addr, Elf32_Shdr* table_section, Elf32_Ehdr header, char* SectionNames)
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

void print_symbol(int sym, ListeSymboles table_symbol,Elf32_Ehdr header, Elf32_Shdr* table_section, char* SymbolNames, char* SectionNames)
{
    //printf("\t[%i]",sym);
    printf("%08x   ",table_symbol.symboles[sym].st_value);
    //printf("\t%s",typeSymbole(table_symbol.symboles[sym].st_info & 0xf));
    //char to_show[42];
    if((table_symbol.symboles[sym].st_info & 0xf) != 3)
    {
        printf("%s",getSymbolNameBis(SymbolNames, table_symbol.symboles[sym]));
    }
    else
    {
        printf("%s",getSectionNameBis(SectionNames, table_section[table_symbol.symboles[sym].st_shndx]));
    }
    //printf("%s",to_show);
}

//affiche une section de relocation
void afficher_sectionR(FILE *f,
                        Elf32_Shdr* table_section,
                        Elf32_Ehdr header,
                        int numS, 
                        char* SectionNames,Str_Reloc *RETOUR, 
                        ListeSymboles table_symbol, 
                        char* SymbolNames,
                        int silent)
{
	int i;
	unsigned char *section;
    section = recuperer_section_num(f,header,table_section, numS); // Y'a rien a voir circulez!
	unsigned long long int addr;
	unsigned long long int info;
    unsigned char type;
    unsigned int sym;
    
    char *nom_section= getSectionNameBis(SectionNames,table_section[numS]);
    if(!silent)
    {
    	printf("\nRelocation section '%s' at offset 0x%x contains %i entries:\n",
                nom_section,
                table_section[numS].sh_offset,
    			(int) table_section[numS].sh_size/8);
    	printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
    }

    RETOUR->Rel = realloc(RETOUR->Rel,sizeof(Elf32_Rel)*(RETOUR->nb_Rel+(int) table_section[numS].sh_size/8));
    RETOUR->Sec_Rel = realloc(RETOUR->Sec_Rel,sizeof(int)*(RETOUR->nb_Rel+(int) table_section[numS].sh_size/8));

	for(i=0; i<(int) table_section[numS].sh_size/8; i++)
	{
		addr = lire_octets_charT(section, header.e_ident[EI_DATA], i*8, 4);
		info = lire_octets_charT(section, header.e_ident[EI_DATA], i*8 +4,4);
        type = info;
        sym = info >>8;

        if(!silent)
        {
    		printf("%08llx  %08llx ",addr,info);
            type_relocation(type);
            print_symbol(sym,table_symbol,header,table_section,SymbolNames, SectionNames);
            //print_section(addr, table_section, header, SectionNames); 
            
            //on affiche les infos.
            sym = sym *2;
            printf("\n");
        }
        RETOUR->nb_Rel ++;

        
        RETOUR->Rel[RETOUR->nb_Rel-1].r_offset = addr;
        RETOUR->Rel[RETOUR->nb_Rel-1].r_info = info;
        RETOUR->Sec_Rel[RETOUR->nb_Rel-1]=numS;
	}
}

// trouve toutes les sections de relocations et les affiche.
Str_Reloc affichage_relocation(FILE* f,
    Elf32_Ehdr header,
    Elf32_Shdr* table_section, 
    ListeSymboles table_symbol,
    int silent)
{
    Str_Reloc RETOUR;
    RETOUR.nb_Rel=0;
    RETOUR.Rel=NULL;
    RETOUR.Sec_Rel=NULL;

	int i=0,ok=0;
	char* SectionNames;
    char* SymbolNames;
    char* CurrentSectionName;
    int Symbol_tab_section_number=0;
	SectionNames = fetchSectionNames(f,header, table_section);

    while(i<header.e_shnum)
    {
        CurrentSectionName = getSectionNameBis(SectionNames,table_section[i]);
        // on vérifie toutes les sections
        // si ce sont des sections de relocations:
        if( !strcmp(".symtab",CurrentSectionName))
        {
            Symbol_tab_section_number=i;
            i=header.e_shnum;
        }
        i++;
    }
    SymbolNames = fetchSymbolNames(f, table_section,Symbol_tab_section_number);

    i=0;
	while(i<header.e_shnum)
	{
        CurrentSectionName = SectionNames+table_section[i].sh_name;
		// on vérifie toutes les sections
		// si ce sont des sections de relocations:
		/*if( CurrentSectionName[0]=='.' 
			&& CurrentSectionName[1]=='r' 
			&& CurrentSectionName[2]=='e' 
			&& CurrentSectionName[3]=='l' 
			&& CurrentSectionName[4]=='a' )
		{
            //printf("Nom de la section courante:%s n°%i\n",CurrentSectionName,i);
			//afficher_sectionRA(f,table_section,header,i,SectionNames, &RETOUR, table_symbol, SymbolNames);
		}
		else */if (CurrentSectionName[0]=='.' 
            && CurrentSectionName[1]=='r' 
            && CurrentSectionName[2]=='e' 
            && CurrentSectionName[3]=='l' )
		{
            //printf("Nom de la section courante:%s n°%i\n",CurrentSectionName,i);
			afficher_sectionR(f,table_section,header,i,SectionNames, &RETOUR, table_symbol, SymbolNames, silent);
			ok=1;
		}
        i++;
	}
	if(!ok && !silent) printf("\nThere are no relocations in this file.\n");
    return RETOUR;
}

			
