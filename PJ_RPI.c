#include "PJ_RPI.h"

struct h3_peripheral gpio = {GPIO_BASE};
struct h3_peripheral spi1 = {SPI1_BASE};

// Exposes the physical address defined in the passed structure using mmap on /dev/mem
int map_peripheral(struct h3_peripheral *p) {

	unsigned int PageSize, PageMask, addr_start, addr_offset;

	PageSize = sysconf(_SC_PAGESIZE);
	PageMask = (~(PageSize-1));

	printf("PageSize -> 0x%08X\n", PageSize);
	printf("PageMask -> 0x%08X\n", PageMask);

	addr_start = p->addr_p & PageMask;
	printf("addr_start to mapping  -> 0x%08X\n", addr_start);

	addr_offset = p->addr_p & ~PageMask;
	printf("addr_offset to mapping  -> 0x%08X\n", addr_offset);

	/* open /dev/mem */
	if ((p->mem_fd = open("/dev/mem", O_RDWR) ) < 0) {
		printf("Failed to open /dev/mem, try checking permissions.\n");
		return -1;
	}
	else {
		printf("opened /dev/mem\n");
	}

	p->map = mmap (
				NULL,
				PageSize*2,
				(PROT_READ|PROT_WRITE),
				MAP_SHARED,
				p->mem_fd,						// File descriptor to physical memory virtual file '/dev/mem'
				addr_start		// Address in physical map that we want this memory block to expose
				);

	if (p->map == MAP_FAILED) {
		perror("mmap");
		return -1;
	}

	printf("addr is maped          -> 0x%08X\n", (volatile unsigned int *)p->map);

	p->addr = (unsigned int *)p->map + addr_offset;

	printf("addr is maped + offset -> 0x%08X\n", p->addr);

	return 0;
}

void unmap_peripheral(struct h3_peripheral *p) {
	munmap(p->map, BLOCK_SIZE);
	close(p->mem_fd);
}

void spi1_init() {




} 

// Priority 

int SetProgramPriority(int priorityLevel)
{
	//    struct sched_param sched;

	//    memset (&sched, 0, sizeof(sched));

	//    if (priorityLevel > sched_get_priority_max (SCHED_RR))
	//        priorityLevel = sched_get_priority_max (SCHED_RR);

	//    sched.sched_priority = priorityLevel;

	//    return sched_setscheduler (0, SCHED_RR, &sched);
}
