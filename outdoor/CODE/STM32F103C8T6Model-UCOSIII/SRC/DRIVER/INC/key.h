
#ifndef __KEY_H_
#define __KEY_H_

#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>
#include "app.h"

/**@name �û�API����
* @{
*/
void Key_Init(void);
uint8_t Key_Read(void);
uint8_t Scanf_Line(void);
uint8_t Scanf_Column(void);
void Key(void);
char return_Keyvalue(void);
void InputNumber(uint8_t x, uint8_t y);
/**@}
*/

#endif
