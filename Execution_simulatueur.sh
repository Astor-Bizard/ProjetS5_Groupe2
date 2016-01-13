## Gabriel Bouvier
## Exectution par le simulatueur: plan de base.
#!/bin/bash

## vérifier qu'on a au moins un argument et que c'est un fichier
if [ $# -gt 0 ]
then
	if [ -f $1 ]
	then
		## Récupérer les arguments (args d'affichage du simulatueur)
		## Récupérer le nom du code à exécuter (Arg1)

		## lancer le simulatueur > arm_simulator (+ args + sleep1)
		arm_simulator --gdb-port 6666 --trace-registers --trace-memory --trace-state &
		sleep 1
		## se connecter au simulatueur > arm_simoluator_interface
		## Envoyer le code au simulatueur (dans l'interface sim)
		## run le simulatueur (dans l'interface sim)
		ARM_runner_example --debug elf_linker-1.0/ARM_runner_example.c --debug elf_linker-1.0/gdb_protocol.c --host localhost --service 6666 $1
	else
		echo "Arguments de la forme: [NOM_FICHIER(.o)] [Flags de arm_simulator]..."
	fi
else
	echo "Ce programme attend un fichier en argument"
fi

