/* Ce README a ete cree pour communiquer facilement des informations via l'affichage sur github. Il n'est pas destine a aider a la comprehension de ce depot. Voir pour cela MODE_D_EMPLOI et STRUCTURE_DU_CODE */

ON A FINI OUAAAAAIS
===================

##Taches  

Etape 1 : Jonathan (lecture_headers)(Fini)  
Etape 2 : Loic (lectureSH)(Fini)  
Etape 3 : Astor (afficher_section)(Fini)  
Etape 4 : Thai Binh (lectureST)(Fini)  
Etape 5 : Gabriel (affichage_relocation)(Fini)  

PHASE 1 TERMINEE - Tests example compris  

Etape 6 : Jonathan (renum_section)(Fini)  
Etape 7 : Loic (correctionSymboles)(Fini)  
Etape 8-9 : Thai Binh (reimpl_R_ARM)(Fini)  
Etape 10 : Gabriel  

#A savoir :  
Ajoutez ces lignes dans votre .bashrc :  
> source ~/.envarm/setenvarm.sh >/dev/null  
> export PATH=$PATH:[chemin vers le projet]/elf_linker-1.0/  

Etape 8 :  
R_ARM_ABS* = 32,16,8 uniquement  

Etape 9 :  
R_ARM_CALL <=> R_ARM_JUMP24  

call/Jump 3 !!! endian  
abs32 4  


##Questions :
- Comment faire fonctionner le simulatueur?!(Solved: Export Path ci dessus)  


-Utilité du numero de la machine  
Test si on est bien sur la machine ARM , si on est pas dans cette machine , dire architecture non reconnue  
et des flags ? (Solved?)  

-.o de tailles différentes.  
Même fichier , l'un avec des options de debug en plus (celui du make) : --gdwarf2 et en big endian : -EB  
Les 2 sont utilisable. (Solved?)  

arm-eabi-as --gdwarf2  -EB  -r  -o example1-arm.o example1.s  
cette commande donne le même resultat que make  



-Dans la relocation, il semblerai que T et A soit toujours égal à 0 , est-ce normal , si oui à quoi sert cette operation ?  

-Question que faut il modifier dans la table des symboles  
Que faut il modifier a partir de la table des realocation  
On peut modifier les symb .data .text fourni mais qu'en est t'il des main $a data_global , etc.... ?  

A quoi doit ressembler la nouvelle table des symboles. Pourquoi est-elle plus grande que l'ancienne ?  
  
 rep : table des symboles et des noms à enlever des sections

  
  
##Mémo des commandes :    

> $ arm-eabi-as -o example1_as.o example1.s  
> $ arm-eabi-as -o example2_as.o example2.s  
> $ arm-eabi-as -o example3_as.o example3.s  
> $ arm-eabi-as -o example4_as.o example4.s  

> $ source ~/.envarm/setenvarm.sh  



arm-eabi-strip fichier exécutable (enleve tout ce qui est inutile pour l'execution : resultat à obtenir)

