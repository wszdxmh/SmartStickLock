
#include "as608.h"
#include "led.h"

uint32_t AS608Addr = 0XFFFFFFFF;
//���ڽ��ջ����� 	
uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
uint8_t USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			  //���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
vu16 		USART3_RX_STA = 0;
SysPara 	AS608Para;//ָ��ģ��AS608����
uint16_t 	ValidN;//ģ������Чģ�����

/**
* @brief TIM4��ʼ��
* @param[in] arr:����ֵ psc:Ԥ��Ƶ��
* @return NULL
* @ref as608.h
*/
void TIM4_Int_Init(uint16_t arr,uint16_t psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM4ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�
	
	TIM_Cmd(TIM4,ENABLE);//������ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

/**
* @brief USART3��ʼ��
* @param[in] boutd:������
* @return NULL
* @ref as608.h
*/
void USART3_Init(uint32_t bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����3ʱ��ʹ��

 	USART_DeInit(USART3);  //��λ����3
	//USART2_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA2
   
  //USART2_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PA3
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART3, &USART_InitStructure); //��ʼ������2

	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
	//ʹ�ܽ����ж�
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	TIM4_Int_Init(99,7199);		//10ms�ж�
	USART3_RX_STA=0;		//����
	TIM_Cmd(TIM4,DISABLE);			//�رն�ʱ��4
}

/**
* @brief UASRT3�жϷ�����
* @return NULL
* @ref as608.h
*/
void USART3_IRQHandler(void)
{
	uint8_t res;	      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res = USART_ReceiveData(USART3);		 
		if((USART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(USART3_RX_STA < USART3_MAX_RECV_LEN)	//�����Խ�������
			{
				TIM_SetCounter(TIM4,0);//���������          				//���������
				if(USART3_RX_STA == 0) 				//ʹ�ܶ�ʱ��4���ж� 			
					TIM_Cmd(TIM4, ENABLE);//ʹ�ܶ�ʱ��4
				USART3_RX_BUF[USART3_RX_STA++] = res;	//��¼���յ���ֵ
			}
			else
				USART3_RX_STA |= 1<<15;				//ǿ�Ʊ�ǽ������
		}
	}
}

/**
* @brief TIM4�жϷ�����
* @return NULL
* @ref as608.h
*/
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{
		USART3_RX_STA |= 1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIM4�����жϱ�־    
		TIM_Cmd(TIM4, DISABLE);  //�ر�TIM4 
	}
}

/**
* @brief ��ʼ��PB9��������
* @return NULL
* @ref as608.h
*/
void PS_StaGPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
  //��ʼ����״̬����GPIOB
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
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
* @brief ָ��ģ���ʼ��
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
* @brief ���ڷ���һ���ֽ�
* @param[in] data: ��������
* @return NULL
* @ref as608.h
*/
static void MYUSART_SendData(uint8_t data)
{
	while((USART3->SR&0X40)==0);
	USART3->DR = data;
}

/**
* @brief ���Ͱ�ͷ
* @return NULL
* @ref as608.h
*/
static void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}

/**
* @brief ���͵�ַ
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
* @brief ���Ͱ���ʶ
* @param[in] flag ��ʶλ
* @return NULL
* @ref as608.h
*/
static void SendFlag(uint8_t flag)
{
	MYUSART_SendData(flag);
}

/**
* @brief ���Ͱ�����
* @param[in] length:����
* @return NULL
* @ref as608.h
*/
static void SendLength(int length)
{
	MYUSART_SendData(length>>8);
	MYUSART_SendData(length);
}

/**
* @brief ����ָ����
* @param[in] cmd:������
* @return NULL
* @ref as608.h
*/
static void Sendcmd(uint8_t cmd)
{
	MYUSART_SendData(cmd);
}

/**
* @brief ����У���
* @param[in] check:У���
* @return NULL
* @ref as608.h
*/
static void SendCheck(uint16_t check)
{
	MYUSART_SendData(check>>8);
	MYUSART_SendData(check);
}

/**
* @brief �ж��жϽ��յ�������û��Ӧ���
* @param[in] waittime:�ȴ��жϽ������ݵ�ʱ��(��λ1ms)
* @return *data:���ݰ��׵�ַ
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
		if(USART3_RX_STA&0X8000)//���յ�һ������
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
* @brief ¼��ͼ��
* ̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer
* @param[in] None
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_GetImage(void)
{
  uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief ��������
* ��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CharBuffer1��CharBuffer2
* @param[in] BufferID --> charBuffer1:0x01	charBuffer1:0x02
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_GenChar(uint8_t BufferID)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief ��ȷ�ȶ���öָ������
* ��ȷ�ȶ�CharBuffer1 ��CharBuffer2 �е������ļ�
* @param[in] None
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_Match(void)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief ����ָ��
* ��CharBuffer1��CharBuffer2�е������ļ����������򲿷�ָ�ƿ�.��������,�򷵻�ҳ��
* @param[in] BufferID -> CharBuffer1	CharBuffer2
* @return ģ�鷵��ȷ����,ҳ��(��ƥ��ָ��ģ��)
* @ref as608.h
*/
uint8_t PS_Search(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief �ϲ�����(����ģ��)
* ��CharBuffer1��CharBuffer2�е������ļ��ϲ�����ģ��,�������CharBuffer1��CharBuffer2
* @param[in] None
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_RegModel(void)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief ����ģ��
* ��CharBuffer1��CharBuffer2�е�ģ���ļ��浽PageID��flash���ݿ�λ��
* @param[in] BufferID:charBuffer1 or charBuffer2
* @param[in] PageID:ָ�ƿ�λ�ú�
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_StoreChar(uint8_t BufferID,uint16_t PageID)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief ɾ��ģ��
* ɾ��flash���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
* @param[in] PageID:ָ�ƿ�λ�ú�
* @param[in] N:ɾ����ģ�����
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_DeletChar(uint16_t PageID,uint16_t N)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief ���ָ�ƿ�
* ɾ��flash���ݿ�������ָ��ģ��
* @param[in] None
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_Empty(void)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief дϵͳ�Ĵ���
* @param[in] RegNum:4\5\6
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_WriteReg(uint8_t RegNum,uint8_t DATA)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
//		printf("\r\n���ò����ɹ���");
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

/**
* @brief ��ϵͳ��������
* @param[in] None
* @return ģ�鷵��ȷ����+��������(16 Byte)
* @ref as608.h
*/
uint8_t PS_ReadSysPara(SysPara *p)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
//		printf("\r\nģ�����ָ������=%d",p->PS_max);
//		printf("\r\n�Աȵȼ�=%d",p->PS_level);
//		printf("\r\n��ַ=%x",p->PS_addr);
//		printf("\r\n������=%d",p->PS_N*9600);
//	}
//	else 
//			printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

/**
* @brief ����ģ���ַ
* @param[in] PS_addr:��ַ
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_SetAddr(uint32_t PS_addr)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
	AS608Addr=PS_addr;//������ָ�������ַ
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
		AS608Addr = PS_addr;
//	if(ensure==0x00)
//		printf("\r\n���õ�ַ�ɹ���");
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

/**
* @brief ģ���ڲ�Ϊ�û�������256bytes��FLASH�ռ����ڴ��û����±�
* @param[in] NotePageNum:0~15
* @param[in] Byte32:
* @return ģ�鷵��ȷ����
* @ref as608.h
*/
uint8_t PS_WriteNotepad(uint8_t NotePageNum,uint8_t *Byte32)
{
	uint16_t temp;
  uint8_t  ensure,i;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief ��ȡFLASH�û�����128bytes����
* @param[in] NotePageNum:0~15
* @param[in] Byte32:
* @return ģ�鷵��ȷ����+�û���Ϣ
* @ref as608.h
*/
uint8_t PS_ReadNotepad(uint8_t NotePageNum,uint8_t *Byte32)
{
	uint16_t temp;
  uint8_t  ensure,i;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief ��������
* �� CharBuffer1��CharBuffer2�е������ļ��������������򲿷�ָ�ƿ⡣
* �����������򷵻�ҳ��,��ָ����ڵ�ȷ������ָ�ƿ��� ���ҵ�¼ʱ�����ܺõ�ָ�ƣ���ܿ�������������
* @param[in] BufferID:
* @param[in] StartPage:��ʼҳ
* @param[in] PageNum:ҳ��
* @return ģ�鷵��ȷ����+ҳ��
* @ref as608.h
*/
uint8_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
* @brief ����Чģ�����
* @param[in] *ValidN
* @return ģ�鷵��ȷ����+��Чģ�����ValidN
* @ref as608.h
*/
uint8_t PS_ValidTempleteNum(uint16_t *ValidN)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
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
//		printf("\r\n��Чָ�Ƹ���=%d",(data[10]<<8)+data[11]);
//	}
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

/**
* @brief ��AS608����
* @param[in] *PS_Addr:��ַָ��
* @return ģ���µ�ַ
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
	if(USART3_RX_STA&0X8000)//���յ�����
	{
		if(//�ж��ǲ���ģ�鷵�ص�Ӧ���
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
* @brief ¼ָ��
* @param[in] None
* @return ģ����
* @ref as608.h
*/
uint8_t Add_FR(uint16_t id)
{
	static uint8_t ensure = 0, flag = 0, number = 0;

		switch(flag)
		{
			case 0:
				number++;
				//��ʾ�밴��ָ
				if((ensure=PS_GetImage()) == 0x00)			
					if((ensure=PS_GenChar(CharBuffer1)) == 0x00)
					{
						//��ʾ¼��ɹ�
						number = 0;
						flag = 1;
					}
					else
					{
						//����
						return 1;
					}
				else
				{
					//��ʾ����
					return 2;
				}
				break;
			case 1:
				number++;
				//��ʾ�ٰ�һ����ָ
				if((ensure=PS_GetImage()) == 0x00)
					if((ensure=PS_GenChar(CharBuffer2)) == 0x00)
					{
						//��ʾָ����ȷ
						number = 0;
						flag = 2;
					}
					else
					{return 3;}
				else
				{return 4;}
				break;
			case 2:
				//�Ա��ٴ�ָ��
				if((ensure=PS_Match()) == 0x00)
				{
					//ָ��һ�£��Աȳɹ�
					flag = 3;
				}
				else
				{
					//�Ա�ʧ��,�����°�ָ��
					number = 0;
					flag = 0;
				}
				break;
			case 3:
				//����ָ��ģ��
				if((ensure=PS_RegModel()) == 0x00)
				{
					//����ģ��ɹ�
					flag = 4;
				}
				else
				{
					
				}
				break;
			case 4:
				//����ID
				if((ensure=PS_StoreChar(CharBuffer2,id)) == 0x00)
				{
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					//��ʾָ�Ƹ���
					LED_Open(1,1);
					return 0;
				}
				else
				{
					flag = 0;
					//��ʾ������Ϣ
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
* @brief ˢָ��
* @param[in] None
* @return ģ����
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
				//�����ɹ�
			}
			else
			{}
		else
		{}
	else
	{}
}
