/*
Cree le 5 janvier 2016 par Astor
Affichage d'une section specifique
*/
#include "afficher_section.h"

#define ASCII_0 48

int is_digit(char c){
	return (c>=ASCII_0 && c<=ASCII_0+9);
}

int is_number(char str[]){
	int i=0;
	while(is_digit(str[i]))i++;
	return str[i]=='\0';
}

void afficher_string(unsigned char *section, int size){
	int i,j;
	unsigned char aff[17];
	for(j=0;j<17;j++)aff[j]='\0';
	printf("  0x%08x ",0);
	i=0;
	// On affiche le contenu de la section
	while(i<size){
		// Affichage de l'offset
		if(i!=0){
			if(i%16==0){
				printf(" %s\n  0x%08x ",aff,i);	// Affichage non hexa
				for(j=0;j<17;j++)aff[j]='\0';
			}
			else if(i%4==0) printf(" ");
		}

		// Affichage hexa
		printf("%02x",section[i]);

		// Memorisation du caractère pour écriture non hexa
		aff[i%16]=section[i];
		if(aff[i%16]<32 || aff[i%16]>126) aff[i%16]='.';
		i++;
	}

	// Terminaison de l'affichage
	while(i%16!=0){
		if(i%4==0) printf(" ");
		printf("  ");
		i++;
	}
	printf(" %s\n\n",aff);
}

// Retourne le numéro de la section demandée, par son nom ou son numéro, -1 si invalide.
int index_Shdr(char str[], SectionsHeadersList shList) {
	int i,num_sh;
	char *name;
	int different;
	if(str[0]!='\0'){
		// Cas nombre : on traduit le nombre (string) en int
		i=0;
		num_sh=0;
		while(is_digit(str[i])){
			num_sh = num_sh*10 + str[i]-ASCII_0;
			i++;
		}
		if(str[i]=='\0'){
			if(num_sh<shList.size){
				name=getSectionNameBis(shList.names, shList.headers[num_sh]);
				strcpy(str, name);
				free(name);
			}
			else num_sh=-1;
		}
		// Cas nom : on le cherche dans la table shstrtab
		else{
			num_sh=-1;
			different=1;
			while(num_sh<shList.size-1 && different){
				num_sh++;
				name=getSectionNameBis(shList.names, shList.headers[num_sh]);
				different=strcmp(str, name);
				free(name);
			}
			if(different) num_sh=-1;
		}
		return num_sh;
	}
	else return -1;
}

// Affiche le contenu d'une section désignée par nom ou numéro. Renvoie ce contenu, NULL si la section n'existe pas. La libération est à la charge de l'utilisateur.
unsigned char *afficher_section(FILE *f, SectionsHeadersList shList, int renvoi, char* strOverride) {
	char str[42];
	int num_sh=0,i;
	unsigned char *section;
	section=NULL;

	// Si on a une entrée, on l'utilise au lieu de demander au clavier
	if(strOverride==NULL)
	{
		scanf("%s",str);
	}
	else
	{
		i = 0;
		while (i<41 && strOverride[i]!='\0')
		{
			str[i] = strOverride[i];
			i++;
		}
		str[i] = '\0';
	}

	// On traduit la demande (string) en index dans la table
	num_sh=index_Shdr(str,shList);
	if(num_sh==-1){
		if(is_number(str)){
			if(atoi(str)!=shList.size) fprintf(stderr,"readelf: Warning: Section %s was not dumped because it does not exist!\n",str);
		}
		else fprintf(stderr,"readelf: Warning: Section '%s' was not dumped because it does not exist!\n",str);
		return NULL;
	}
	else{
		section=malloc(sizeof(unsigned char)*shList.headers[num_sh].sh_size);
		if(section != NULL){
			// On affiche le contenu de la section
			if(shList.headers[num_sh].sh_size==0){
				printf("\nSection '%s' has no data to dump.\n",str);
				if(!renvoi){
					free(section);
					section=NULL;
				}
				return section;
			}
			else{
				// Initialisation de l'affichage, récupération du nom de la section
				printf("\nHex dump of section '%s':\n",str);
				for(i=0; i<shList.size; i++){
					if(shList.headers[i].sh_info==num_sh && shList.headers[i].sh_type==SHT_REL)
						printf(" NOTE: This section has relocations against it, but these have NOT been applied to this dump.\n");
				}

				// On se place
				fseek(f,shList.headers[num_sh].sh_offset,0);

				// On affiche le contenu de la section
				for(i=0;i<shList.headers[num_sh].sh_size;i++){
					section[i]=fgetc(f);
				}
			}
			afficher_string(section,shList.headers[num_sh].sh_size);
			if(!renvoi){
				free(section);
				section=NULL;
			}
		}
		else{
			fprintf(stderr,"Erreur d'allocation !\n");
			exit(42);
		}
	}
	return section;
}

// Renvoie un pointeur sur le contenu de la section numero num_sh, NULL si la section n'existe pas. La libération est à la charge de l'utilisateur.
unsigned char *recuperer_section_num(FILE *f, SectionsHeadersList shList, int num_sh) {
	int i;
	unsigned char c;
	unsigned char *section;

	if(num_sh<0 || num_sh>=shList.size){
		return NULL;
	}
	else{
		// On se place
		fseek(f,shList.headers[num_sh].sh_offset,0);
		section=malloc(sizeof(unsigned char)*shList.headers[num_sh].sh_size);
		if(section != NULL){
			// On mémorise le contenu de la section
			for(i=0;i<shList.headers[num_sh].sh_size;i++){
				c=fgetc(f);
				section[i]=c;
			}
		}
		else{
			fprintf(stderr,"Erreur d'allocation !\n");
			exit(42);
		}
	}
	return section;
}
