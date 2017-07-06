
#ifndef __STM32FLASH_H_
#define __STM32FLASH_H_

#include "sys.h"

/*程序占用内存大小,在map文件*/
#define RO_SIZE		26236

/*内存相关宏定义*/
#define STM32_FLASH_SIZE 128 	 				//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1        		//使能FLASH写入(0，不是能;1，使能)
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

/*内存相关条件编译*/
#if STM32_FLASH_SIZE < 256
	#define STM_SECTOR_SIZE 1024 				//字节
#else
	#define STM_SECTOR_SIZE	2048
#endif /*end STM32_FLASH_SIZE*/

/*页对齐*/
#if ((RO_SIZE+1)/STM_SECTOR_SIZE!=0)
	#define FLASH_SAVE_ADDR (((RO_SIZE+1)/STM_SECTOR_SIZE+1)*STM_SECTOR_SIZE+STM32_FLASH_BASE)
#else
	#define FLASH_SAVE_ADDR (((RO_SIZE+1)/STM_SECTOR_SIZE)*STM_SECTOR_SIZE+STM32_FLASH_BASE)
#endif /*end (RO_SIZE+1)/STM_SECTOR_SIZE!=0*/

/*提示是否超出容量*/
#if FLASH_SAVE_ADDR > STM32_FLASH_BASE+STM32_FLASH_SIZE*STM_SECTOR_SIZE
	#error "FLASH SIZE Out of memory"
#endif /*#error*/

/**@name 用户API函数
*@{
*/
u16 STMFLASH_ReadHalfWord(u32 faddr);		  													//读出半字
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);											//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);			//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   			//从指定地址开始读出指定长度的数据
/**@}
*/

#endif
