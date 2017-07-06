
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//用PB4时要禁用JTAG，禁用要先开AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
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
* @param[in] mode:模式
* @retf key.h
*/
void Key_Colunm(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	if(mode == 1)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	else if(mode == 0)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	if(mode == 0)
		GPIO_ResetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	else if(mode == 1)
		GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}

/**
* @brief 行初始化
* @param[in] mode:模式
* @retf key.h
*/
void Key_Line(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	if(mode == 1)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	else if(mode == 0)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	if(mode == 0)
		GPIO_ResetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);
	else if(mode ==1)
		GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);
}

/**
* @brief 扫描矩阵列
* @return ret:列值
* @retf key.h
*/
uint8_t Scanf_Line(void)
{
	uint16_t read_value = 0;
	uint8_t ret = 0;
	Key_Colunm(1);
	Key_Line(0);
	read_value = GPIO_ReadInputData(GPIOA);
	read_value &= 0x0FE0;
	switch(read_value)
	{
		case 0x0060:ret = 1;break;
		case 0x00A0:ret = 2;break;
		case 0x00C0:ret = 3;break;
		default:ret = 0;break;
	}
	return ret;
}

/**
* @brief 扫描矩阵行
* @return ret:行值
* @retf key.h
*/
uint8_t Scanf_Column(void)
{
	uint16_t read_value = 0;
	uint8_t ret = 0;
	Key_Colunm(0);
	Key_Line(1);
	read_value = GPIO_ReadInputData(GPIOA);
	read_value &= 0x0FE0;
	switch(read_value)
	{
		case 0x0700:ret = 0;break;
		case 0x0B00:ret = 3;break;
		case 0x0D00:ret = 6;break;
		case 0x0E00:ret = 9;break;
		default:ret = 13;break;
	}
	do{
		read_value = GPIO_ReadInputData(GPIOA);
		read_value &= 0x0FE0;
	}while(read_value != 0x0F00);
	return ret;
}

/**
* @brief 返回按键字符串
* @return *ref:字符串首地址
* @retf key.h
*/
char *return_Keyvalue(void)
{
	uint8_t value = 0;
	value = Scanf_Line() + Scanf_Column();
	switch(value)
	{
		case 1:return "1";
		case 2:return "2";
		case 3:return "3";
		case 4:return "4";
		case 5:return "5";
		case 6:return "6";
		case 7:return "7";
		case 8:return "8";
		case 9:return "9";
		case 10:return "*";
		case 11:return "0";
		case 12:return "#";
		default:return NULL;
	}
}

/**
* @brief 输入密码函数
* @return NULL
* @retf key.h
*/
void inputPasswd(void)
{
	uint8_t char_count = 0;
	char *thischar = NULL;
	while(1)
	{
		thischar = return_Keyvalue();
		if(!strcmp(thischar, "#"))
			break;
		else if(thischar != NULL)
		{
			char_count++;
			if(char_count == 20)
				break;
			strcat(passwd, thischar);
		}
	}
}
