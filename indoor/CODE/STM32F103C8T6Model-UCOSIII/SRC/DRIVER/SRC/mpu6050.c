
#include "mpuiic.h"
#include "mpu6050.h"
#include "mpu6050reg.h"
#include "PC_Link.h"

/**
  **********************************
  * @brief  MPU6050��ʼ��
	* @param  None
  * @retval 0:�ɹ���ʼ��
  **********************************
*/
unsigned char MPU6050_Init(void)
{
	unsigned char res;
	MPU_IIC_Init();
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80);	//��λMPU6050
	delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(50);						//���ò�����50Hz
	MPU_Write_Byte(MPU_INT_EN_REG, 0X00);	//�ر������ж�
	MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00);	//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);	//�ر�FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80);	//INT���ŵ͵�ƽ��Ч
	res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if (res == MPU_ADDR)//����ID��ȷ
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);						//���ò�����Ϊ50Hz
	}
	else
		return 1;
	return 0;
}

/**
  **********************************
  * @brief  ����MPU6050�����������̷�Χ
	* @param  fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
  * @retval 0���ɹ�
  **********************************
*/
unsigned char MPU_Set_Gyro_Fsr(unsigned char fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}

/**
  **********************************
  * @brief  ����MPU6050���ٶȼ������̷�Χ
	* @param  fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
  * @retval 0���ɹ�
  **********************************
*/
unsigned char MPU_Set_Accel_Fsr(unsigned char fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}

/**
  **********************************
  * @brief  ����MPU6050���ֵ�ͨ�˲���
	* @param  lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
  * @retval 0���ɹ�
  **********************************
*/
unsigned char MPU_Set_LPF(uint16_t lpf)
{
	unsigned char data = 0;
	if (lpf >= 188)data = 1;
	else if (lpf >= 98)data = 2;
	else if (lpf >= 42)data = 3;
	else if (lpf >= 20)data = 4;
	else if (lpf >= 10)data = 5;
	else data = 6;
	return MPU_Write_Byte(MPU_CFG_REG, data);//�������ֵ�ͨ�˲���  
}

/**
  **********************************
  * @brief  ����MPU6050����Ƶ��
	* @param  rate:4~1000(Hz)
  * @retval 0���ɹ�
  **********************************
*/
unsigned char MPU_Set_Rate(uint16_t rate)
{
	unsigned char data;
	if (rate>1000)rate = 1000;
	if (rate<4)rate = 4;
	data = 1000 / rate - 1;
	data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data);	//�������ֵ�ͨ�˲���
	return MPU_Set_LPF(rate / 2);	//�Զ�����LPFΪ�����ʵ�һ��
}

/**
  **********************************
  * @brief  MPU6050�ɼ��¶�
	* @param  None
  * @retval ����100��
  **********************************
*/
short MPU_Get_Temperature(void)
{
	unsigned char buf[2];
	short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
	raw = ((u16)buf[0] << 8) | buf[1];
	temp = 36.53 + ((double)raw) / 340;
	return temp * 100;;
}

/**
  **********************************
  * @brief  �ɼ�MPU6050������ԭʼ����
	* @param  gx,gy,gz:������x,y,z���ԭʼ����(������)
  * @retval 0���ɹ�
  **********************************
*/
unsigned char MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
	unsigned char buf[6], res;
	res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*gx = ((u16)buf[0] << 8) | buf[1];
		*gy = ((u16)buf[2] << 8) | buf[3];
		*gz = ((u16)buf[4] << 8) | buf[5];
	}
	return res;
}

/**
  **********************************
  * @brief  �ɼ�MPU6050���ٶȼ�ԭʼ����
	* @param  gx,gy,gz:������x,y,z���ԭʼ����(������)
  * @retval 0���ɹ�
  **********************************
*/
unsigned char MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
	unsigned char buf[6], res;
	res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*ax = ((u16)buf[0] << 8) | buf[1];
		*ay = ((u16)buf[2] << 8) | buf[3];
		*az = ((u16)buf[4] << 8) | buf[5];
	}
	return res;
}

/**
  **********************************
  * @brief  IIC����д����
	* @param  addr:������ַ	reg:�Ĵ�����ַ len:д�볤�� buf:������
  * @retval 0���ɹ�
  **********************************
*/
unsigned char MPU_Write_Len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf)
{
	unsigned char i;
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr << 1) | 0);//����������ַ+д����	
	if (MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
	for (i = 0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	//��������
		if (MPU_IIC_Wait_Ack())		//�ȴ�ACK
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_Stop();
	return 0;
}

/**
  **********************************
  * @brief  IIC����������
	* @param  addr:������ַ	reg:�Ĵ�����ַ len:д�볤�� buf:������
  * @retval 0���ɹ�
  **********************************
*/
unsigned char MPU_Read_Len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr << 1) | 0);//����������ַ+д����	
	if (MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr << 1) | 1);//����������ַ+������	
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	while (len)
	{
		if (len == 1)*buf = MPU_IIC_Read_Byte(0);//������,����nACK 
		else *buf = MPU_IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++;
	}
	MPU_IIC_Stop();	//����һ��ֹͣ���� 
	return 0;
}

/**
  **********************************
  * @brief  MPU6050дһ���ֽ�
	* @param  reg:�Ĵ�����ַ data:����
  * @retval None
  **********************************
*/
unsigned char MPU_Write_Byte(unsigned char reg, unsigned char data)
{ 
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0);//����������ַ+д����	
	if (MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	MPU_IIC_Send_Byte(data);//��������
	if (MPU_IIC_Wait_Ack())	//�ȴ�ACK
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Stop();
	return 0;
}

/**
  **********************************
  * @brief  MPU6050��һ���ֽ�
	* @param  reg:�Ĵ�����ַ
  * @retval None
  **********************************
*/
unsigned char MPU_Read_Byte(unsigned char reg)
{
	unsigned char res;
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0);//����������ַ+д����	
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR << 1) | 1);//����������ַ+������	
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res = MPU_IIC_Read_Byte(0);//��ȡ����,����nACK 
	MPU_IIC_Stop();			//����һ��ֹͣ���� 
	return res;
}
