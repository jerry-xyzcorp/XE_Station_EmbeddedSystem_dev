/*
 * User_HD-SR04.c
 */
#include <User_HC_SR04.h>

GPIO_TypeDef * HCSR04_TIMER_GPIO_Port[] = {CP01_TIM_GPIO_Port,
											CF01_TIM_GPIO_Port,
											PD01_TIM_GPIO_Port,
											JG01_TIM_GPIO_Port,
											JG02_TIM_GPIO_Port,
											LD01_TIM_GPIO_Port,
											JG01_TIM_GPIO_Port,
											JG02_TIM_GPIO_Port};
uint16_t* HCSR04_TIMER_Pin[] = 			  {CP01_TIM_Pin,
											CF01_TIM_Pin,
											PD01_TIM_Pin,
											JG01_TIM_Pin,
											JG02_TIM_Pin,
											LD01_TIM_Pin,
											JG01_TIM_Pin,
											JG02_TIM_Pin};


uint32_t firstValue = 0;
uint32_t secondValue = 0;
uint32_t sub = 0;			//subtract value
uint8_t capturedOne = 0;
float distance  = 0.0;

void delay (uint16_t time)					//delay function in us
{
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while (__HAL_TIM_GET_COUNTER (&htim3) < time);	 //
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance ==TIM3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		int a = 1;
	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 |
		htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 |
		htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if (capturedOne==0) // check if first value is captured
		{
			firstValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);    //read the value
			capturedOne = 1;  									// change the value of the variable
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);  		//if the value is captured change the polarity to falling edge
		}

		else if (capturedOne==1)   // if the first value is captured
		{
			secondValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  							// reset the counter

			sub = secondValue-firstValue;				//subtraction of the two values

			distance = sub * .034/2;			//time in us * speed of sound in air in cm / 2 (the sound needs to go back to the sensor)
			capturedOne = 0; 					// set the capture value to 0

			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);  //change the polarity back to rising edge
			__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC2);											//disable the TIM1 interrupt
		}
	}
}

void HCSR04_Read(uint8_t sensor_id)
{
	uint8_t ID = sensor_id - OFFSET;


	HAL_GPIO_WritePin(HCSR04_TIMER_GPIO_Port[ID], HCSR04_TIMER_Pin[ID], GPIO_PIN_SET);    // TRIG set to HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(HCSR04_TIMER_GPIO_Port[ID], HCSR04_TIMER_Pin[ID], GPIO_PIN_RESET);  // TRIG set to LOW


	//Enable timer interrupt
	if(sensor_id == CKCP01){
		__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC2);
	}
	else if(sensor_id == CKCF01){
		__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC2);
	}
	else if(sensor_id == CKPD01){
		__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1);
	}
	else if(sensor_id == CKJG01 | sensor_id == CJL01){
		__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	}
	else if(sensor_id == CKJG02 | sensor_id == CJL02){
		__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC2);
	}
	else if(sensor_id == CKLD01){
		__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC3);
	}
}
