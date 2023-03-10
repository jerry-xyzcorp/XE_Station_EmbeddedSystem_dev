#ifndef INC_USER_TFMINIS_H_
#define INC_USER_TFMINIS_H_

#ifdef __cplusplus
extern "C" {
#endif
/*
 * User_TFminiS.h
 * DEFAULT address: 0x10
 * address range  : 0x01~0x7F
 */

#include "main.h"
#include "stm32f4xx_hal.h"

// I2C1 : BK, CP
// I2C2 : LD
//******* Slave address
#define GetAddr(sensor_id) (sensor_id << 1)
#define ID_OFFSET		0x21

#define OBTAIN_FIRMWARE_VERSION 0
#define SET_I2C 				1
#define SAVE_SETTING			2
#define SYSTEM_RESET			3
#define SET_OUTPUT_CM		    4
#define SET_OUTPUT_MM			5
#define OBTAIN_DATA_CM			6
#define OBTAIN_DATA_MM			7
#define SET_12C_ADDR			8

#define I2C_ADDR 				3

// sensor ID
#define BK01		0x21
#define BK02		0x22
#define BK03		0x23
#define CP01		0x24
#define CP02		0x25
#define CP03		0x26
#define CP04		0x27
#define LD01		0x28
#define LD02		0x29
#define LD03		0x2a
#define LD04		0x2b

#define _DIST 		0
#define _STOCK_NUM 	1
#define _OFFSET		2
#define LEN 		3

#define BK_NUM			3
#define CP_NUM			4
#define LD_NUM			4
#define TFminiS_NUM		11

// Error list
#define TX_ERROR	1 // Error TX

extern I2C_HandleTypeDef hi2c1;
bool	error[10];
uint8_t error_value;

bool 		is_running;
uint16_t 	streaming_period;
uint16_t 	TFminiS_info[TFminiS_NUM][LEN];
uint16_t 	TFminiS_offset_info[TFminiS_NUM];
uint8_t		stock_size_info[LEN];

void 		initTFminiS(void);
bool 		set_thresh(uint8_t sensor_id, uint16_t threashold);
uint8_t* 	initPacket(uint8_t mode);
void	 	getDistance(uint8_t sensor_id);
uint8_t 	getStock(uint8_t sensor_id);
void 		TFminiSTest(void);

uint8_t* 	TFminiS_I2C(uint8_t sensor_id, uint8_t cmd);
void 		TFminiS_setting(void);
void 		TFminiS_uart_setting(uint8_t cmd);


#ifdef __cplusplus
}
#endif

#endif
