## Gabriel Bouvier
## Exectution par le simulatueur: plan de base.
#!bin/bash

## vérifier qu'on a au moins un argument et que c'est un fichier
if[ $# -gt 0 and -f $1 ]
then
	## Récupérer les arguments (args d'affichage du simulatueur)
	## Récupérer le nom du code à exécuter (Arg1)

	arg_simulator = "--gdb-port 6666"
	for i in $
	do
		if[ $i != $1 ]
			arg_simulator = "$arg_simulator $i"
		fi
	done
	echo debug: arg_simulator= $arg_simulator



	## lancer le simulatueur > arm_simulator (+ args + sleep1)
	arm_simulator $arg_simulator &
	sleep 1
	## se connecter au simulatueur > arm_simoluator_interface

	## Envoyer le code au simulatueur 

	## run le simulatueur (selon les arguments)
else
	echo Arguments de la forme: [NOM_FICHIER(.o)] [Flags de arm_simulator]...
fi