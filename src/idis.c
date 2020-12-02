#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <capstone/capstone.h>

#include <include/idis.h>

/*
* open and read iboot file
*/
iboot_t *idis_init(char *filename)
{
	FILE *fp = NULL;
	struct stat st;
	iboot_t *iboot = NULL;
	int ret = 0;


	/* open and read file*/
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("failed to open file %s\n", filename);
		goto err;
	}

	/* allocate mem for our ptr struct */
	iboot = malloc(sizeof(iboot_t));
	if (iboot == NULL) {
		printf("could not allocate memory to iboot struct");
		goto err;
	}

	if(stat(filename, &st) != 0) {
		printf("could not get size\n");
		goto err;
	}

	iboot->len = st.st_size;

	/* alloc mem for iboot->iboot*/
	iboot->iboot = malloc(iboot->len);
	if (iboot->iboot == NULL) {
		printf("could not allocate memory to iboot buf\n");
		goto err;
	}

	/* read file and store in iboot */
	ret = fread(iboot->iboot, 1, st.st_size, fp);
	if (ret != st.st_size) {
		printf("iBoot size didn't match\n");
		goto err;
	}

	return iboot;

err:
	if (iboot->iboot)
		free(iboot->iboot);
	if (iboot)
		free(iboot);
	if (fp)
		fclose(fp);

	return NULL;

}

void iboot_free(iboot_t *iboot)
{
	free(iboot->iboot);
	free(iboot);
}


/*
* Find iBoot base address.
* We look for the version string, since iOS14 (iBoot-6603)
* base address offset has been moved to 0x300 instead of 0x318.
* Mostly based on dayt0n's function.
*/
uint64_t get_iboot_base_addr(iboot_t *iboot)
{
	char version[5];
	uint32_t base_addr_offset = 0x318;
 	void *version_string = memmem(iboot->iboot, iboot->len, "iBoot-",strlen("iBoot-"));
	if (version == NULL)
		return 0;

	/* Grab the first 4 digit
	 * TODO : find first LDR insn and grab offset to base addr
	 */
	strncpy(version, version_string + 6, 4);

	if (atoi(version) >= 6603)
		base_addr_offset = 0x300;

	return *(uint64_t*)(iboot->iboot + base_addr_offset);
}

int do_disassemble(iboot_t *iboot, uint count, uint from)
{
	size_t insn_count, i;
	cs_insn *insn;
	static csh handle;
	cs_opt_skipdata skipdata = {
       .mnemonic = "db",
    };

	if (cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle) != CS_ERR_OK)
		return -1;

	/* use SKIPDATA mode so it doesn't stop whenever it finds DATA */
	cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
    cs_option(handle, CS_OPT_SKIPDATA, CS_OPT_ON);
    cs_option(handle, CS_OPT_SKIPDATA_SETUP, (size_t)&skipdata);

	insn_count = cs_disasm(handle, iboot->iboot, iboot->len - 1, iboot->base_addr, count, &insn);
	if (insn_count <= 0) {
		printf("ERROR: Failed to disassemble code\n");
		return -1;
	}

	i = from;
	for (; i < insn_count; i++) {
		//printf("%d\n", insn[i].id);
		printf("0x%"PRIx64":\t%s \t\t%s\n", insn[i].address, insn[i].mnemonic, insn[i].op_str);
		//break;
	}

	cs_free(insn, insn_count);
	cs_close(&handle);

	return 0;
}
