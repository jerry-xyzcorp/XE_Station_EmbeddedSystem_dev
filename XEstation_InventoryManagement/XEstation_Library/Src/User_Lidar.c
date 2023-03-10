/*
 * User_Lidar.c
 */
#include <User_Lidar.h>
#include <time.h>

extern UART_HandleTypeDef huart3; // PC
extern UART_HandleTypeDef huart5; // Lidar1
extern UART_HandleTypeDef huart6; // Lidar2

// lidar
uint8_t lidar_req_packet[8]; 		// lidar request packet
uint8_t lidar_res_packet[14407] = {0};	// lidar response packet

char uartBuf[4000];			//used as buffer for the string to be send to PC

uint8_t response_packet_protocol_run_3d[] = {0x5a, 0x77, 0xff, 0x41, 0x38, 0x08};
uint8_t recv_pre[PACKET_LEN] = {0,};
bool receive_start_flag = false;
bool receive_end_flag = false;
bool RxCpltCallback_flag = false;
uint32_t receive_packet_cnt = 0;

time_t start;
time_t end;


uint8_t data = 0x00;
uint32_t cnt = 0;
uint32_t success_cnt = 0;
uint32_t fail_cnt = 0;

void init_Lidar(void)
{
	  requestPacket(STOP, NO_VALUE);
	  HAL_Delay(2000);
	HAL_UART_Receive_IT(&huart5, lidar_res_packet, sizeof(lidar_res_packet));
//	  HAL_UART_Receive_IT(&huart5, &data, sizeof(data));
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if (huart->Instance == UART5)
//	{
////		end = time(NULL);
//		printf("cnt : %d \n", cnt);
//
//		if(cnt == 1000000)
//			cnt = 0;
//		else
//			cnt += 1;
//		//		printf("UART5 receive interrupt!, %lf\n", time(NULL));
//
////	    time_t start2 = time(NULL);
//
////		time_t end2 = time(NULL);
////		printf("TRANSMIT-RECEIVE TIME time : %lf\n", (double)(end - start));
//
////		printf("ALL TIME time : %lf\n", (double)(end2 - start2));
//		RxCpltCallback_flag = true;
//
//	    // enable interrupt again
////		response_packet(data);
////	    HAL_UART_Receive_IT(&huart5, &data, sizeof(data));
//	    HAL_UART_Receive_IT(&huart5, lidar_res_packet, sizeof(lidar_res_packet));
////	    requestPacket(RUN_3D_MODE, NO_VALUE);
//
////	    requestPacket(RUN_3D_MODE, NO_VALUE);
//
//	}
//
//}

void Lidar3dTest(void)
{
	uint32_t baud_rate = 57600;
//	requestPacket(SET_BAUDRATE, baud_rate);
	HAL_Delay(2000);

//	  HAL_NVIC_DisableIRQ(UART5_IRQn); //Rx Callback 함수 Disable
//		requestPacket(GET_DEVICE_INFO, baud_rate);
//	  HAL_NVIC_EnableIRQ(UART5_IRQn);  //Rx callback 함수 enable

	  requestPacket(RUN_3D_MODE, NO_VALUE);
	  while(1){
		process_lidarData_3d(lidar_res_packet);
		 HAL_Delay(20);
	  }
}


uint8_t CalcChecksum(uint8_t* packet, uint32_t packet_size)
{
	uint8_t CheckSum = 0x00;

	for(int i = PAYLOAD_LENGTH_LSB_INDEX; i < packet_size-1; i++)
	{
		CheckSum ^= packet[i];
	}
	return CheckSum;
}

void requestPacket(uint8_t packet_cmd, uint32_t value)
{
	uint8_t packet_value = 0x00;

	switch(packet_cmd){
	case GET_DEVICE_INFO: // Get Device Info
		packet_cmd = 0x10;
		break;
	case RUN_2D_MODE: // Run 2D Mode
		packet_cmd = 0x01;
		break;
	case RUN_3D_MODE: // Run 3D Mode
		packet_cmd = 0x08;
		break;
	case RUN_DUAL_MODE: // Run Dual Mode
		packet_cmd = 0x07;
		break;
	case STOP: // Stop
		packet_cmd = 0x02;
		break;
	case SET_3D_PULSE_DURATION_REQUEST: // Set 3D Pulse Duration Request
		packet_cmd = 0x0c;

		break;
	case SET_FREQ_REQUEST: // Frequency Setting Request
		packet_cmd = 0x0f;
		break;
	case SET_SENSITIVITY_REQUEST: // Sensitivity Setting Request
		packet_cmd = 0x11;
		break;
	case SET_BAUDRATE: // Set Serial Baud Rate
		packet_cmd = 0x12;
		switch(value){
		case 57600:
			packet_value = 0x39;
			huart5.Init.BaudRate = 57600;
			break;
		case 115200:
			packet_value = 0xaa;
			huart5.Init.BaudRate = 115200;
			break;
		case 250000:
			packet_value = 0x77;
			huart5.Init.BaudRate = 250000;
			break;
		case 3000000:
			packet_value = 0x55;
			huart5.Init.BaudRate = 3000000;
			break;
		}
		if (HAL_UART_Init(&huart5) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	}

	uint8_t lidar_req_packet[] = {0x5a, 0x77, 0xff, 0x02, 0x00, packet_cmd, packet_value, 0x00};

	// calc checksum
	lidar_req_packet[7] = CalcChecksum(lidar_req_packet, sizeof(lidar_req_packet));


	 //print request packet
//	for(int i=0; i<sizeof(lidar_req_packet); i++){
//		sprintf(uartBuf, "%0.2x \r", lidar_req_packet[i]);			//convert to string
//		HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);
//	}
//
//	sprintf(uartBuf, "-> request packet\n");			//convert to string
//	HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);
//	HAL_UART_Transmit(&huart3, (uint8_t *)lidar_req_packet, sizeof(lidar_req_packet), 100);

	// request packet
	start = time(NULL);
	HAL_UART_Transmit(&huart5, (uint8_t *)lidar_req_packet, sizeof(lidar_req_packet), 100);
}


uint8_t process_lidarData_3d(uint8_t* lidarData_3d)
{
	if(RxCpltCallback_flag == true){
		if(lidarData_3d[PACKET_HEADER_1] == response_packet_protocol_run_3d[PACKET_HEADER_1] &&
			lidarData_3d[PACKET_HEADER_2] == response_packet_protocol_run_3d[PACKET_HEADER_2] &&
			lidarData_3d[PACKET_HEADER_3] == response_packet_protocol_run_3d[PACKET_HEADER_3] &&
			lidarData_3d[PAYLOAD_LENGTH_LSB_INDEX] == response_packet_protocol_run_3d[PAYLOAD_LENGTH_LSB_INDEX] &&
			lidarData_3d[PAYLOAD_LENGTH_MSB_INDEX] == response_packet_protocol_run_3d[PAYLOAD_LENGTH_MSB_INDEX] &&
			lidarData_3d[PAYLOAD_HEADER] == response_packet_protocol_run_3d[PAYLOAD_HEADER])
			{
				uint16_t distance_arr[FORMAT_3D_ARR] = {0}; // malloc

				for(int i_arr=0; i_arr<FORMAT_3D_ARR/2; i_arr++){
					uint16_t idx = PAYLOAD_HEADER + 3*i_arr;

					distance_arr[2*i_arr] = 16*16*(lidarData_3d[idx]>>4) + 16*(lidarData_3d[idx] & 0x0f) + (lidarData_3d[idx+1]>>4);
					distance_arr[2*i_arr+1] = 16*16*(lidarData_3d[idx+1] & 0x0f) + 16*(lidarData_3d[idx+2]>>4) + (lidarData_3d[idx+2] & 0x0f);
					//	    HAL_UART_Transmit(&huart3, lidar_res_packet, sizeof(lidar_res_packet), 100);

			//		printf("arr[%d]:%d\n", 2*i_arr, distance_arr[2*i_arr]);
			//		printf("arr[%d]:%d\n", 2*i_arr+1, distance_arr[2*i_arr+1]);

				}
				// calc checksum
				uint8_t chksum = CalcChecksum(lidarData_3d, 14407);

				if(lidarData_3d[PACKET_LEN+FORMAT_3D_LEN] == chksum){ // need to add init byte num
//					printf("!!!!!!!!!!!!!!!!!!!!!!!checksum is correct!\n");
					success_cnt += 1;
				}
				else{
//					printf("recv : %0.2x \r calc : %0.2x \n", lidarData_3d[PACKET_LEN+FORMAT_3D_LEN], chksum);
					fail_cnt += 1;
				}
				printf("success percentage : %lf\n", (float)success_cnt/(success_cnt+fail_cnt)*100.0);
				RxCpltCallback_flag == false;
				return distance_arr;
			}
	}
}



