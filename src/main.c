#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <inttypes.h>
#include <include/idis.h>

#define TOOLNAME "idis"

static struct option longopts[] = {
	{ "base",     required_argument,   NULL, 'b'},
	{ "count",    required_argument,   NULL, 'c'},
	{ "file",     required_argument,   NULL, 'f'},
	{ "help",     no_argument,         NULL, 'h'},
	{ "skip",     required_argument,   NULL, 's'},
	{ "version",  no_argument,         NULL, 'v'},
	{  NULL,     0,                    NULL,  0 }
};

void usage(char **argv)
{
	char *name = NULL;
	name = strrchr(argv[0], '/');
	printf("Usage : %s [options] <iboot>\n",(name ? name + 1: argv[0]));
	printf(" -b, --base\tiBoot base address\n");
	printf(" -c, --count\tinstructions count\n");
	printf(" -s, --skip\tnumber of instructions to skip\n");
	printf(" -h, --help\tprint help\n");
	printf(" -v, --version\tversion\n");
}


int main(int argc, char **argv)
{
	int optindex = 0, opt = 0;
	uint count = 0, skip = 0;
	uint64_t base_addr = 0xffffffff;
	iboot_t *iboot = NULL;
	
	if (argc < 2) {
		usage(argv);
		return -1;
	}

	while ((opt = getopt_long(argc, (char* const *)argv, "bcsvh", longopts, &optindex)) != -1) {
		switch (opt) {
			case 'h':
				usage(argv);
				break;
			case 'b':
				base_addr = (uint64_t)strtol(argv[optind], NULL, 16);
				break;
			case 'c':
				count = atoi(argv[optind]);
				break;
			case 's':
				skip =  atoi(argv[optind]);
				break;
			case 'v' :
				fprintf(stdout, "%s\nCompiled on %s\n", TOOLNAME, __DATE__);
				break;
			default:
				usage(argv);
				return -1;
		}

	}

	iboot = idis_init(argv[argc - 1]);
	if (iboot == NULL) {
		printf("idis init failed\n");
		return 0;
	}

	if (base_addr == 0xffffffff)
		iboot->base_addr = get_iboot_base_addr(iboot);

	do_disassemble(iboot, count, skip);

	iboot_free(iboot);

    return 0;
}
