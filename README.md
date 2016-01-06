Etape 1 : Jonathan (lecture_headers)(Fini)  
Etape 2 : Loic (lectureSH)  
Etape 3 : Astor (afficher_section)  
Etape 4 : Thai Binh (lectureST)  
Etape 5 : Gabriel (affichage_relocation)  


Questions :  
-Utilité du numero de la machine  
Test si on est bien sur la machine ARM , si on est pas dans cette machine , dire architecture non reconnue
et des flags ?  

-.o de tailles différentes.  
Même fichier , l'un avec des options de debug en plus (celui du make) : --gdwarf2 et en big endian : -EB
Les 2 sont utilisable.  

arm-eabi-as --gdwarf2  -EB  -r  -o example1-arm.o example1.s  
cette commande donne le même resultat que make
