/*Creer le 6/01/2015 par Jonathan
Renumerote les section dans un ELF et la table des réimplantations
*/

#include "renum_section.h"

/*

ON REECRIE LE FICHIER


*/
/* Copie une chaine de 32 OCTETS dans une chaine source plus grande à partir de id_dest
Fait pas le con Jimmy
*/
void CopieOctet(unsigned char *dest,Elf32_Word *src, Elf32_Addr id_dest)
{
	Elf32_Addr i;
	for(i=0; i<32;i++)
	{
		dest[i+id_dest] = src[i];
	}
}


void renumerote_section(FILE *f_read, 
						FILE *f_write,
						Elf32_Ehdr elfHeaders, 
						Elf32_Shdr *section_headers, 
						ListeSymboles sym_tab,
						Str_Reloc str_reloc,
						Elf32_Ehdr *elfHeaders_mod, 
						Elf32_Shdr *section_headers_mod
						)
{
	int i,k;
	//int premier = 1;
	int nbRel = 0; 
	int nb_Sec_A_Traiter = 0;
	//int id_Sec_Cour = 0;
	Table_Donnees tab_donnees;
	//unsigned char *sec_Cour;
	Elf32_Word OctetSupp = 0;


	//Valeur de test a changer
	tab_donnees.nbSecRel = 2;
	tab_donnees.table_Addr = malloc(sizeof(Elf32_Addr)*tab_donnees.nbSecRel);
	if (tab_donnees.table_Addr==NULL) {
		printf("\nErreur lors de l'allocation initiale de tab_donnees.table_Addr.\n");
		exit(1);
	}
	tab_donnees.table_Nom_Addr = malloc(sizeof(Elf32_Word) * tab_donnees.nbSecRel); 
	if (tab_donnees.table_Nom_Addr==NULL) {
		printf("\nErreur lors de l'allocation initiale de tab_donnees.table_Nom_Addr.\n");
		exit(1);
	}

	//tab_donnees.table_Nom_Addr[0] = ;
	//.text = 0x58
	tab_donnees.table_Nom_Addr[0] = 0x1f;
	//.data = 0x1000
	tab_donnees.table_Nom_Addr[1] = 0x29;

	for(i=0;i<elfHeaders.e_shnum;i++)
	{
		printf("Nom : %06x\n",section_headers[i].sh_offset);
	}

	tab_donnees.table_Addr[0]=0x58;
	tab_donnees.table_Addr[1]=0x1000;

	printf("ICI\n");
	//Modification du Headers

	nb_Sec_A_Traiter = nbSecRel(elfHeaders,section_headers);
	printf("LA\n");
	elfHeaders_mod->e_shnum = elfHeaders.e_shnum - nb_Sec_A_Traiter;
	
	section_headers_mod = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr)*elfHeaders.e_shnum);
	if (section_headers_mod==NULL) {
		printf("\nErreur lors de l'allocation initiale de section_headers_mod.\n");
		exit(1);
	}

	//Modification table des sections
	printf("Début\n");
	for(i=0;i<elfHeaders.e_shnum;i++)
	{
		printf("Nom : %06x\n",section_headers[i].sh_name);
		if(section_headers[i].sh_type == SHT_REL)
		{
			printf("ICI\n");
			nbRel++;
			OctetSupp += section_headers[i].sh_size;
			k=0;
			while(tab_donnees.table_Nom_Addr[k] != section_headers[i-1].sh_name && k < elfHeaders.e_shnum)
			{
				k++;
			}
			if(k == elfHeaders.e_shnum)
			{
				printf("Table (%x) non trouvé , erreur d'argument\n",section_headers[i-1].sh_name);
				exit(1);
			}
			else
			{
				section_headers_mod[i-1].sh_addr = tab_donnees.table_Nom_Addr[k];
			}
/*		}
		while(str_reloc.Sec_Rel[j] == i )
		{
			str_reloc.Sec_Rel[j] -= nbRel;
			j++;
		}*/
		}
		else
		{
			section_headers_mod[i] = section_headers[i];
		}	
				
	}

	elfHeaders_mod->e_shoff -= (Elf32_Off) OctetSupp;

	fwrite(elfHeaders_mod,sizeof(Elf32_Ehdr),1,f_write);


	// Ecriture de la nouvelle section
/*
	printf("Boucle\n");
	printf("str_reloc.nb_Rel : %i\n", str_reloc.nb_Rel);
	for(i=0;i<str_reloc.nb_Rel;i++)
	{
		printf("Debut : %i\n",i);

		if(id_Sec_Cour != str_reloc.Sec_Rel[i])
		{
			// On appelle ce que fait la partie 8
			//On ecrit dans le fichier
			for(j=id_Sec_Cour;j<= str_reloc.Sec_Rel[i]; j++)
			{
				sec_Cour = recuperer_section_num(f_read,*elfHeaders, section_headers, j);
				fwrite(sec_Cour,section_headers[j].sh_size,1,f_write);
			}

			if(!premier)
			{
				

				printf("On ecrit taille : %x\n",section_headers[id_Sec_Cour].sh_size);
				fwrite(sec_Cour,section_headers[id_Sec_Cour].sh_size,1,f_write);
			}
			
			
			rewind(f_read);
			sec_Cour = recuperer_section_num(f_read,*elfHeaders, section_headers, str_reloc.Sec_Rel[i]);
			id_Sec_Cour = str_reloc.Sec_Rel[i];
			printf("id_Sec_Cour : %i\n", id_Sec_Cour);
			premier=0;
			
		}

		CopieOctet(sec_Cour,&str_reloc.Rel[i].r_info,str_reloc.Rel[i].r_offset);
		printf("Fin : %i\n",i);
	}
	// On appelle ce que fait la partie 8
	
	//On ecrit dans le fichier
	fwrite(sec_Cour,section_headers[id_Sec_Cour].sh_size,1,f_write);
		
	for(j=id_Sec_Cour+1;j<elfHeaders.e_shnum;j++)
	{
		sec_Cour = recuperer_section_num(f_read,*elfHeaders, section_headers, j);
		fwrite(sec_Cour,section_headers[j].sh_size,1,f_write);
	}
*/

}


int nbSecRel(Elf32_Ehdr elfHeaders, Elf32_Shdr *section_headers)
{
	
	int i;
	int retour = 0;	
	for(i=0; i<elfHeaders.e_shnum; i++)
	{
		if(section_headers[i].sh_type == SHT_REL)
		{
			retour++;
		}
	}
	return retour;

}