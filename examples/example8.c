#include <stdio.h>
#include <stdlib.h>
#include "arbrebin.h"

/* implementation des operateurs de l'Arbre binaire */


/***************************************
 * ArbreVide                           *
 * parametres : aucun                  *
 * resultat : un Arbre                 *
 * description : renvoie un arbre vide *
 * effet de bord :aucun                *
 ***************************************/
Arbre ArbreVide() {
  return NULL;
}


/**********************************************************************
 * NouveauNoeud                                                       *
 * parametres : les donnees : un Arbre g, un caractere c, un Arbre d  *
 * resultat : un Arbre                                                *
 * description : renvoie un nouvel Arbre dont la racine est etiquetee *
 * par c, de fils gauche g et de fils droit d                         *
 * effet de bord : une nouvelle cellule est allouee                   *
 **********************************************************************/
Arbre NouveauNoeud(Arbre g, Element c, Arbre d) {
  Arbre a = (Arbre)malloc(sizeof(struct cellule));

  a->etiq = c;
  a->fg = g;
  a->fd = d;

  return a;
}

/********************************************
 * EstVide                                  *
 * parametres : un Arbre a                  *
 * resultat : un booleen                    *
 * description : renvoie vrai si a est vide *
 * effet de bord : aucun                    *
 ********************************************/
int EstVide(Arbre a) {
  return (a==NULL);
}


/******************************************
 * Etiq                                   *
 * parametres : un Arbre a                *
 * precondition : a non vide              *
 * resultat : un caractere                *
 * description : renvoie l'etiquette de a *
 * effet de bord : aucun                  *
 ******************************************/
Element Etiq(Arbre a) {
  return a->etiq;
}


/*********************************************
 * FilsGauche                                *
 * parametres : un Arbre a                   *
 * precondition : a non vide                 *
 * resultat : un arbre                       *
 * description : renvoie le fils gauche de a *
 * effet de bord : aucun                     *
 *********************************************/
Arbre FilsGauche(Arbre a) {
  return a->fg;
}


/********************************************
 * FilsDroit                                *
 * parametres : un Arbre a                  *
 * precondition : a non vide                *
 * resultat : un arbre                      *
 * description : renvoie le fils droit de a *
 * effet de bord : aucun                    *
 ********************************************/
Arbre FilsDroit(Arbre a) {
  return a->fd;
}


/********************************************
 * LibererArbre                             *
 * parametres : un Arbre a                  *
 * resultat : aucun                         *
 * description : libere l'arbre a           *
 * effet de bord : a est detruit            *
 ********************************************/
void LibererArbre(Arbre a) {
  if (!EstVide(a)) {
    LibererArbre(FilsGauche(a));
    LibererArbre(FilsDroit(a));
    free(a);
  }
}


/**********************************************************************
 * EcrireArbre                                                        *
 * parametres : un descripteur de fichier, un Arbre a                 *
 * precondition : le fichier est ouvert en ecriture                   *
 * resultat : aucun                                                   *
 * description : ecrit l'arbre a dans le fichier, sous la forme d'une *
 * sequence de triplets (entier, entier representant l'arbre gauche,  *
 *  entier representant l'arbre droit). L'arbre vide est represente   *
 * par 0. La sequence est terminee par 0.                             *
 * effet de bord : ecriture dans le fichier                           *
 **********************************************************************/
int cpt_noeud;

int EcrireArbreRec(FILE * fichier, Arbre a) {
  int fg,fd;
  int racine;
  
  if (EstVide(a)) {
    /* Ne rien ecrire, renvoyer 0 */
    return 0;
  } else {
    /* Ecrire le fils gauche, fg = caractere a la racine du fils gauche */
    fg = EcrireArbreRec(fichier,FilsGauche(a));
    /* Ecrire le fils droit, fd = caractere a la racine du fils droit */
    fd = EcrireArbreRec(fichier,FilsDroit(a));
    /* Ecrire la sequence (caractere du noeud courant, fg, fd) */
    if (fg == 0) {
      /* Ecriture d'une feuille : le code de l'arbre est le code
         ascii du caractere */
      racine = (int)Etiq(a);
    } else {
      racine = cpt_noeud;
      cpt_noeud += 1;
    };
    fwrite(&racine,sizeof(int),1,fichier);
    fwrite(&fg,sizeof(int),1,fichier);
    fwrite(&fd,sizeof(int),1,fichier);
    return racine;
  }
}

void EcrireArbre(FILE * fichier, Arbre a) {
  int zero = 0;
  cpt_noeud = 256;
  EcrireArbreRec(fichier,a);
  /* sequence terminee par 0 */
  fwrite(&zero,sizeof(int),1,fichier);
}

/*********************************************************************
 * LireArbre                                                         *
 * parametres : un descripteur de fichier                            *
 * precondition : le fichier est ouvert en lecture, et contient a la *
 * position courante un arbre ecrit par EcrireArbre                  *
 * resultat : un arbre                                               *
 * description : lit l'arbre dans le fichier, dans lequel il a ete   *
 * ecrit par EcrireArbre.                                            *
 * effet de bord : le fichier a ete lu, un arbre a ete cree          *
 *********************************************************************/
Arbre LireArbre(FILE * fichier) {
  /* Stockage des noeuds crees */
  Arbre TabArbre[512];
  int entierlu,fg,fd,racine;
  Element etiq;

  /* Initialisation : TabArbre[0] contient l'arbre vide */
  TabArbre[0] = ArbreVide();

  racine = 0;
  fread(&entierlu,sizeof(int),1,fichier);
  while(entierlu!=0) {
//    printf("%d\n",entierlu);
    fread(&fg,sizeof(int),1,fichier);
    fread(&fd,sizeof(int),1,fichier);
//    printf("%d\n",fg);
//    printf("%d\n",fd);
    /* Assertion : TabArbre[fg] et TabArbre[fd] ont ete affectes,
       eventuellement a l'arbre vide si fg=0 ou fd=0 */
    if (fg == 0) {
      etiq = (Element)entierlu;
    } else {
      etiq = 'a';
    };
    TabArbre[entierlu] = NouveauNoeud(TabArbre[fg],etiq,TabArbre[fd]);
    racine = entierlu;
    /* Noeud suivant */
    fread(&entierlu,sizeof(int),1,fichier);
  }
  return TabArbre[racine];
}


/*********************************************************************
 * AfficherArbre                                                     *
 * parametres : un arbre a                                           *
 * resultat : aucun                                                  *
 * description : affiche l'arbre a sur la sortie standard            *
 * effet de bord : un arbre est affiche                              *
 *********************************************************************/
void AfficherArbreRec(Arbre a, int indent) {
  int i;
  
  if (!EstVide(a)) {
    for(i = 0; i < indent; i++) {
      printf(" ");
    }
    printf("%c\n",Etiq(a));
    AfficherArbreRec(FilsGauche(a),indent+2);
    AfficherArbreRec(FilsDroit(a),indent+2);
  }
}
  
void AfficherArbre(Arbre a) {
  AfficherArbreRec(a,0);
}

void AfficherArbreFichRec(Arbre a, int indent,FILE *f) {
  int i;
  char c;
  if (!EstVide(a)) {
    for(i = 0; i < indent; i++) {
      c = ' ';
			fprintf(f,"%c",c);
    }
    fprintf(f,"%c",Etiq(a));
		c = '\n';
    fprintf(f,"%c",c);
    AfficherArbreFichRec(FilsGauche(a),indent+2,f);
    AfficherArbreFichRec(FilsDroit(a),indent+2,f);
  }
}

void AfficherArbreFich(Arbre a,FILE *f){
	AfficherArbreFichRec(a,0,f);
}


