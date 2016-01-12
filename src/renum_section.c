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


Elf32_Shdr *renumerote_section(FILE *f_read, 
						FILE *f_write,
						Elf32_Ehdr elfHeaders, 
						Elf32_Shdr *section_headers, 
						Elf32_Ehdr *elfHeaders_mod,
						Table_Donnees tab_donnees
						)
{
	int i,k; 
	int nbRec;
	int nb_Sec_A_Traiter = 0;
	Elf32_Word OctetSupp = 0;
	Elf32_Shdr *section_headers_mod;
	
	//Modification du Headers
	nb_Sec_A_Traiter = nbSecRel(elfHeaders,section_headers);
	
	*elfHeaders_mod=elfHeaders;

	elfHeaders_mod->e_shnum = elfHeaders.e_shnum - nb_Sec_A_Traiter;

	for(i=0;i<tab_donnees.nbSecRel;i++){
		if(tab_donnees.table_Num_Addr[i]<elfHeaders.e_shstrndx){
			elfHeaders_mod->e_shstrndx--;
		}
	}
	
	section_headers_mod = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr)*elfHeaders.e_shnum);
	if (section_headers_mod==NULL) {
		printf("\nErreur lors de l'allocation initiale de section_headers_mod.\n");
		exit(1);
	}



	//Modification table des sections

	//Copie (sauf REL)
	nbRec=0;
	for(i=0;i<elfHeaders.e_shnum;i++)
	{
		if(section_headers[i].sh_type != SHT_REL)
		{
			section_headers_mod[i-nbRec] = section_headers[i];
		}
		else nbRec++;
	}

	nbRec=0;
	//Application des modifications de relocation
	for(i=0;i<elfHeaders.e_shnum;i++)
	{
		if(section_headers[i].sh_type == SHT_REL)
		{
			OctetSupp += section_headers[i].sh_size;
			nbRec++;
			k= 0;
			while(k < tab_donnees.nbSecRel && tab_donnees.table_Num_Addr[k] != i)
			{
				k++;
			}
			if(k == tab_donnees.nbSecRel)
			{
				printf("Table (%i) non trouvée , erreur d'argument\n",i);
				exit(1);
			}
			else
			{
				section_headers_mod[i-nbRec].sh_addr += tab_donnees.table_Addr[k]+section_headers_mod[i-nbRec].sh_offset;
			}
		}
	}

	elfHeaders_mod->e_shoff -=  OctetSupp;

	fwrite(&elfHeaders,sizeof(Elf32_Ehdr),1,f_write);

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
