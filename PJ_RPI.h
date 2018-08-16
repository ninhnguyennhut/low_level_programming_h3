#ifndef _INC_PJ_GPIO_H
#define _INC_PJ_GPIO_H

#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>

#include <sched.h>		// To set the priority on linux

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

// Define which Raspberry Pi board are you using. Take care to have defined only one at time.
//#define RPI
#define H3_SOC 1

#ifdef H3_SOC
	#define H3_PERI_BASE	0x01000000
	#define GPIO_BASE		(H3_PERI_BASE + 0xC20800)		// GPIO controller. Maybe wrong. Need to be tested.
	#define SPI1_BASE 		(H3_PERI_BASE + 0xC69000)				// SPI controller
#endif

#define BLOCK_SIZE			(1024*2)


/* IO Acces */
struct h3_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void *map;
	volatile unsigned int *addr;
};

extern struct h3_peripheral gpio; 	// They have to be found somewhere, but can't be in the header
extern struct h3_peripheral spi1;	// so use extern!!

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g)			*(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)			*(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a)	*(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET			*(gpio.addr + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR			*(gpio.addr + 10) // clears bits which are 1 ignores bits which are 0

#define GPIO_READ(g)		*(gpio.addr + 13) &= (1<<(g))

/*
 * GPIOA reg off
 */
#define PA_CFG0_REG		*(gpio.addr + 0x00)
#define PA_CFG1_REG		*(gpio.addr + 0x04)
#define PA_CFG2_REG		*(gpio.addr + 0x08)
#define PA_CFG3_REG		*(gpio.addr + 0x0C)
#define PA_DATA_REG		*(gpio.addr + 0x10)
#define PA_DRV0_REG		*(gpio.addr + 0x14)
#define PA_DRV1_REG		*(gpio.addr + 0x18)
#define PA_PULL0_REG	*(gpio.addr + 0x1C)
#define PA_PULL1_REG	*(gpio.addr + 0x20)

#define PE_CFG0_REG			*(gpio.addr +0x90)
#define PE_CFG1_REG			*(gpio.addr +0x94)
#define PE_CFG2_REG			*(gpio.addr +0x98)
#define PE_CFG3_REG			*(gpio.addr +0x9C)
#define PE_DATA_REG			*(gpio.addr +0xA0)
#define PE_DRV0_REG			*(gpio.addr +0xA4)
#define PE_DRV1_REG			*(gpio.addr +0xA8)
#define PE_PULL0_REG		*(gpio.addr +0xAC)
#define PE_PULL1_REG		*(gpio.addr +0xB0)

/* spi1 addr off */

#define SPI_GCR		*(spi1.addr + 0x04	)		// SPI Global Control Register
#define SPI_TCR		*(spi1.addr + 0x08	)		// SPI Transfer Control register
#define SPI_IER		*(spi1.addr + 0x10	)		// SPI Interrupt Control register
#define SPI_ISR		*(spi1.addr + 0x14	)		// SPI Interrupt Status register
#define SPI_FCR		*(spi1.addr + 0x18	)		// SPI FIFO Control register
#define SPI_FSR		*(spi1.addr + 0x1C	)		// SPI FIFO Status register
#define SPI_WCR		*(spi1.addr + 0x20	)		// SPI Wait Clock Counter register
#define SPI_CCR		*(spi1.addr + 0x24	)		// SPI Clock Rate Control register
#define SPI_MBC		*(spi1.addr + 0x30	)		// SPI Burst Counter register
#define SPI_MTC		*(spi1.addr + 0x34	)		// SPI Transmit Counter Register
#define SPI_BCC		*(spi1.addr + 0x38	)		// SPI Burst Control register
#define SPI_TXD		*(spi1.addr + 0x200	)		// SPI TX Data register
#define SPI_RXD		*(spi1.addr + 0x300	)		// SPI RX Data register

/* Function prototypes */
int map_peripheral(struct h3_peripheral *p);
void unmap_peripheral(struct h3_peripheral *p);

/* spi1 */
void spi1_init();
// Priority
int SetProgramPriority(int priorityLevel);

#endif
