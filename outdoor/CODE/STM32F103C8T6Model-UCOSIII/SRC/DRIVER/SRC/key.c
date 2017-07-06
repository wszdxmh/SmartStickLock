
#include "key.h"

char passwd[20] = "";

/**
* @brief 按键初始化
* @return NULL
* @retf key.h
*/
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
* @brief 读取键值
* @return 键值
* @retf key.h
*/
uint8_t Key_Read(void)
{
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13))
		return 1;
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14))
		return 2;
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15))
		return 3;
	return 0;
}

/**
* @brief 列初始化
* @param[in] mode:模式	1:输入 0:输出
* @retf key.h
*/
void Key_Colunm(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	if(mode == 1)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	else if(mode == 0)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	if(mode == 0)
		GPIO_ResetBits(GPIOB, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
	else if(mode == 1)
		GPIO_SetBits(GPIOB, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
}

/**
* @brief 行初始化
* @param[in] mode:模式	1:输入 0:输出
* @retf key.h
*/
void Key_Line(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//用PB4时要禁用JTAG，禁用要先开AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	if(mode == 1)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	else if(mode == 0)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	if(mode == 0)
		GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
	else if(mode ==1)
		GPIO_SetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}

/**
* @brief 扫描矩阵列
* @return ret:列值
* @retf key.h
*/
uint8_t Scanf_Line(void)
{
	uint8_t read_vaule = 0;
	Key_Colunm(1);
	Key_Line(0);
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
		return 1;
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8))
		return 2;
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		return 3;
	return 0;
}

/**
* @brief 扫描矩阵行
* @return ret:行值
* @retf key.h
*/
uint8_t Scanf_Column(void)
{
	uint8_t read_valuer = 0;
	Key_Colunm(0);
	Key_Line(1);
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3))
		return 0;
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
		return 3;
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5))
		return 6;
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
		return 9;
	return 13;
}

/**
* @brief 返回按键字符串
* @return *ref:字符串首地址
* @retf key.h
*/
char return_Keyvalue(void)
{
	uint8_t value = 0;
	value = Scanf_Line() + Scanf_Column();
	switch(value)
	{
		case 1:return 1;
		case 2:return 2;
		case 3:return 3;
		case 4:return 4;
		case 5:return 5;
		case 6:return 6;
		case 7:return 7;
		case 8:return 8;
		case 9:return 9;
		case 10:return 10;
		case 11:return 0;
		case 12:return 11;
		default:return 12;
	}
}

extern user_flag User_Flag;
extern uint8_t Key_Buffer[12];
void InputNumber(uint8_t x, uint8_t y)
{
	uint8_t passwd_flag = 0;
	uint8_t cnt = 0;
	uint8_t count = 0;
	uint8_t scanf = 0;
	uint8_t num = 0;
	if (scanf != 11)
	{
		scanf = return_Keyvalue();
		if(scanf != 12)
		{
			Clear_OLED_Zone(0,6,128,7);
			passwd_flag = 0;
			cnt = 0;
			count = 0;
			Key_Buffer[num++] = scanf;
			if (num == 11)
			{
				Key_Buffer[11] = 11;
				scanf = 11;
				num = 0;
			}
			if (Key_Buffer[0] != 10)
				Display_P16x16_Number(x,4,10);
			else
				Display_P16x16_Number(x,4,scanf);
			x += 8;
		}
	}
	else
	{
		if (Key_Buffer[0] != 10)
		{
			while(*(Key_Buffer + count) != 11)
			{
				if (passwd_flag < 6)
				{
					if (*(Key_Buffer + count) == *(User_Flag.passwd + cnt))
						passwd_flag++, cnt++;
					else if (*(Key_Buffer + count) == *(User_Flag.passwd))
						passwd_flag = 1, cnt = 1;
					else
						passwd_flag = 0, cnt = 0;
				}
				count++;
			}
		}
//		if (passwd_flag == 4)
		Display_P16x16_Number(30,6,1);
		scanf = 12;
		x = 10;
		num = 0;
		Clear_OLED_Zone(0,4,128,5);
		if (passwd_flag == 6)
		{
			User_Flag.mode |= 0x01;
			Display_P16x16_Number(50,6,passwd_flag);
		}
	}
}
