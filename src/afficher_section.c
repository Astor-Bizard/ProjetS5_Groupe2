/*
Cree le 5 janvier 2016 par Astor
Affichage d'une section specifique
*/
#include "afficher_section.h"
#include "lectureSH.h"

#define ASCII_0 48

int is_digit(char c){
	return (c>=ASCII_0 && c<=ASCII_0+9);
}

int is_number(char str[]){
	int i=0;
	while(is_digit(str[i]))i++;
	return str[i]=='\0';
}

// Retourne le numéro de la section demandée, par son nom ou son numéro, -1 si invalide.
int index_Shdr(char str[], FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH){
	int i,num_sh;
	char *names;
	names = fetchSectionNames(f,elfHeader,tabSH);
	if(str[0]!='\0'){
		// Cas nombre : on traduit le nombre (string) en int
		i=0;
		num_sh=0;
		while(is_digit(str[i])){
			num_sh = num_sh*10 + str[i]-ASCII_0;
			i++;
		}
		if(str[i]=='\0'){
			if(num_sh<elfHeader.e_shnum){
				strcpy(str,getSectionNameBis(names,tabSH[num_sh]));
				return num_sh;
			}
			else return -1;
		}
		// Cas nom : on le cherche dans la table str
		else{
			num_sh=0;
			while(num_sh<elfHeader.e_shnum && strcmp(str,getSectionNameBis(names,tabSH[num_sh]))) num_sh++;
		}
		if(num_sh>=elfHeader.e_shnum) return -1;
		else return num_sh;
	}
	else return -1;
}

// Affiche le contenu d'une section désignée par nom ou numéro. Renvoie ce contenu, NULL si la section n'existe pas. La libération est à la charge de l'utilisateur.
unsigned char *afficher_section(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH, int renvoi, char* strOverride){
	char str[42];
	int num_sh=0,i,j;
	unsigned char c;
	unsigned char *section;
	char *names, *sectionName;
	section=NULL;
	unsigned char aff[17];
	for(j=0;j<17;j++)aff[j]='\0';

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

	rewind(f);
	// On traduit la demande (string) en index dans la table
	num_sh=index_Shdr(str,f,elfHeader,tabSH);
	if(num_sh==-1){
		if(is_number(str)){
			if(atoi(str)!=elfHeader.e_shnum) fprintf(stderr,"readelf: Warning: Section %s was not dumped because it does not exist!\n",str);
		}
		else fprintf(stderr,"readelf: Warning: Section '%s' was not dumped because it does not exist!\n",str);
		return NULL;
	}
	else{
		if(renvoi) section=malloc(sizeof(unsigned char)*(tabSH[num_sh].sh_size+1));
		if(section != NULL || !renvoi){
			// On affiche le contenu de la section
			if(tabSH[num_sh].sh_size==0){
				printf("\nSection '%s' has no data to dump.\n",str);
				if(renvoi) section[0]='\0';
				return section;
			}
			else{
				printf("\nHex dump of section '%s':\n",str);
				names = fetchSectionNames(f,elfHeader,tabSH);
				for(i=0;i<elfHeader.e_shnum;i++){
					sectionName=getSectionNameBis(names,tabSH[i]);
					if(!strcmp(str,sectionName+4) && tabSH[i].sh_type==SHT_REL) printf(" NOTE: This section has relocations against it, but these have NOT been applied to this dump.\n");
				}
				// On se place
				fseek(f,tabSH[num_sh].sh_offset,0);
				printf("  0x%08x ",0);
				i=0;
				while(i<tabSH[num_sh].sh_size){
					if(i!=0){
						if(i%16==0){
							printf(" %s\n  0x%08x ",aff,i);
							for(j=0;j<17;j++)aff[j]='\0';
						}
						else if(i%4==0) printf(" ");
					}
					c=fgetc(f);
					printf("%02x",c);
					if(renvoi) section[i]=c;
					if(c>=32 && c<=126) aff[i%16]=c;
					else aff[i%16]='.';
					i++;
				}
				if(renvoi) section[i]='\0';
				while(i%16!=0){
					if(i%4==0) printf(" ");
					printf("  ");
					i++;
				}
				printf(" %s",aff);
			}
			printf("\n\n");
		}
		else{
			fprintf(stderr,"Erreur d'allocation !\n");
			exit(42);
		}
	}
	return section;
}

// Affiche le contenu de la section numero num_sh. Renvoie ce contenu, NULL si la section n'existe pas. La libération est à la charge de l'utilisateur.
unsigned char *recuperer_section_num(FILE *f, Elf32_Ehdr elfHeader, Elf32_Shdr *tabSH, int num_sh){
	int i;
	unsigned char c;
	unsigned char *section;

	if(num_sh<0 || num_sh>=elfHeader.e_shnum){
		return NULL;
	}
	else{
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
		}
		else{
			fprintf(stderr,"Erreur d'allocation !\n");
			exit(42);
		}
	}
	return section;
}
