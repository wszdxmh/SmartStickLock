
#ifndef __MPU6050_H_
#define __MPU6050_H_

#include "stdint.h"

//����MPU6050Ӳ����ַ
#define MPU_ADDR	0X68//�ӵ�Ϊ0X68 �Ӹߵ�ƽΪ0X69

unsigned char MPU6050_Init(void);
unsigned char MPU_Set_Gyro_Fsr(unsigned char fsr);
unsigned char MPU_Set_Accel_Fsr(unsigned char fsr);
unsigned char MPU_Set_LPF(uint16_t lpf);
unsigned char MPU_Set_Rate(uint16_t rate);
short MPU_Get_Temperature(void);
unsigned char MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
unsigned char MPU_Get_Accelerometer(short *ax,short *ay,short *az);
unsigned char MPU_Write_Len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf);
unsigned char MPU_Read_Len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf);
unsigned char MPU_Write_Byte(unsigned char reg, unsigned char data);
unsigned char MPU_Read_Byte(unsigned char reg);

#endif
