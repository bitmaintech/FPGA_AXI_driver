#ifndef __AXI_FPGA
#define __AXI_FPGA

#define LWFPGASLAVES_ADDR	0x43C00000
#define BASE_PHYS_ADDR          (LWFPGASLAVES_ADDR+0x00000000)
#define TOTAL_LEN		0x160


#define RESET_MANAGER_BASE_ADDR 0xFFD05000
#define BRGMODRST_ADDR		0x1C

#if 0
#define BASE_PHYS_ADDR		(LWFPGASLAVES_ADDR+0x00000000)
#define QN_RN_ADDR			(BASE_PHYS_ADDR + 0)
#define TW_ADDR				(BASE_PHYS_ADDR + 0x40)
#define QN_WD_ADDR			(BASE_PHYS_ADDR + 0x40)
#define BC_ADDR				(BASE_PHYS_ADDR + 0x40)

#define CONTROL_REG			(BASE_PHYS_ADDR + 0x100)
#define WORK_CALC_NUM		(BASE_PHYS_ADDR + 0x102)
#define COINBASE1_LEN		(BASE_PHYS_ADDR + 0x104)
#define COINBASE2_LEN		(BASE_PHYS_ADDR + 0x106)
#define NONCE2				(BASE_PHYS_ADDR + 0x108)
#define MERKLES_NUM			(BASE_PHYS_ADDR + 0x10C)
#define BLK_HDR_VERSION		(BASE_PHYS_ADDR + 0x110)
#define HDR_TIMESTAMP		(BASE_PHYS_ADDR + 0x114)
#define TARGET_BITS			(BASE_PHYS_ADDR + 0x118)
#define RESERVED			(BASE_PHYS_ADDR + 0x11C)
#define HDR_PREV_HASH		(BASE_PHYS_ADDR + 0x120)
#define MERKLE_ROOT			(BASE_PHYS_ADDR + 0x140)
#define MIDSTATE			(BASE_PHYS_ADDR + 0x160)
#define DATA_ADDR			(BASE_PHYS_ADDR + 0x10000)	//include Coinbase1, Work_nonce1, Work_nonce2, Coinbase2, Merkle_bin

#define TOTAL_LEN_1			0x180		//for all the data but except DATA
#define TOTAL_LEN_2			0x10000		//for DATA
#define TOTAL_LEN			0x20000
//control register define
#define TYPE				0x80
#define CAL_START_STOP		0x40
#define CAL_DONE			0x20
#define CAL_MODE			0x10
#define PADDING_DONE		0x08
#define PING_PONG_RAM_SEL	0x04

#define RESET_MANAGER_BASE_ADDR	0xFFD05000
#define BRGMODRST_ADDR			0x1C


#define QN_RN_LEN	52
#define TW_LEN		52
#define QN_WD_LEN	52
#define BC_LEN		8
#endif


#endif
