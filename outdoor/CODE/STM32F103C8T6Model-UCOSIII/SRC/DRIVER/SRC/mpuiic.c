
#include "mpuiic.h"

/**
  **********************************
  * @brief  MPU6050��ʼ��
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
}

/**
  **********************************
  * @brief  ����IIC��ʼ�ź�
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();//sda�����
	MPU_IIC_SDA = 1;
	MPU_IIC_SCL = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SDA = 0;//START:when CLK is high,DATA change form high to low
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ��������
}

/**
  **********************************
  * @brief  ����IICֹͣ�ź�
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();//sda�����
	MPU_IIC_SCL = 0;
	MPU_IIC_SDA = 0;//STOP:when CLK is high DATA change form low to high
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 1;
	MPU_IIC_SDA = 1;//����I2C���߽����ź�
	MPU_IIC_DELAY();
}

/**
  **********************************
  * @brief  �ȴ�Ӧ���ź�
  * @param  None
  * @retval 1������Ӧ��ʧ��	0������Ӧ��ɹ�
  **********************************
*/
unsigned char MPU_IIC_Wait_Ack(void)
{
	unsigned char ucErrTime = 0;
	MPU_SDA_IN();      //SDA����Ϊ����  
	MPU_IIC_SDA = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 1;
	MPU_IIC_DELAY();
	while (MPU_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL = 0;//ʱ�����0
	return 0;
} 

/**
  **********************************
  * @brief  ����Ӧ���ź�
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL = 0;
	MPU_SDA_OUT();
	MPU_IIC_SDA = 0;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 0;
}

/**
  **********************************
  * @brief  ������Ӧ���ź�
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL = 0;
	MPU_SDA_OUT();
	MPU_IIC_SDA = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 0;
}

/**
  **********************************
  * @brief  ����һ���ֽ�
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Send_Byte(unsigned char txd)
{                        
	unsigned char t;
	MPU_SDA_OUT();
	MPU_IIC_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
	for (t = 0;t<8;t++)
	{
		MPU_IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		MPU_IIC_SCL = 1;
		MPU_IIC_DELAY();
		MPU_IIC_SCL = 0;
		MPU_IIC_DELAY();
	}
}

/**
  **********************************
  * @brief  ��һ���ֽ�
  * @param  None
  * @retval None
  **********************************
*/
unsigned char MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	MPU_SDA_IN();//SDA����Ϊ����
	for (i = 0;i<8;i++)
	{
		MPU_IIC_SCL = 0;
		MPU_IIC_DELAY();
		MPU_IIC_SCL = 1;
		receive <<= 1;
		if (MPU_READ_SDA)receive++;
		MPU_IIC_DELAY();
	}
	if (!ack)
		MPU_IIC_NAck();//����nACK
	else
		MPU_IIC_Ack(); //����ACK
	return receive;
}
