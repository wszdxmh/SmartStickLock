
#ifndef __STM32FLASH_H_
#define __STM32FLASH_H_

#include "sys.h"

/*����ռ���ڴ��С,��map�ļ�*/
#define RO_SIZE		26236

/*�ڴ���غ궨��*/
#define STM32_FLASH_SIZE 128 	 				//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1        		//ʹ��FLASHд��(0��������;1��ʹ��)
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

/*�ڴ������������*/
#if STM32_FLASH_SIZE < 256
	#define STM_SECTOR_SIZE 1024 				//�ֽ�
#else
	#define STM_SECTOR_SIZE	2048
#endif /*end STM32_FLASH_SIZE*/

/*ҳ����*/
#if ((RO_SIZE+1)/STM_SECTOR_SIZE!=0)
	#define FLASH_SAVE_ADDR (((RO_SIZE+1)/STM_SECTOR_SIZE+1)*STM_SECTOR_SIZE+STM32_FLASH_BASE)
#else
	#define FLASH_SAVE_ADDR (((RO_SIZE+1)/STM_SECTOR_SIZE)*STM_SECTOR_SIZE+STM32_FLASH_BASE)
#endif /*end (RO_SIZE+1)/STM_SECTOR_SIZE!=0*/

/*��ʾ�Ƿ񳬳�����*/
#if FLASH_SAVE_ADDR > STM32_FLASH_BASE+STM32_FLASH_SIZE*STM_SECTOR_SIZE
	#error "FLASH SIZE Out of memory"
#endif /*#error*/

/**@name �û�API����
*@{
*/
u16 STMFLASH_ReadHalfWord(u32 faddr);		  													//��������
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);											//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);			//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   			//��ָ����ַ��ʼ����ָ�����ȵ�����
/**@}
*/

#endif
