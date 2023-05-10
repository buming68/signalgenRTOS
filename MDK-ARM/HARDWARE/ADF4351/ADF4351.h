
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#ifndef _ADF4351_H_
#define _ADF4351_H_


#include "system.h"
#include "stm32f1xx_hal_gpio.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


#define ADF4351_RF_OFF	((u32)0XEC801C)

#define ADF4351_CLK PBout(15)
#define ADF4351_OUTPUT_DATA PBout(14)
#define ADF4351_LE PBout(13)
#define ADF4351_CE PBout(12)

#define ADF4351_INPUT_DATA PBin(14)


void ADF4351Init(void);
void ReadToADF4351(uint8_t count, uint8_t *buf);
void WriteToADF4351(uint8_t count, uint8_t *buf);
void WriteOneRegToADF4351(uint32_t Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre);		//	(xx.x) M Hz

#endif

