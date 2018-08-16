#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>

#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>

#include "PJ_RPI.h"

#define SW_PORTC_IO_BASE	0x01C20800			//dataregister
#define PWM_CH_CTRL			0x01C21400			//pwm control register

char * gpio_base;

unsigned int *gpio_base_testing;

#define GPIOA_DATA_CTRL			*(unsigned int *)(gpio_base + 0xA0)

int main() {
	unsigned int * pc;
	int fd;
	unsigned int addr_start, addr_offset, PageSize, PageMask, data;
	unsigned int GPIOA_DR, GPIOE_DR, GPIOA_CFG0;

	PageSize = sysconf(_SC_PAGESIZE);
	PageMask = (~(PageSize-1));
	addr_start = SW_PORTC_IO_BASE & PageMask;
	addr_offset = SW_PORTC_IO_BASE & ~PageMask;

	fd = open("/dev/mem", O_RDWR);

	if(fd < 0) {
		perror("Unable to open /dev/mem");
		return(-1);
	}

	pc = mmap(0, PageSize*2, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr_start);

	if(pc == MAP_FAILED) {
		perror("Unable to mmap file");
		printf("pc:%lx\n", (unsigned long)pc);
		return(-1);
	}

	gpio_base = (char *)pc + addr_offset;

	gpio_base_testing = (unsigned int *)pc + (unsigned int)addr_offset;

	printf("GPIO BASE PC1 -> %08X\n", addr_offset);
	printf("GPIO BASE PC2 -> %08X\n", addr_offset);

	printf("GPIO BASE1 -> %08X\n", gpio_base);
	printf("GPIO BASE2 -> %08X\n", gpio_base_testing);
	printf("GPIO BASE2 -> %08X\n", (unsigned int *)(gpio_base+0x90));

	GPIOA_CFG0 = *(unsigned int *)(gpio_base+0x90);		//offset for controller register
	// PA_CFGO_REG  0x01c20800 page 317 H3 datasheet
	GPIOA_CFG0 |= (1<<0);						//set port PA5 to pwm (011)
	GPIOA_CFG0 &= ~(1<<1);
	GPIOA_CFG0 &= ~(1<<2);

	*(unsigned int *)(gpio_base + 0x90) = GPIOA_CFG0;

	GPIOA_DR = *(unsigned int *)(gpio_base + 0xA0);

	while (1) {
		printf("fanning nguyen\n");
		//GPIOA_DR |= (1<<0);
		//*(unsigned int *)(GPIO_BASE + 0xA0) = GPIOA_DR;
		GPIOA_DATA_CTRL |= (1<<0);

		sleep(1);

		//GPIOA_DR &= (~(1<<0));
		GPIOA_DATA_CTRL &= (~(1<<0));

		sleep(1);
	}

	return 0;
}
