/*Creer le 6/01/2015 par Jonathan
Renumerote les section dans un ELF et la table des réimplantations
*/

#include "renum_section.h"

/*

ON REECRIE LE FICHIER


*/
/* Copie une chaine de 32 OCTETS dans une chaine source plus grande à partir de id_dest
*/
void CopieOctet(unsigned char *dest,Elf32_Word *src, Elf32_Addr id_dest)
{
	Elf32_Addr i;
	for(i=0; i<32;i++)
	{
		dest[i+id_dest] = src[i];
	}
}


SectionsHeadersList renumerote_section(FILE *f_read, 
						FILE *f_write,
						Elf32_Ehdr elfHeaders, 
						SectionsHeadersList section_headers, 
						Elf32_Ehdr *elfHeaders_mod,
						Table_Donnees tab_donnees
						)
{
	int i,k; 
	int nbRec, nbSecVide;
	int nb_Sec_A_Traiter = 0;
	Elf32_Word OctetSupp = 0;
	SectionsHeadersList section_headers_mod;
	
	//Modification du Headers
	nb_Sec_A_Traiter = nbSecRel(elfHeaders,section_headers.headers);
	
	*elfHeaders_mod=elfHeaders;

	elfHeaders_mod->e_shnum = elfHeaders.e_shnum - nb_Sec_A_Traiter;

	for(i=0;i<tab_donnees.nbSecRel;i++){
		if(tab_donnees.table_Num_Addr[i]<elfHeaders.e_shstrndx){
			elfHeaders_mod->e_shstrndx--;
		}
	}

	
	section_headers_mod.headers = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr)*elfHeaders.e_shnum);
	if (section_headers_mod.headers==NULL) {
		printf("\nErreur lors de l'allocation initiale de section_headers_mod.headers.\n");
		exit(1);
	}



	//Modification table des sections

	//Copie (sauf REL)
	nbRec=0;
	for(i=0;i<elfHeaders.e_shnum;i++)
	{
		if(section_headers.headers[i].sh_type != SHT_REL)
		{
			section_headers_mod.headers[i-nbRec] = section_headers.headers[i];
		}
		else nbRec++;
	}

	//Application des modifications de relocation et
	nbRec=0;
	for(i=0;i<elfHeaders.e_shnum;i++)
	{
		if(section_headers.headers[i].sh_type == SHT_REL)
		{
			OctetSupp += section_headers.headers[i].sh_size;
			nbRec++;
			k= 0;
			while(k < tab_donnees.nbSecRel && tab_donnees.table_Num_Addr[k] !=  section_headers.headers[i].sh_info)
			{
				k++;
			}
			if(k == tab_donnees.nbSecRel)
			{
				printf("Table (%i) non trouvée , erreur d'argument\n",section_headers.headers[i].sh_info);
				exit(1);
			}
			else
			{
				section_headers_mod.headers[i-nbRec].sh_addr = tab_donnees.table_Addr[k];
			}
		}
	}
	// Decalage des offset et suppresion des sections vide 
	nbSecVide = 0;
	for(i=1;i<elfHeaders_mod->e_shnum;i++)
	{
		if(section_headers_mod.headers[i].sh_addr == 0)
		{
			if(section_headers_mod.headers[i].sh_type == SHT_NOBITS)
			{
				nbSecVide ++;
			}
			else
			{
				section_headers_mod.headers[i-nbSecVide] = section_headers_mod.headers[i];
				section_headers_mod.headers[i-nbSecVide].sh_offset = section_headers_mod.headers[i-nbSecVide-1].sh_offset + section_headers_mod.headers[i-nbSecVide-1].sh_size; 

				
			}
		}
		else
		{
			section_headers_mod.headers[i].sh_offset = section_headers_mod.headers[i].sh_addr; 
		}
	}
	// on remet à jour la taille de la table des sections
	
	elfHeaders_mod->e_shnum -= nbSecVide;
	elfHeaders_mod->e_shoff -=  OctetSupp;


	// copie et maj des anciens valeurs de la struture section_headers 
	section_headers_mod.size = elfHeaders_mod->e_shnum;
	
	section_headers_mod.names = malloc(sizeof(char)*section_headers.headers[elfHeaders.e_shstrndx].sh_size);
	if (section_headers_mod.names==NULL) {
		printf("\nErreur lors de l'allocation initiale de section_headers_mod.headers.\n");
		exit(1);
	}
	for(i=0;i<section_headers.headers[elfHeaders.e_shstrndx].sh_size;i++)
	{
		section_headers_mod.names[i] = section_headers.names[i];
	}
	

	//fwrite(elfHeaders_mod,sizeof(Elf32_Ehdr),1,f_write);
	//fwrite(section_headers_mod.headers,sizeof(Elf32_Shdr),elfHeaders_mod->e_shnum,f_write);

	return section_headers_mod;
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
