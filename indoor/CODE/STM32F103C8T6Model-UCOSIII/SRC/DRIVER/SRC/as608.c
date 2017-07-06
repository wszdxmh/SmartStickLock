
#include "as608.h"
#include "led.h"

uint32_t AS608Addr = 0XFFFFFFFF;
//串口接收缓存区 	
uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
uint8_t USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节
vu16 		USART3_RX_STA = 0;
SysPara 	AS608Para;//指纹模块AS608参数
uint16_t 	ValidN;//模块内有效模板个数

/**
* @brief TIM4初始化
* @param[in] arr:周期值 psc:预分频数
* @return NULL
* @ref as608.h
*/
void TIM4_Int_Init(uint16_t arr,uint16_t psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM4时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断
	
	TIM_Cmd(TIM4,ENABLE);//开启定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

/**
* @brief USART3初始化
* @param[in] boutd:波特率
* @return NULL
* @ref as608.h
*/
void USART3_Init(uint32_t bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口3时钟使能

 	USART_DeInit(USART3);  //复位串口3
	//USART2_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PA2
   
  //USART2_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PA3
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART3, &USART_InitStructure); //初始化串口2

	USART_Cmd(USART3, ENABLE);                    //使能串口 
	
	//使能接收中断
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	TIM4_Int_Init(99,7199);		//10ms中断
	USART3_RX_STA=0;		//清零
	TIM_Cmd(TIM4,DISABLE);			//关闭定时器4
}

/**
* @brief UASRT3中断服务函数
* @return NULL
* @ref as608.h
*/
void USART3_IRQHandler(void)
{
	uint8_t res;	      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res = USART_ReceiveData(USART3);		 
		if((USART3_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 
			if(USART3_RX_STA < USART3_MAX_RECV_LEN)	//还可以接收数据
			{
				TIM_SetCounter(TIM4,0);//计数器清空          				//计数器清空
				if(USART3_RX_STA == 0) 				//使能定时器4的中断 			
					TIM_Cmd(TIM4, ENABLE);//使能定时器4
				USART3_RX_BUF[USART3_RX_STA++] = res;	//记录接收到的值
			}
			else
				USART3_RX_STA |= 1<<15;				//强制标记接收完成
		}
	}
}

/**
* @brief TIM4中断服务函数
* @return NULL
* @ref as608.h
*/
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
	{
		USART3_RX_STA |= 1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIM4更新中断标志    
		TIM_Cmd(TIM4, DISABLE);  //关闭TIM4 
	}
}

/**
* @brief 初始化PB9下拉输入
* @return NULL
* @ref as608.h
*/
void PS_StaGPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
  //初始化读状态引脚GPIOB
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//输入下拉模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
}

void PS_GPIO(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/**
* @brief 指纹模块初始化
* @return NULL
* @ref as608.h
*/
void PS_Init(void)
{
//	PS_StaGPIO_Init();
	PS_GPIO(RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_9);
	USART3_Init(57600);
}

/**
* @brief 串口发送一个字节
* @param[in] data: 发送数据
* @return NULL
* @ref as608.h
*/
static void MYUSART_SendData(uint8_t data)
{
	while((USART3->SR&0X40)==0);
	USART3->DR = data;
}

/**
* @brief 发送包头
* @return NULL
* @ref as608.h
*/
static void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}

/**
* @brief 发送地址
* @return NULL
* @ref as608.h
*/
static void SendAddr(void)
{
	MYUSART_SendData(AS608Addr>>24);
	MYUSART_SendData(AS608Addr>>16);
	MYUSART_SendData(AS608Addr>>8);
	MYUSART_SendData(AS608Addr);
}

/**
* @brief 发送包标识
* @param[in] flag 标识位
* @return NULL
* @ref as608.h
*/
static void SendFlag(uint8_t flag)
{
	MYUSART_SendData(flag);
}

/**
* @brief 发送包长度
* @param[in] length:长度
* @return NULL
* @ref as608.h
*/
static void SendLength(int length)
{
	MYUSART_SendData(length>>8);
	MYUSART_SendData(length);
}

/**
* @brief 发送指令码
* @param[in] cmd:命令码
* @return NULL
* @ref as608.h
*/
static void Sendcmd(uint8_t cmd)
{
	MYUSART_SendData(cmd);
}

/**
* @brief 发送校验和
* @param[in] check:校检和
* @return NULL
* @ref as608.h
*/
static void SendCheck(uint16_t check)
{
	MYUSART_SendData(check>>8);
	MYUSART_SendData(check);
}

/**
* @brief 判断中断接收的数组有没有应答包
* @param[in] waittime:等待中断接收数据的时间(单位1ms)
* @return *data:数据包首地址
* @ref as608.h
*/
static uint8_t *JudgeStr(uint16_t waittime)
{
	char *data;
	uint8_t str[8];
	str[0]=0xef;str[1]=0x01;str[2]=AS608Addr>>24;
	str[3]=AS608Addr>>16;str[4]=AS608Addr>>8;
	str[5]=AS608Addr;str[6]=0x07;str[7]='\0';
	USART3_RX_STA=0;
	while(--waittime)
	{
		delay_ms(1);
		if(USART3_RX_STA&0X8000)//接收到一次数据
		{
			USART3_RX_STA=0;
			data=strstr((const char*)USART3_RX_BUF,(const char*)str);
			if(data)
				return (uint8_t*)data;
		}
	}
	return 0;
}

/**
* @brief 录入图像
* 探测手指，探测到后录入指纹图像存于ImageBuffer
* @param[in] None
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_GetImage(void)
{
  uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x01);
  temp =  0x01+0x03+0x01;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

/**
* @brief 生成特征
* 将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2
* @param[in] BufferID --> charBuffer1:0x01	charBuffer1:0x02
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_GenChar(uint8_t BufferID)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x04);
	Sendcmd(0x02);
	MYUSART_SendData(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

/**
* @brief 精确比对两枚指纹特征
* 精确比对CharBuffer1 与CharBuffer2 中的特征文件
* @param[in] None
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_Match(void)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01+0x03+0x03;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

/**
* @brief 搜索指纹
* 以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到,则返回页码
* @param[in] BufferID -> CharBuffer1	CharBuffer2
* @return 模块返回确认字,页码(相匹配指纹模板)
* @ref as608.h
*/
uint8_t PS_Search(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x04);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage>>8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum>>8);
	MYUSART_SendData(PageNum);
	temp = 0x01+0x08+0x04+BufferID
	+(StartPage>>8)+(uint8_t)StartPage
	+(PageNum>>8)+(uint8_t)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure = data[9];
		p->pageID   =(data[10]<<8)+data[11];
		p->mathscore=(data[12]<<8)+data[13];
	}
	else
		ensure = 0xff;
	return ensure;
}

/**
* @brief 合并特征(生成模板)
* 将CharBuffer1与CharBuffer2中的特征文件合并生成模板,结果存于CharBuffer1与CharBuffer2
* @param[in] None
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_RegModel(void)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;		
}

/**
* @brief 储存模板
* 将CharBuffer1或CharBuffer2中的模板文件存到PageID号flash数据库位置
* @param[in] BufferID:charBuffer1 or charBuffer2
* @param[in] PageID:指纹库位置号
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_StoreChar(uint8_t BufferID,uint16_t PageID)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x06);
	Sendcmd(0x06);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	temp = 0x01+0x06+0x06+BufferID
	+(PageID>>8)+(uint8_t)PageID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}

/**
* @brief 删除模板
* 删除flash数据库中指定ID号开始的N个指纹模板
* @param[in] PageID:指纹库位置号
* @param[in] N:删除的模板个数
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_DeletChar(uint16_t PageID,uint16_t N)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x07);
	Sendcmd(0x0C);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	MYUSART_SendData(N>>8);
	MYUSART_SendData(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(uint8_t)PageID
	+(N>>8)+(uint8_t)N;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

/**
* @brief 清空指纹库
* 删除flash数据库中所有指纹模板
* @param[in] None
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_Empty(void)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

/**
* @brief 写系统寄存器
* @param[in] RegNum:4\5\6
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_WriteReg(uint8_t RegNum,uint8_t DATA)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x05);
	Sendcmd(0x0E);
	MYUSART_SendData(RegNum);
	MYUSART_SendData(DATA);
	temp = RegNum+DATA+0x01+0x05+0x0E;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
//	if(ensure==0)
//		printf("\r\n设置参数成功！");
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

/**
* @brief 读系统基本参数
* @param[in] None
* @return 模块返回确认字+基本参数(16 Byte)
* @ref as608.h
*/
uint8_t PS_ReadSysPara(SysPara *p)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x0F);
	temp = 0x01+0x03+0x0F;
	SendCheck(temp);
	data=JudgeStr(1000);
	if(data)
	{
		ensure = data[9];
		p->PS_max = (data[14]<<8)+data[15];
		p->PS_level = data[17];
		p->PS_addr=(data[18]<<24)+(data[19]<<16)+(data[20]<<8)+data[21];
		p->PS_size = data[23];
		p->PS_N = data[25];
	}		
	else
		ensure=0xff;
//	if(ensure==0x00)
//	{
//		printf("\r\n模块最大指纹容量=%d",p->PS_max);
//		printf("\r\n对比等级=%d",p->PS_level);
//		printf("\r\n地址=%x",p->PS_addr);
//		printf("\r\n波特率=%d",p->PS_N*9600);
//	}
//	else 
//			printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

/**
* @brief 设置模块地址
* @param[in] PS_addr:地址
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_SetAddr(uint32_t PS_addr)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x07);
	Sendcmd(0x15);
	MYUSART_SendData(PS_addr>>24);
	MYUSART_SendData(PS_addr>>16);
	MYUSART_SendData(PS_addr>>8);
	MYUSART_SendData(PS_addr);
	temp = 0x01+0x07+0x15
	+(uint8_t)(PS_addr>>24)+(uint8_t)(PS_addr>>16)
	+(uint8_t)(PS_addr>>8) +(uint8_t)PS_addr;				
	SendCheck(temp);
	AS608Addr=PS_addr;//发送完指令，更换地址
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
		AS608Addr = PS_addr;
//	if(ensure==0x00)
//		printf("\r\n设置地址成功！");
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

/**
* @brief 模块内部为用户开辟了256bytes的FLASH空间用于存用户记事本
* @param[in] NotePageNum:0~15
* @param[in] Byte32:
* @return 模块返回确认字
* @ref as608.h
*/
uint8_t PS_WriteNotepad(uint8_t NotePageNum,uint8_t *Byte32)
{
	uint16_t temp;
  uint8_t  ensure,i;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(36);
	Sendcmd(0x18);
	MYUSART_SendData(NotePageNum);
	for(i=0;i<32;i++)
	 {
		 MYUSART_SendData(Byte32[i]);
		 temp += Byte32[i];
	 }
  temp =0x01+36+0x18+NotePageNum+temp;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

/**
* @brief 读取FLASH用户区的128bytes数据
* @param[in] NotePageNum:0~15
* @param[in] Byte32:
* @return 模块返回确认字+用户信息
* @ref as608.h
*/
uint8_t PS_ReadNotepad(uint8_t NotePageNum,uint8_t *Byte32)
{
	uint16_t temp;
  uint8_t  ensure,i;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x04);
	Sendcmd(0x19);
	MYUSART_SendData(NotePageNum);
	temp = 0x01+0x04+0x19+NotePageNum;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];
		for(i=0;i<32;i++)
		{
			Byte32[i]=data[10+i];
		}
	}
	else
		ensure=0xff;
	return ensure;
}

/**
* @brief 高速搜索
* 以 CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库。
* 若搜索到，则返回页码,该指令对于的确存在于指纹库中 ，且登录时质量很好的指纹，会很快给出搜索结果。
* @param[in] BufferID:
* @param[in] StartPage:起始页
* @param[in] PageNum:页数
* @return 模块返回确认字+页码
* @ref as608.h
*/
uint8_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x1b);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage>>8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum>>8);
	MYUSART_SendData(PageNum);
	temp = 0x01+0x08+0x1b+BufferID
	+(StartPage>>8)+(uint8_t)StartPage
	+(PageNum>>8)+(uint8_t)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
 	if(data)
	{
		ensure=data[9];
		p->pageID 	=(data[10]<<8) +data[11];
		p->mathscore=(data[12]<<8) +data[13];
	}
	else
		ensure=0xff;
	return ensure;
}

/**
* @brief 读有效模板个数
* @param[in] *ValidN
* @return 模块返回确认字+有效模板个数ValidN
* @ref as608.h
*/
uint8_t PS_ValidTempleteNum(uint16_t *ValidN)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x1d);
	temp = 0x01+0x03+0x1d;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];
		*ValidN = (data[10]<<8) +data[11];
	}		
	else
		ensure=0xff;
	
//	if(ensure==0x00)
//	{
//		printf("\r\n有效指纹个数=%d",(data[10]<<8)+data[11]);
//	}
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

/**
* @brief 与AS608握手
* @param[in] *PS_Addr:地址指针
* @return 模块新地址
* @ref as608.h
*/
uint8_t PS_HandShake(uint32_t *PS_Addr)
{
	SendHead();
	SendAddr();
	MYUSART_SendData(0X01);
	MYUSART_SendData(0X00);
	MYUSART_SendData(0X00);
	delay_ms(200);
	if(USART3_RX_STA&0X8000)//接收到数据
	{
		if(//判断是不是模块返回的应答包
					USART3_RX_BUF[0]==0XEF
				&&USART3_RX_BUF[1]==0X01
				&&USART3_RX_BUF[6]==0X07
			)
			{
				*PS_Addr=(USART3_RX_BUF[2]<<24) + (USART3_RX_BUF[3]<<16)
								+(USART3_RX_BUF[4]<<8) + (USART3_RX_BUF[5]);
				USART3_RX_STA=0;
				return 0;
			}
		USART3_RX_STA=0;
	}
	return 1;
}

/**
* @brief 录指纹
* @param[in] None
* @return 模块字
* @ref as608.h
*/
uint8_t Add_FR(uint16_t id)
{
	static uint8_t ensure = 0, flag = 0, number = 0;

		switch(flag)
		{
			case 0:
				number++;
				//显示请按手指
				if((ensure=PS_GetImage()) == 0x00)			
					if((ensure=PS_GenChar(CharBuffer1)) == 0x00)
					{
						//显示录入成功
						number = 0;
						flag = 1;
					}
					else
					{
						//错误
						return 1;
					}
				else
				{
					//显示错误
					return 2;
				}
				break;
			case 1:
				number++;
				//显示再按一次手指
				if((ensure=PS_GetImage()) == 0x00)
					if((ensure=PS_GenChar(CharBuffer2)) == 0x00)
					{
						//显示指纹正确
						number = 0;
						flag = 2;
					}
					else
					{return 3;}
				else
				{return 4;}
				break;
			case 2:
				//对比再次指纹
				if((ensure=PS_Match()) == 0x00)
				{
					//指纹一致，对比成功
					flag = 3;
				}
				else
				{
					//对比失败,请重新按指纹
					number = 0;
					flag = 0;
				}
				break;
			case 3:
				//生成指纹模板
				if((ensure=PS_RegModel()) == 0x00)
				{
					//生成模板成功
					flag = 4;
				}
				else
				{
					
				}
				break;
			case 4:
				//输入ID
				if((ensure=PS_StoreChar(CharBuffer2,id)) == 0x00)
				{
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					//显示指纹个数
					LED_Open(1,1);
					return 0;
				}
				else
				{
					flag = 0;
					//显示错误信息
				}
				break;
		}
		if(number == 5)
		{
			//No find
			return 5;
		}
	
}

/**
* @brief 刷指纹
* @param[in] None
* @return 模块字
* @ref as608.h
*/
void Press_FR(void)
{
	SearchResult seach;
	uint8_t ensure = 0;
	if((ensure=PS_GetImage()) == 0x00)
		if((ensure=PS_GenChar(CharBuffer1)) == 0x00)
			if((ensure=PS_HighSpeedSearch(CharBuffer1, 0, 300, &seach)) == 0x00)
			{
				//搜索成功
			}
			else
			{}
		else
		{}
	else
	{}
}
