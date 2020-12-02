#ifndef IDIS_H
#define IDIS_H

typedef struct iboot_s iboot_t;
struct iboot_s {
	void *iboot;
	unsigned int len;
	uint64_t base_addr;
};

iboot_t *idis_init(char *filename);
void iboot_free(iboot_t *iboot);
uint64_t get_iboot_base_addr(iboot_t *iboot);
int do_disassemble(iboot_t *iboot, uint count, uint from);
#endif