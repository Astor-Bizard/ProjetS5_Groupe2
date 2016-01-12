//05/01/2016 Gabriel

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "lectureSH.h"
#include "lecture_headers.h"
#include "afficher_section.h"
#include "affichage_relocation.h"

// lit une suite d'octet selon le mode dans un tableau d'octet
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


// afficher le type de relocation
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

// affiche le nom d'une section qui commence à l'addresse addr
void print_section(unsigned long long int addr, SectionsHeadersList liste_sections, Elf32_Ehdr header)
{
    int i;
    for(i=0; i< header.e_shnum;i++)
    {
        if(liste_sections.headers[i].sh_offset <= addr && addr <= liste_sections.headers[i].sh_offset + liste_sections.headers[i].sh_size)
        {
            printf("%s", liste_sections.names + liste_sections.headers[i].sh_name);
        }
    }
}

// affiche la valeur et le nom d'un symbol (ou la section correspondante si le symbol est une section)
void print_symbol(int sym, ListeSymboles table_symbol, Elf32_Ehdr header, SectionsHeadersList liste_sections)
{
    printf("%08x   ", table_symbol.symboles[sym].st_value);
		char *name;
    if((table_symbol.symboles[sym].st_info & 0xf) != 3)
    {
	   name = getSymbolNameBis(table_symbol.names, table_symbol.symboles[sym]);
    }
    else
    {
		name = getSectionNameBis(liste_sections.names, liste_sections.headers[table_symbol.symboles[sym].st_shndx]);
    }
	printf("%s",name);
	free(name);
}

//affiche une section de relocation
void afficher_sectionR(FILE *f,
                        SectionsHeadersList liste_sections,
                        Elf32_Ehdr header,
                        int numS, 
                        Str_Reloc *RETOUR, 
                        ListeSymboles table_symbol, 
                        int silent)
{
	int i;
	unsigned char *section;
    section = recuperer_section_num(f, header, liste_sections, numS); // Y'a rien a voir circulez!
	unsigned long long int addr;
	unsigned long long int info;
    unsigned char type;
    unsigned int sym;
    
    char *nom_section = getSectionNameBis(liste_sections.names, liste_sections.headers[numS]);
    if(!silent)
    {
    	printf("\nRelocation section '%s' at offset 0x%x contains %i entries:\n",
                nom_section,
                liste_sections.headers[numS].sh_offset,
    			(int) liste_sections.headers[numS].sh_size/8);
    	printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
    }
    free(nom_section);

    RETOUR->Rel = realloc(RETOUR->Rel,sizeof(Elf32_Rel)*(RETOUR->nb_Rel+(int) liste_sections.headers[numS].sh_size/8));
    RETOUR->Sec_Rel = realloc(RETOUR->Sec_Rel,sizeof(int)*(RETOUR->nb_Rel+(int) liste_sections.headers[numS].sh_size/8));

	for(i=0; i<(int) liste_sections.headers[numS].sh_size/8; i++)
	{
		addr = lire_octets_charT(section, header.e_ident[EI_DATA], i*8, 4);
		info = lire_octets_charT(section, header.e_ident[EI_DATA], i*8 +4,4);
        type = info;
        sym = info >>8;

        if(!silent)
        {
    		printf("%08llx  %08llx ", addr,info);
            type_relocation(type);
            print_symbol(sym, table_symbol, header, liste_sections);
            //print_section(addr, liste_sections, header); 
            
            //on affiche les infos.
            sym = sym *2;
            printf("\n");
        }
        RETOUR->nb_Rel ++;

        
        RETOUR->Rel[RETOUR->nb_Rel-1].r_offset = addr;
        RETOUR->Rel[RETOUR->nb_Rel-1].r_info = info;
        RETOUR->Sec_Rel[RETOUR->nb_Rel-1] = numS;
	}
	free(section);
}

// trouve toutes les sections de relocations et les affiche (ou pas).
Str_Reloc affichage_relocation(FILE* f,
    Elf32_Ehdr header,
    SectionsHeadersList liste_sections, 
    ListeSymboles table_symbol,
    int silent)
{
    // initialisation
    Str_Reloc RETOUR;
    RETOUR.nb_Rel=0;
    RETOUR.Rel=NULL;
    RETOUR.Sec_Rel=NULL;

	int i=0,ok=0;
    char* CurrentSectionName;

    // on parcours les titres des sections pour trouver les sections de reallocation.
    i=0;
	while(i<header.e_shnum)
	{
        CurrentSectionName = liste_sections.names + liste_sections.headers[i].sh_name;
		// on vérifie toutes les sections
		// si ce sont des sections de relocations:
		/*if( CurrentSectionName[0]=='.' 
			&& CurrentSectionName[1]=='r' 
			&& CurrentSectionName[2]=='e' 
			&& CurrentSectionName[3]=='l' 
			&& CurrentSectionName[4]=='a' )
		{
            //printf("Nom de la section courante:%s n°%i\n",CurrentSectionName,i);
			//afficher_sectionRA(f, liste_sections, header, i, &RETOUR, table_symbol);
		}
		else */if (CurrentSectionName[0]=='.' 
            && CurrentSectionName[1]=='r' 
            && CurrentSectionName[2]=='e' 
            && CurrentSectionName[3]=='l' )
		{
            //printf("Nom de la section courante:%s n°%i\n", CurrentSectionName, i);
			afficher_sectionR(f, liste_sections, header, i, &RETOUR, table_symbol, silent);
			ok = 1;
		}
        i++;
	}
	if(!ok && !silent) printf("\nThere are no relocations in this file.\n");
    return RETOUR;
}

			
