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


void renumerote_section(FILE *f_read, FILE *f_write,Elf32_Ehdr *elfHeaders, Elf32_Shdr *section_headers, ListeSymboles sym_tab,Str_Reloc str_reloc)
{
	int i;
	int nbRel = 0; 
	int nb_Sec_A_Traiter = 0;
	int id_Sec_Cour = 0;
	unsigned char *sec_Cour;
	//Modification du Headers
	
	nb_Sec_A_Traiter = nbSecRel(elfHeaders,section_headers);
	elfHeaders->e_shnum = elfHeaders->e_shnum - nb_Sec_A_Traiter;

	//Modification table des symboles

	for(i=0;i<elfHeaders->e_shnum;i++)
	{
		if(section_headers[i].sh_type == SHT_REL)
		{
			nbRel++;
		}
		section_headers[i] = section_headers[i+nbRel];
	}


	fwrite(elfHeaders,sizeof(elfHeaders),1,f_write);

	// Ecriture de la nouvelle section
	if(str_reloc.nb_Rel>0)
	{
		sec_Cour = recuperer_section_num(f_read,*elfHeaders, section_headers, str_reloc.Sec_Rel[i]);
		id_Sec_Cour = str_reloc.Sec_Rel[i];
	}
	

	for(i=0;i<str_reloc.nb_Rel;i++)
	{
		if(id_Sec_Cour != str_reloc.Sec_Rel[i])
		{
			// On appelle ce que fait la partie 8

			//On ecrit dans le fichier
			fwrite(sec_Cour,sizeof(sec_Cour),1,f_write);
			free(sec_Cour);
			sec_Cour = recuperer_section_num(f_read,*elfHeaders, section_headers, str_reloc.Sec_Rel[i]);
			id_Sec_Cour = str_reloc.Sec_Rel[i];
			
		}

		CopieOctet(sec_Cour,&str_reloc.Rel[i].r_info,str_reloc.Rel[i].r_offset);


	}
	// On appelle ce que fait la partie 8

	//On ecrit dans le fichier
	fwrite(sec_Cour,sizeof(sec_Cour),1,f_write);
	free(sec_Cour);
}


int nbSecRel(Elf32_Ehdr *elfHeaders, Elf32_Shdr *section_headers)
{
	
	int i;
	int retour = 0;	
	for(i=0; i<elfHeaders->e_shnum; i++)
	{
		if(section_headers->sh_type == SHT_REL)
		{
			retour++;
		}
	}
	return retour;

}