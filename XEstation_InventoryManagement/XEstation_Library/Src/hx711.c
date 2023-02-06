//#include "User_hx711.h"

#include "hx711.h"
#include "hx711Config.h"
#if (_HX711_USE_FREERTOS == 1)
#include "cmsis_os.h"
#define hx711_delay(x)    osDelay(x)
#else
#define hx711_delay(x)    HAL_Delay(x)
#endif

//#############################################################################################
void hx711_delay_us(void)
{
  uint32_t delay = _HX711_DELAY_US_LOOP;
  while (delay > 0)
  {
    delay--;
//    __nop(); __nop(); __nop(); __nop();
  }
}
//#############################################################################################
void hx711_lock(hx711_t *hx711)
{
  while (hx711->lock)
    hx711_delay(1);
  hx711->lock = 1;      
}
//#############################################################################################
void hx711_unlock(hx711_t *hx711)
{
  hx711->lock = 0;
}
//#############################################################################################
//void hx711_init(hx711_t *hx711, uint8_t hx711_idx, GPIO_TypeDef** HX711_GPIO_Port, uint16_t* HX711_Pin, char channel)
//void hx711_init(hx711_t *hx711, GPIO_TypeDef *clk_gpio, uint16_t clk_pin, GPIO_TypeDef *dat_gpio, uint16_t dat_pin, char channel)
void hx711_init(hx711_t *hx711, uint8_t hx711_idx, char channel)
{

	GPIO_TypeDef* HX711_GPIO_Port[][2] = {{PD01_sck_GPIO_Port, PD01_dout_GPIO_Port},
											{PD02_sck_GPIO_Port, PD02_dout_GPIO_Port},
											{PD03_sck_GPIO_Port, PD03_dout_GPIO_Port},
											{PD04_sck_GPIO_Port, PD04_dout_GPIO_Port},
											{PD05_sck_GPIO_Port, PD05_dout_GPIO_Port},
											{EXIC01_sck_GPIO_Port, EXIC01_dout_GPIO_Port},
											{EXPD01_sck_GPIO_Port, EXPD01_dout_GPIO_Port}};

	uint16_t HX711_Pin[][2] = {{PD01_sck_Pin, PD01_dout_Pin},
								{PD02_sck_Pin, PD02_dout_Pin},
								{PD03_sck_Pin, PD03_dout_Pin},
								{PD04_sck_Pin, PD04_dout_Pin},
								{PD05_sck_Pin, PD05_dout_Pin},
								{EXIC01_sck_Pin, EXIC01_dout_Pin},
								{EXPD01_sck_Pin, EXPD01_dout_Pin}};
  hx711_lock(hx711);

	GPIO_TypeDef* clk_gpio = HX711_GPIO_Port[hx711_idx][SCK];
	uint16_t clk_pin = HX711_Pin[hx711_idx][SCK];
	GPIO_TypeDef* dat_gpio = HX711_GPIO_Port[hx711_idx][DOUT];
	uint16_t dat_pin = HX711_Pin[hx711_idx][DOUT];

//	printf("clk_gpio : %x\n", clk_gpio);
	printf("clk_pin : %x\n", clk_pin);
//	printf("dat_gpio : %x\n", dat_gpio);
//	printf("dat_pin : %x\n", dat_pin);

  hx711->clk_gpio = HX711_GPIO_Port[hx711_idx][SCK];
  hx711->clk_pin = clk_pin;

  printf("hx711->clk_pin : %x\n", hx711->clk_pin);
//  hx711->clk_pin = HX711_Pin[hx711_idx][SCK];
//  hx711->clk_pin = PD01_sck_Pin;
  printf("hx711->clk_pin : %d\n", hx711->clk_pin);
  hx711->dat_gpio = HX711_GPIO_Port[hx711_idx][DOUT];
  hx711->dat_pin = HX711_Pin[hx711_idx][DOUT];
  hx711->channel = channel;
	printf("4\n");

  GPIO_InitTypeDef  gpio = {0};
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio.Pin = HX711_Pin[hx711_idx][SCK];
  printf("5\n");
  HAL_GPIO_Init(HX711_GPIO_Port[hx711_idx][SCK], &gpio);
  gpio.Mode = GPIO_MODE_INPUT;
  gpio.Pull = GPIO_PULLUP;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio.Pin = HX711_Pin[hx711_idx][DOUT];
  HAL_GPIO_Init(HX711_GPIO_Port[hx711_idx][DOUT], &gpio);
  printf("9\n");
  printf("hx711->clk_gpio : %x\n", hx711->clk_gpio);
  printf("hx711->clk_pin : %x\n", hx711->clk_pin);
  printf("*hx711->clk_gpio : %x\n", *(GPIO_TypeDef *)hx711->clk_gpio);
  printf("*hx711->clk_pin : %x\n", (uint16_t*)hx711->clk_pin);
  HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
  printf("10\n");

  hx711_delay(10);
  printf("11\n");

  HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
  hx711_delay(10);  
	printf("3\n");
  hx711_value(hx711);
  hx711_value(hx711);
  hx711_unlock(hx711); 
}
//#############################################################################################
int32_t hx711_value(hx711_t *hx711)
{
  uint32_t data = 0;
  uint32_t  startTime = HAL_GetTick();
  while(HAL_GPIO_ReadPin(hx711->dat_gpio, hx711->dat_pin) == GPIO_PIN_SET)
  {
    hx711_delay(1);
    if(HAL_GetTick() - startTime > 150)
      return 0;
  }
  switch(hx711->channel){
  case 'A':
//	  PD_SCK_Pulses = 24;
	  break;
  case 'B':
//	  PD_SCK_Pulses = 24;
	  HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
	  hx711_delay_us();
	  if(HAL_GPIO_ReadPin(hx711->dat_gpio, hx711->dat_pin) == GPIO_PIN_SET)
		data ++;
	  HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
	  hx711_delay_us();
	  if(HAL_GPIO_ReadPin(hx711->dat_gpio, hx711->dat_pin) == GPIO_PIN_SET)
		data ++;
	  break;
  }
  for(int8_t i=0; i<24 ; i++)
  {
    HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);   
    hx711_delay_us();
    HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
    hx711_delay_us();
    data = data << 1;    
    if(HAL_GPIO_ReadPin(hx711->dat_gpio, hx711->dat_pin) == GPIO_PIN_SET)
      data ++;
  }
  data = data ^ 0x800000; 
  HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);   
  hx711_delay_us();
  HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
  hx711_delay_us();
  return data;    
}
//#############################################################################################
int32_t hx711_value_ave(hx711_t *hx711, uint16_t sample)
{
  hx711_lock(hx711);
  int64_t  ave = 0;
  for(uint16_t i=0 ; i<sample ; i++)
  {
    ave += hx711_value(hx711);
    hx711_delay(5);
  }
  int32_t answer = (int32_t)(ave / sample);
  hx711_unlock(hx711);
  return answer;
}
//#############################################################################################
void hx711_tare(hx711_t *hx711, uint16_t sample)
{
  hx711_lock(hx711);
  int64_t  ave = 0;
  for(uint16_t i=0 ; i<sample ; i++)
  {
    ave += hx711_value(hx711);
    hx711_delay(5);
  }
  hx711->offset = (int32_t)(ave / sample);
  hx711_unlock(hx711);
}
//#############################################################################################
void hx711_calibration(hx711_t *hx711, int32_t noload_raw, int32_t load_raw, float scale)
{
  hx711_lock(hx711);
  hx711->offset = noload_raw;
  hx711->coef = (load_raw - noload_raw) / scale;  
  hx711_unlock(hx711);
}
//#############################################################################################
float hx711_weight(hx711_t *hx711, uint16_t sample)
{
  hx711_lock(hx711);
  int64_t  ave = 0;
  for(uint16_t i=0 ; i<sample ; i++)
  {
    ave += hx711_value(hx711);
    hx711_delay(5);
  }
  int32_t data = (int32_t)(ave / sample);
  float answer =  (data - hx711->offset) / hx711->coef;
  hx711_unlock(hx711);
  return answer;
}
//#############################################################################################
void hx711_coef_set(hx711_t *hx711, float coef)
{
  hx711->coef = coef;  
}
//#############################################################################################
float hx711_coef_get(hx711_t *hx711)
{
  return hx711->coef;  
}
//#############################################################################################
void hx711_power_down(hx711_t *hx711)
{
  HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
  hx711_delay(1);  
}
//#############################################################################################
void hx711_power_up(hx711_t *hx711)
{
  HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
}
//#############################################################################################
