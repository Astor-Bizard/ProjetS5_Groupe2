/*
Cree le 5 janvier 2016 par Astor
Affichage d'une section specifique
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>

// Retourne le numéro de la section demandée, par son nom ou son numéro, -1 si invalide.
int index_Shdr(char str[], FILE *f, int ShdrCount, int ShdrStrIndex){
	int i,num_sh;
	if(str[0]!='\0'){
		if(str[0]>=48 && str[0]<=57){
			num_sh = str[i]-48;
			for(i=1;str[i]!='\0';i++){
				if(str[i]>=48 && str[i]<=57) num_sh = num_sh*10 + str[i]-48;
			}
		}
		else{
			//while(num_sh<ShdrCount && strcmp(str,tabSH[num_sh].name)
		}
		if(num_sh<0 || num_sh>=ShdrCount) return -1;
		else return num_sh;
	}
	else return -1;
}

void afficher_section(char *nom_f, Elf32_Shdr *tabSH, int ShdrCount, int ShdrStrIndex){
	FILE *f;
	char str[42];
	int num_sh=0,i;

	printf("Section à afficher : ");
	scanf("%s",str);

	f=fopen(nom_f,"r");
	num_sh=index_Shrd(str,f,ShdrStrIndex);
	fclose(f);

	if(num_sh<0 || num_sh>=ShdrCount) printf("Section absente : %d", sum_sh);
	else{
		f=fopen(nom_f,"r");
		printf("Section %s\n",str);
		for(i=0;i<tabSH[num_sh].offset) fgetc(f);
		for(i=0;i<tabSH[num_sh].size) printf("%x",fgetc(f));
		fclose(f);
	}
}
