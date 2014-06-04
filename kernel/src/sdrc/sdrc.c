/*
 * sdrc.c
 *
 *  Created on: 03.06.2014
 *      Author: faisstm
 */

#include "sdrc.h"

#include "../common/common.h"

#define SDRC_BASE_ADDR 		0x6D000000
#define SDRC_RESET_DONE_BIT 0x1
#define SDRC_SOFT_RESET		0x2

#define SDRC_BASE 				READ_REGISTER(SDRC_BASE_ADDR)
#define SDRC_SYSCONFIG			READ_REGISTER_OFFSET(SDRC_BASE_ADDR, 0x10)
#define SDRC_SYSSTATUS			READ_REGISTER_OFFSET(SDRC_BASE_ADDR, 0x14)
#define SDRC_CS1_CFG			READ_REGISTER_OFFSET(SDRC_BASE_ADDR, 0x40)
#define SDRC_SHARING			READ_REGISTER_OFFSET(SDRC_BASE_ADDR, 0x44)

#define SDRC_MCFG_0				READ_REGISTER_OFFSET(SDRC_BASE_ADDR, 0x80)
#define SDRC_MCFG_1				READ_REGISTER_OFFSET(SDRC_BASE_ADDR, 0xB0)

uint8_t initSDRam(void) {

	BIT_SET(SDRC_MCFG_0, 0x100 << 8);
	BIT_CLEAR(SDRC_MCFG_0, 0x20 << 8);

	BIT_SET(SDRC_MCFG_1, 0x100 << 8);

	return 0;

}

