
#include "hmciic.h"

/**
  **********************************
  * @brief  HMC5883��ʼ��
  * @param  None
  * @retval None
  **********************************
*/
void HMC_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

/**
  **********************************
  * @brief  ����IIC��ʼ�ź�
  * @param  None
  * @retval None
  **********************************
*/
void HMC_IIC_Start(void)
{
	HMC_SDA_OUT();//sda�����
	HMC_IIC_SDA = 1;
	HMC_IIC_SCL = 1;
	HMC_IIC_DELAY();
	HMC_IIC_SDA = 0;//START:when CLK is high,DATA change form high to low
	HMC_IIC_DELAY();
	HMC_IIC_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ��������
}

/**
  **********************************
  * @brief  ����IICֹͣ�ź�
  * @param  None
  * @retval None
  **********************************
*/
void HMC_IIC_Stop(void)
{
	HMC_SDA_OUT();//sda�����
	HMC_IIC_SCL = 0;
	HMC_IIC_SDA = 0;//STOP:when CLK is high DATA change form low to high
	HMC_IIC_DELAY();
	HMC_IIC_SCL = 1;
	HMC_IIC_SDA = 1;//����I2C���߽����ź�
	HMC_IIC_DELAY();
}

/**
  **********************************
  * @brief  �ȴ�Ӧ���ź�
  * @param  None
  * @retval 1������Ӧ��ʧ��	0������Ӧ��ɹ�
  **********************************
*/
unsigned char HMC_IIC_Wait_Ack(void)
{
	unsigned char ucErrTime = 0;
	HMC_SDA_IN();      //SDA����Ϊ����  
	HMC_IIC_SDA = 1;
	HMC_IIC_DELAY();
	HMC_IIC_SCL = 1;
	HMC_IIC_DELAY();
	while (HMC_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime>250)
		{
			HMC_IIC_Stop();
			return 1;
		}
	}
	HMC_IIC_SCL = 0;//ʱ�����0
	return 0;
} 

/**
  **********************************
  * @brief  ����Ӧ���ź�
  * @param  None
  * @retval None
  **********************************
*/
void HMC_IIC_Ack(void)
{
	HMC_IIC_SCL = 0;
	HMC_SDA_OUT();
	HMC_IIC_SDA = 0;
	HMC_IIC_DELAY();
	HMC_IIC_SCL = 1;
	HMC_IIC_DELAY();
	HMC_IIC_SCL = 0;
}

/**
  **********************************
  * @brief  ������Ӧ���ź�
  * @param  None
  * @retval None
  **********************************
*/
void HMC_IIC_NAck(void)
{
	HMC_IIC_SCL = 0;
	HMC_SDA_OUT();
	HMC_IIC_SDA = 1;
	HMC_IIC_DELAY();
	HMC_IIC_SCL = 1;
	HMC_IIC_DELAY();
	HMC_IIC_SCL = 0;
}

/**
  **********************************
  * @brief  ����һ���ֽ�
  * @param  None
  * @retval None
  **********************************
*/
void HMC_IIC_Send_Byte(unsigned char txd)
{                        
	unsigned char t;
	HMC_SDA_OUT();
	HMC_IIC_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
	for (t = 0;t<8;t++)
	{
		HMC_IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		HMC_IIC_SCL = 1;
		HMC_IIC_DELAY();
		HMC_IIC_SCL = 0;
		HMC_IIC_DELAY();
	}
}

/**
  **********************************
  * @brief  ��һ���ֽ�
  * @param  None
  * @retval None
  **********************************
*/
unsigned char HMC_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	HMC_SDA_IN();//SDA����Ϊ����
	for (i = 0;i<8;i++)
	{
		HMC_IIC_SCL = 0;
		HMC_IIC_DELAY();
		HMC_IIC_SCL = 1;
		receive <<= 1;
		if (HMC_READ_SDA)receive++;
		HMC_IIC_DELAY();
	}
	if (!ack)
		HMC_IIC_NAck();//����nACK
	else
		HMC_IIC_Ack(); //����ACK
	return receive;
}
