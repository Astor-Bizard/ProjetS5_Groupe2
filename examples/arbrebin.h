#ifndef __ABR_H__
#define __ABR_H__

#include <stdio.h>

typedef unsigned char Element;

struct cellule{
  Element etiq;
  struct cellule *fd;
  struct cellule *fg;
};

typedef struct cellule * Arbre;



/***************************************
 * ArbreVide                           *
 * parametres : aucun                  *
 * resultat : un Arbre                 *
 * description : renvoie un arbre vide *
 * effet de bord :aucun                *
 ***************************************/
Arbre ArbreVide();


/**********************************************************************
 * NouveauNoeud                                                       *
 * parametres : les donnees : un Arbre g, un caractere c, un Arbre d  *
 * resultat : un Arbre                                                *
 * description : renvoie un nouvel Arbre dont la racine est etiquetee *
 * par c, de fils gauche g et de fils droit d                         *
 * effet de bord : une nouvelle cellule est allouee                   *
 **********************************************************************/
Arbre NouveauNoeud(Arbre g, Element c, Arbre d);


/********************************************
 * EstVide                                  *
 * parametres : un Arbre a                  *
 * resultat : un booleen                    *
 * description : renvoie vrai si a est vide *
 * effet de bord : aucun                    *
 ********************************************/
int EstVide(Arbre a);


/******************************************
 * Etiq                                   *
 * parametres : un Arbre a                *
 * precondition : a non vide              *
 * resultat : un caractere                *
 * description : renvoie l'etiquette de a *
 * effet de bord : aucun                  *
 ******************************************/
Element Etiq(Arbre a);


/*********************************************
 * FilsGauche                                *
 * parametres : un Arbre a                   *
 * precondition : a non vide                 *
 * resultat : un arbre                       *
 * description : renvoie le fils gauche de a *
 * effet de bord : aucun                     *
 *********************************************/
Arbre FilsGauche(Arbre a);


/********************************************
 * FilsDroit                                *
 * parametres : un Arbre a                  *
 * precondition : a non vide                *
 * resultat : un arbre                      *
 * description : renvoie le fils droit de a *
 * effet de bord : aucun                    *
 ********************************************/
Arbre FilsDroit(Arbre a);


/********************************************
 * LibererArbre                             *
 * parametres : un Arbre a                  *
 * resultat : aucun                         *
 * description : libère l'arbre a           *
 * effet de bord : a est détruit            *
 ********************************************/
void LibererArbre(Arbre a);


/**********************************************************************
 * EcrireArbre                                                        *
 * parametres : un descripteur de fichier, un Arbre a                 *
 * precondition : le fichier est ouvert en ecriture                   *
 * resultat : aucun                                                   *
 * description : ecrit l'arbre a dans le fichier, sous la forme d'une *
 * sequence de triplets (caractere, caractere de l'arbre gauche,      *
 * caractere de l'arbre droit). L'arbre vide est represente par le    *
 * caractere de code 0. La sequence est terminee par le code 0.       *
 * effet de bord : ecriture dans le fichier                           *
 **********************************************************************/
void EcrireArbre(FILE * fichier, Arbre a);


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
Arbre LireArbre(FILE * fichier);



/*********************************************************************
 * AfficherArbre                                                     *
 * parametres : un arbre a                                           *
 * resultat : aucun                                                  *
 * description : affiche l'arbre a sur la sortie standard            *
 * effet de bord : un arbre est affiche                              *
 *********************************************************************/
void AfficherArbre(Arbre a);

void AfficherArbreFich(Arbre a,FILE *f);


#endif
