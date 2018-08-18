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

#define RET_OK				0

#define SW_PORTC_IO_BASE	0x01C20800
#define SUNXI_SPI1_PBASE	0x01c69000

char * gpio_base;
char * spi1_base;

#define GPIOF_DATA_CTRL			*(unsigned int *)(gpio_base + 0xA0)
#define GPIOF_CFG0_BASE			*(unsigned int *)(gpio_base + 0x90)

#define GPIOA_CFG0_BASE			*(unsigned int *)(gpio_base + 0x00)
#define GPIOA_CFG1_BASE			*(unsigned int *)(gpio_base + 0x04)
#define GPIOA_CFG2_BASE			*(unsigned int *)(gpio_base + 0x08)
#define GPIOA_DATA_CTRL			*(unsigned int *)(gpio_base + 0x10)

/* SPI Registers offsets from peripheral base address */
#define SPI_VER_REG				*(unsigned int *)(spi1_base + 0x00)			/* version number register */
#define SPI_GC_REG				*(unsigned int *)(spi1_base + 0x04)			/* global control register */
#define SPI_TC_REG				*(unsigned int *)(spi1_base + 0x08) 		/* transfer control register */
#define SPI_INT_CTL_REG			*(unsigned int *)(spi1_base + 0x10) 		/* interrupt control register */
#define SPI_INT_STA_REG			*(unsigned int *)(spi1_base + 0x14) 		/* interrupt status register */
#define SPI_FIFO_CTL_REG		*(unsigned int *)(spi1_base + 0x18) 		/* fifo control register */
#define SPI_FIFO_STA_REG		*(unsigned int *)(spi1_base + 0x1C) 		/* fifo status register */
#define SPI_WAIT_CNT_REG		*(unsigned int *)(spi1_base + 0x20) 		/* wait clock counter register */
#define SPI_CLK_CTL_REG			*(unsigned int *)(spi1_base + 0x24) 		/* clock rate control register */
#define SPI_BURST_CNT_REG		*(unsigned int *)(spi1_base + 0x30) 		/* burst counter register */
#define SPI_TRANSMIT_CNT_REG	*(unsigned int *)(spi1_base + 0x34) 		/* transmit counter register */
#define SPI_BCC_REG				*(unsigned int *)(spi1_base + 0x38) 		/* burst control counter register */
#define SPI_DMA_CTL_REG			*(unsigned int *)(spi1_base + 0x88)			/* DMA control register, only for 1639 */
#define SPI_TXDATA_REG			*(unsigned int *)(spi1_base + 0x200)		/* tx data register */
#define SPI_RXDATA_REG			*(unsigned int *)(spi1_base + 0x300)		/* rx data register */

int8_t gpio_base_map(void);
int8_t spi1_base_map(void);

void led_life_on(void);
void led_life_off(void);
void led_life_init(void);

int main() {

	if (gpio_base_map() == RET_OK) {
		printf("gpio base map done\n");
	}

	if (spi1_base_map() == RET_OK) {
		printf("spi1 base map done\n");
	}

	led_life_init();

	/* ledlife init */

	while (1) {
		printf("fanning nguyen\n");
		led_life_on();
		sleep(1);
		led_life_off();
		sleep(1);
	}

	return 0;
}

void led_life_on(void) {
	GPIOF_DATA_CTRL |= (1<<0);
}

void led_life_off(void) {
	GPIOF_DATA_CTRL &= (~(1<<0));
}

int8_t gpio_base_map(void) {
	unsigned int * pc;
	int fd;
	unsigned int addr_start, addr_offset, PageSize, PageMask;

	/* ledlife init */
	PageSize = sysconf(_SC_PAGESIZE);
	PageMask = (~(PageSize-1));
	addr_start = SW_PORTC_IO_BASE & PageMask;
	addr_offset = SW_PORTC_IO_BASE & ~PageMask;

	printf("PageSize    --> [%08X]\n", PageSize);
	printf("PageMask    --> [%08X]\n", PageMask);
	printf("Addr_start  --> [%08X]\n", addr_start);
	printf("Addr_offset --> [%08X]\n", addr_offset);

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

	printf("Addr_start mapped        --> [%08X]\n", (unsigned int)pc);
	printf("Addr_start + Addr_offset --> [%08X]\n", (unsigned int)gpio_base);

	close(fd);

	return RET_OK;
}

int8_t spi1_base_map(void) {
	unsigned int * pc;
	int fd;
	unsigned int addr_start, addr_offset, PageSize, PageMask;

	/* ledlife init */
	PageSize = sysconf(_SC_PAGESIZE);
	PageMask = (~(PageSize-1));
	addr_start = SUNXI_SPI1_PBASE & PageMask;
	addr_offset = SUNXI_SPI1_PBASE & ~PageMask;

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

	spi1_base = (char *)pc + addr_offset;

	close(fd);

	return RET_OK;
}

void led_life_init(void) {
	GPIOF_CFG0_BASE |= (1<<0);						//set port PA5 to pwm (011)
	GPIOF_CFG0_BASE &= ~(1<<1);
	GPIOF_CFG0_BASE &= ~(1<<2);
}







