
#ifndef __AS608_H_
#define __AS608_H_

#include <string.h>
#include "delay.h"
#include "sys.h"

#define USART3_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		400					//最大发送缓存字节数
#define USART3_RX_EN 					1						//0,不接收;1,接收.
#define CharBuffer1 					0x01
#define CharBuffer2 					0x02

extern uint32_t AS608Addr;

typedef struct  
{
	u16 pageID;//指纹ID
	u16 mathscore;//匹配得分
}SearchResult;

typedef struct
{
	u16 PS_max;//指纹最大容量
	u8  PS_level;//安全等级
	u32 PS_addr;
	u8  PS_size;//通讯数据包大小
	u8  PS_N;//波特率基数N
}SysPara;

/**@name 用户API函数
* @{
*/
void PS_Init(void);
uint8_t PS_HandShake(uint32_t *PS_Addr);
uint8_t PS_ValidTempleteNum(uint16_t *ValidN);
uint8_t PS_ReadSysPara(SysPara *p);
uint8_t PS_Empty(void);
uint8_t PS_DeletChar(uint16_t PageID,uint16_t N);
uint8_t PS_GetImage(void);
uint8_t PS_GenChar(uint8_t BufferID);
uint8_t PS_Match(void);
uint8_t PS_RegModel(void);
uint8_t PS_StoreChar(uint8_t BufferID,uint16_t PageID);
uint8_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p);
uint8_t Add_FR(uint16_t id);
void Press_FR(void);
/**@}
*/

#endif
