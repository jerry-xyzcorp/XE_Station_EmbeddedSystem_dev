#ifndef HX711_H_
#define HX711_H_


/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw
  
  Version:    1.1.1
  
  
  Reversion History:
  
  (1.1.1):
    Add power down/up.
  (1.1.0):
    Add structure, Add calibration, Add weight, change names, ...
  (1.0.0):
    First Release.
*/

#ifdef __cplusplus
extern "C" {
#endif
  
#include "main.h"

//####################################################################################################################
#define SCK  0
#define DOUT 1

//GPIO_TypeDef* HX711_GPIO_Port[][2];
//uint16_t HX711_Pin[][2];

//####################################################################################################################

typedef struct
{
  GPIO_TypeDef  *clk_gpio;
  GPIO_TypeDef  *dat_gpio;
  uint16_t      clk_pin;
  uint16_t      dat_pin;
  int32_t       offset;
  float         coef;
  uint8_t       lock;    
  char 			channel;
  
}hx711_t;

//####################################################################################################################
//void hx711_init(hx711_t *hx711, uint8_t hx711_idx, GPIO_TypeDef* HX711_GPIO_Port, uint16_t HX711_Pin, char channel);
//void        hx711_init(hx711_t *hx711, GPIO_TypeDef *clk_gpio, uint16_t clk_pin, GPIO_TypeDef *dat_gpio, uint16_t dat_pin, char channel);
void 		hx711_init(hx711_t *hx711, uint8_t hx711_idx, char channel);

int32_t     hx711_value(hx711_t *hx711);
int32_t     hx711_value_ave(hx711_t *hx711, uint16_t sample);

void        hx711_coef_set(hx711_t *hx711, float coef);
float       hx711_coef_get(hx711_t *hx711);
void        hx711_calibration(hx711_t *hx711, int32_t value_noload, int32_t value_load, float scale);
void        hx711_tare(hx711_t *hx711, uint16_t sample);
float       hx711_weight(hx711_t *hx711, uint16_t sample);
void        hx711_power_down(hx711_t *hx711);
void        hx711_power_up(hx711_t *hx711);

//####################################################################################################################

#ifdef __cplusplus
}
#endif

#endif
