/*
Cree le 5 janvier 2016 par Astor
Affichage d'une section specifique
*/
#include "afficher_section.h"
#include "lectureSH.h"

//char** getSectionsNames(FILE* f, int sectionHeaderCount, uint32_t tableSize, Elf32_Off tableOffset);

// Retourne le numéro de la section demandée, par son nom ou son numéro, -1 si invalide.
int index_Shdr(char str[], FILE *f, int ShdrCount, int ShdrStrIndex){
	int i,num_sh;
	char **names;
	if(str[0]!='\0'){
		if(str[0]>=48 && str[0]<=57){
			num_sh = str[i]-48;
			for(i=1;str[i]!='\0';i++){
				if(str[i]>=48 && str[i]<=57) num_sh = num_sh*10 + str[i]-48;
			}
		}
		else{
			names=getSectionsNames
			//while(num_sh<ShdrCount && strcmp(str,tabSH[num_sh].name)
		}
		if(num_sh<0 || num_sh>=ShdrCount) return -1;
		else return num_sh;
	}
	else return -1;
}

char *afficher_section(char *nom_f, Elf32_Shdr *tabSH, int ShdrCount, int ShdrStrIndex){
	FILE *f;
	char str[42];
	int num_sh=0,i;
	char c;

	printf("Section à afficher : ");
	scanf("%s",str);

	f=fopen(nom_f,"r");
	num_sh=index_Shrd(str,f,ShdrStrIndex);
	fclose(f);

	if(num_sh<0 || num_sh>=ShdrCount){
		printf("Section absente : %d", sum_sh);
		return NULL;
	}
	else{
		f=fopen(nom_f,"r");
		printf("Section %s\n",str);
		for(i=0;i<tabSH[num_sh].offset;i++) fgetc(f);
		char section[tabSH[num_sh].size+1];
		for(i=0;i<tabSH[num_sh].size;i++){
			c=fgetc(f)
			printf("%x",c);
			section[i]=c;
		}
		section[i]='\0';
		fclose(f);
		return section;
	}
}
