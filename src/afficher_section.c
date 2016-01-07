/*
Cree le 5 janvier 2016 par Astor
Affichage d'une section specifique
*/
#include "afficher_section.h"
#include "lectureSH.h"

#define ASCII_0 48

// Retourne le numéro de la section demandée, par son nom ou son numéro, -1 si invalide.
int index_Shdr(char str[], FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH){
	int i,num_sh;
	char *names;
	names = fetchSectionNames(f,elfHeader,tabSH);
	if(str[0]!='\0'){
		// Cas nombre : on traduit le nombre (string) en int
		if(str[0]>=ASCII_0 && str[0]<=ASCII_0+9){
			num_sh = str[0]-ASCII_0;
			for(i=1;str[i]!='\0';i++){
				if(str[i]>=ASCII_0 && str[i]<=ASCII_0+9) num_sh = num_sh*10 + str[i]-ASCII_0;
			}
			if(num_sh>=0 && num_sh<elfHeader.e_shnum) strcpy(str,getSectionNameBis(names,tabSH[num_sh]));
			else return -1;
		}
		// Cas nom : on le cherche dans la table str
		else{
			num_sh=0;
			while(num_sh<elfHeader.e_shnum && strcmp(str,getSectionNameBis(names,tabSH[num_sh]))) num_sh++;
		}
		return num_sh;
	}
	else return -1;
}

// Affiche le contenu d'une section désignée par nom ou numéro. Renvoie ce contenu, NULL si la section n'existe pas. La libération est à la charge de l'utilisateur.
unsigned char *afficher_section(char *nom_f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH, int renvoi){
	FILE *f;
	char str[42];
	int num_sh=0,i;
	unsigned char c;
	unsigned char *section;
	section=NULL;

	scanf("%s",str);
	printf("\n");

	f=fopen(nom_f,"r");
	// On traduit la demande (string) en index dans la table
	num_sh=index_Shdr(str,f,elfHeader,tabSH);
	fclose(f);

	if(num_sh<0 || num_sh>=elfHeader.e_shnum){
		printf("Section absente !\n\n");
		return NULL;
	}
	else{
		f=fopen(nom_f,"r");
		printf("Hex dump of section '%s':\n",str);
		// On se place
		fseek(f,tabSH[num_sh].sh_offset,0);
		if(renvoi) section=malloc(sizeof(unsigned char)*(tabSH[num_sh].sh_size+1));
		if(section != NULL || !renvoi){
			// On affiche le contenu de la section
			if(tabSH[num_sh].sh_size==0){
				printf("Section '%s' has no data to dump.",str);
				if(renvoi) section[0]='\0';
			}
			else{
				printf("  0x%08x ",0);
				for(i=0;i<tabSH[num_sh].sh_size;i++){
					if(i!=0){
						if(i%16==0) printf("\n  0x%08x ",i);
						else if(i%4==0) printf(" ");
					}
					c=fgetc(f);
					printf("%02x",c);
					if(renvoi) section[i]=c;
				}
				if(renvoi) section[i]='\0';
			}
			fclose(f);
			printf("\n\n");
		}
		else{
			printf("Erreur d'allocation\n\n");
			exit(42);
		}
	}
	return section;
}

// Affiche le contenu de la section numero num_sh. Renvoie ce contenu, NULL si la section n'existe pas. La libération est à la charge de l'utilisateur.
unsigned char *afficher_section_num(char *nom_f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH, int num_sh){
	FILE *f;
	int i;
	unsigned char c;
	unsigned char *section;

	if(num_sh<0 || num_sh>=elfHeader.e_shnum){
		return NULL;
	}
	else{
		f=fopen(nom_f,"r");
		// On se place
		fseek(f,tabSH[num_sh].sh_offset,0);
		section=malloc(sizeof(unsigned char)*(tabSH[num_sh].sh_size+1));
		if(section != NULL){
			// On affiche le contenu de la section
			for(i=0;i<tabSH[num_sh].sh_size;i++){
				c=fgetc(f);
				section[i]=c;
			}
			section[i]='\0';
			fclose(f);
		}
		else{
			printf("Erreur d'allocation\n\n");
			exit(42);
		}
	}
	return section;
}
