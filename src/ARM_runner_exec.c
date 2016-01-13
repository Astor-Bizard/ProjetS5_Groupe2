#include "ARM_runner_exec.h"

void lecture_fichier(FILE *f, uint32_t *address, size_t *size)
{
	// on lit le fichier, on renvoit a partir de quand il faut lire et combien d'octet? (ligne?)
	*address = lecture_Headers(f, 1).e_entry;
	fseek(f,0,SEEK_END);
	*size = ftell(f);
	rewind(f);
}


void run(char *hostname, char *servicename, FILE* f)
{
	size_t size;
	uint32_t address;
	lecture_fichier(f, &address, &size);

	// début de la simulation proprement dite.
	arm_simulator_data_t simulator;
	debug("Connecting to simulator using host %s and service %s\n", hostname, servicename);
	simulator = arm_connect(hostname, servicename);

	debug("Fetching code to simulator and setting PC\n");
	// remplissage de la mémoire
	arm_write_memory(simulator, address, f, size);
	// addresse de pc au début
	arm_write_register(simulator, 15, address);

	debug("Running simulator\n");
	arm_run(simulator);
	debug("End of simulation\n");
}


int main (int argc,char* argv[])
{
	//usage: mode d'emploi?
	int opt;
	char *hostname, *servicename;
	FILE* f;

	struct option longopts[] = {
		{ "debug", required_argument, NULL, 'd' },
		{ "host", required_argument, NULL, 'H' },
		{ "service", required_argument, NULL, 'S' },
		{ "help", no_argument, NULL, 'h' },
		{ NULL, 0, NULL, 0 }
	};

	hostname = NULL;
	servicename = NULL;
	while ((opt = getopt_long(argc, argv, "S:H:d:h", longopts, NULL)) != -1) {
		switch(opt) {
		case 'H':
			hostname = optarg;
			break;
		case 'S':
			servicename = optarg;
			break;
		case 'h':
			//usage(argv[0]);
			exit(0);
		case 'd':
			add_debug_to(optarg);
			break;
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			//usage(argv[0]);
			exit(1);
		}
	}

	f = fopen(argv[1],"r");
	//RUN: 
	run(hostname, servicename, f);
	fclose(f);
	return 0;
}