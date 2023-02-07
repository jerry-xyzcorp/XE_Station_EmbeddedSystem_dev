/*
 * User_TFminiS.c
 */
#include "User_TFminiS.h"
#include "main.h"
#include "stm32f4xx_hal.h"

#define OBTAIN_FIRMWARE_VERSION 0
#define SET_I2C 				1
#define SAVE_SETTING			2
#define SYSTEM_RESET			3
#define SET_OUTPUT_CM		    4
#define SET_OUTPUT_MM			5
#define OBTAIN_DATA_CM			6
#define OBTAIN_DATA_MM			7


uint8_t TFminiS_req_packet_list[][5] =
				{
						{0x5a, 0x04, 0x01, 0x5f, 0x00},// Obtain firmware version
						{0x5a, 0x05, 0x0a, 0x01, 0x00},// I2C setting - Communication interface
						{0x5a, 0x04, 0x11, 0x6f, 0x00},// save settings
						{0x5a, 0x04, 0x02, 0x60, 0x00},// system reset
						{0x5a, 0x05, 0x05, 0x01, 0x65},// set Output format: standard 9 bytes(cm)
						{0x5a, 0x05, 0x05, 0x06, 0x6a},// set Output format: standard 9 bytes(mm)
						{0x5a, 0x05, 0x00, 0x01, 0x60},// obtain data frame(9bytes-cm)
						{0x5a, 0x05, 0x00, 0x06, 0x65},// obtain data frame(9bytes-mm)
				};

void initTFminiS(void)
{
	is_running = false;
	streaming_period = 100; //ms

	// init OFFSET -> need to be tuned!
	uint16_t TFminiS_offset_info[] = {30, 31, 31, 28, 30, 31, 31, 28, 30, 31, 31};
	uint8_t stock_size_info[] = {90,90,90,20,20,20,20,20,20,20,20};

	// init initial distance, number of stocks, offset
	for(uint8_t i=0; i<sizeof(TFminiS_info); i++){
		TFminiS_info[i][_DIST] = 0;
		TFminiS_info[i][_STOCK_NUM] = 0;
		TFminiS_info[i][_OFFSET] = TFminiS_offset_info[i];
	}
}

bool set_thresh(uint8_t sensor_id, uint16_t threashold)
{
	uint8_t ID = sensor_id - ID_OFFSET;
	TFminiS_info[ID][_OFFSET] = threashold;

	if (TFminiS_info[ID][_OFFSET] = threashold)
		return true;
	else
		return false;
}

uint8_t* initPacket(uint8_t mode)
{
	// packet order : [header] [len] ... [checksum]


	// calc checksum



	return TFminiS_req_packet_list[mode];
}

void getDistance(uint8_t sensor_id)
{
	uint8_t TFminiS_req_packet = initPacket(OBTAIN_FIRMWARE_VERSION);

	uint8_t buf[1] = {GetAddr(sensor_id)};
	uint8_t rxbuf[1] = {0};
	uint8_t rxdata[20] = {0};

	// 1. write
	uint8_t ret = HAL_I2C_Master_Transmit(&hi2c1, GetAddr(sensor_id), TFminiS_req_packet, sizeof(TFminiS_req_packet), 10);

	if ( ret != HAL_OK ) {
		error_value = ret;
		error[TX_ERROR] = true;
	} else {
		HAL_Delay(100);
		// 2. read
		 ret = HAL_I2C_Master_Transmit(&hi2c1, GetAddr(sensor_id)|0x01, buf, sizeof(buf), 10);

		 for(int i=0; i<sizeof(rxdata); i++){
			 if ( ret == HAL_OK ){
				 ret = HAL_I2C_Master_Receive(&hi2c1, GetAddr(sensor_id)|0x01, rxbuf, sizeof(rxbuf), HAL_MAX_DELAY);
				 if ( ret == HAL_OK ){
					 rxdata[i] = rxbuf[0];
				 }
			 }
		 }
	}

	// calc & update distance
	uint8_t ID = sensor_id - ID_OFFSET;
	for(int i=0; i<sizeof(rxdata); i++){
		if(rxdata[i] == 0x59 && rxdata[i+1] == 0x59){
			TFminiS_info[ID][_DIST] = rxdata[i+2]|(rxdata[i+3]<<8);
			break;
		}
	}
}

uint8_t getStock(uint8_t sensor_id)
{
	uint8_t ID = sensor_id - ID_OFFSET;
	TFminiS_info[ID][_STOCK_NUM] = (stock_size_info[ID]-TFminiS_info[ID][_DIST])/stock_size_info[ID];
	//!!!!
	return (uint8_t)TFminiS_info[ID][_STOCK_NUM];
}

void TFminiSTest(void)
{
	uint8_t TFminiS_req_packet = TFminiS_req_packet_list[SET_I2C];

	 // UART
	for(int i=0; i<sizeof(TFminiS_req_packet); i++){
		TFminiS_req_packet[sizeof(TFminiS_req_packet)-1] += TFminiS_req_packet[i];
	}

	HAL_UART_Transmit(&huart3, (uint8_t *)TFminiS_req_packet, sizeof(TFminiS_req_packet), 100);

	// request packet
	HAL_UART_Transmit(&huart2, (uint8_t *)TFminiS_req_packet, sizeof(TFminiS_req_packet), 100);

	for(int i=0; i<20; i++){
		uint8_t buf = 0;
		HAL_UART_Receive(&huart2, &buf, sizeof(buf), 10);
		HAL_UART_Transmit(&huart3, &buf, sizeof(buf), 100);
	}

	HAL_Delay(500);
}
