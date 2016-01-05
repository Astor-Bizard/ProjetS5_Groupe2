/*
Cree le 5 janvier 2016 par Astor
Affichage d'une section specifique
*/
#include "afficher_section.h"
#include "lectureSH.h"

// Retourne le numéro de la section demandée, par son nom ou son numéro, -1 si invalide.
int index_Shdr(char str[], FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH){
	int i,num_sh;
	char **names;
	if(str[0]!='\0'){
		// Cas nombre : on traduit le nombre (string) en int
		if(str[0]>=48 && str[0]<=57){
			num_sh = str[0]-48;
			for(i=1;str[i]!='\0';i++){
				if(str[i]>=48 && str[i]<=57) num_sh = num_sh*10 + str[i]-48;
			}
		}
		// Cas nom : on le cherche dans la table str
		else{
			names=getSectionsNames(f,elfHeader,tabSH);
			num_sh=0;
			while(num_sh<elfHeader.e_shnum && strcmp(str,names[num_sh]))num_sh++;
		}
		return num_sh;
	}
	else return -1;
}

// Affiche le contenu d'une section désignée par nom ou numéro. Renvoie ce contenu, NULL si la section n'existe pas. La libération est à la charge de l'utilisateur.
unsigned char *afficher_section(char *nom_f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH){
	FILE *f;
	unsigned char str[42];
	int num_sh=0,i;
	unsigned char c;
	unsigned char *section;

	printf("Section à afficher : ");
	scanf("%s",str);

	f=fopen(nom_f,"r");
	// On traduit la demande (string) en index dans la table
	num_sh=index_Shdr(str,f,elfHeader,tabSH);
	fclose(f);

	if(num_sh<0 || num_sh>=elfHeader.e_shnum){
		printf("Section absente : %s\n", str);
		return NULL;
	}
	else{
		f=fopen(nom_f,"r");
		printf("Section %s\n",str);
		// On se place
		fseek(f,tabSH[num_sh].sh_offset,0);
		section=malloc(sizeof(unsigned char)*(tabSH[num_sh].sh_size+1));
		if(section != NULL){
			// On affiche le contenu de la section
			for(i=0;i<tabSH[num_sh].sh_size;i++){
				if(i!=0 && i%8==0) printf(" ");
				c=fgetc(f);
				printf("%x",c);
				section[i]=c;
			}
			section[i]='\0';
			fclose(f);
		}
		printf("\n");
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
		printf("Section absente : %d\n", num_sh);
		return NULL;
	}
	else{
		f=fopen(nom_f,"r");
		printf("Section %d\n",num_sh);
		// On se place
		fseek(f,tabSH[num_sh].sh_offset,0);
		section=malloc(sizeof(unsigned char)*(tabSH[num_sh].sh_size+1));
		if(section != NULL){
			// On affiche le contenu de la section
			for(i=0;i<tabSH[num_sh].sh_size;i++){
				if(i!=0 && i%8==0) printf(" ");
				c=fgetc(f);
				printf("%x",c);
				section[i]=c;
			}
			section[i]='\0';
			fclose(f);
		}
		printf("\n");
	}
	return section;
}
