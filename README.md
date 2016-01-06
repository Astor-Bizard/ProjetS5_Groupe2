Etape 1 : Jonathan (lecture_headers)(Fini)  
Etape 2 : Loic (lectureSH)(Fini)  
Etape 3 : Astor (afficher_section)(Fini)  
Etape 4 : Thai Binh (lectureST)(Fini)  
Etape 5 : Gabriel (affichage_relocation)  

Etape 6 : Jonathan  
Etape 7 : Loic (correctionSymboles)  
Etape 8 : Thai Binh (reimpl_R_ARM_ABS)  


# Questions :  
-Utilité du numero de la machine  
Test si on est bien sur la machine ARM , si on est pas dans cette machine , dire architecture non reconnue
et des flags ?  

-.o de tailles différentes.  
Même fichier , l'un avec des options de debug en plus (celui du make) : --gdwarf2 et en big endian : -EB
Les 2 sont utilisable.  

arm-eabi-as --gdwarf2  -EB  -r  -o example1-arm.o example1.s  
cette commande donne le même resultat que make  

## Mémo des commandes :  

arm-eabi-as -o example1_as.o example1.s  
arm-eabi-as -o example2_as.o example2.s  
arm-eabi-as -o example3_as.o example3.s  
arm-eabi-as -o example4_as.o example4.s  

source ~/.envarm/setenvarm.sh  
