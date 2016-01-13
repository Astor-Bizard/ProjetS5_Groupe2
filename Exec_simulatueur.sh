## Gabriel Bouvier
#!/bin/bash

## vérifier le fait d'avoir un fichier en argument
if [ $# -gt 0 ]; then
	if [ -f $1 ]; then

		arm_simulator --gdb-port 6666 --trace-register --trace-memory --trace-state &
		sleep 1
		## lancer le simulateur puis s'y connecter.
		ARM_runner_exec --debug elf_linker-1.0/ARM_runner_example.c --debug elf_linker-1.0/gdb_protocol.c --host localhost --service 6666 $1
	else
		echo "Arguments de la form:truc"
	fi
else
	echo "Ce prog attend un fichier en argument"
fi