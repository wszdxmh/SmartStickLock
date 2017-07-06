
#include "nrf24l01.h"

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x45,0x45,0x10,0x10,0x01}; //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x45,0x45,0x10,0x10,0x01}; //���͵�ַ

/**
* @brief SPI1 ��ʼ��
* @return NULL
* @ref nrf24l01.h
*/
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
	SPI1_ReadWriteByte(0xff);//��������		 
}

/**
* @brief NRF24L01 ��ʼ��IO��
* @return NULL
* @ref nrf24l01.h
*/
void NRF24L01_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
 	
	GPIO_InitStructure.GPIO_Pin = PIN_NRF24L01_CE;	//���� 	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��ָ��IO
	
	GPIO_InitStructure.GPIO_Pin = PIN_NRF24L01_CSN;	//���� 	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��ָ��IO
	
	GPIO_SetBits(GPIOB, PIN_NRF24L01_CE);//����	
	GPIO_SetBits(GPIOA, PIN_NRF24L01_CSN);
  
	GPIO_InitStructure.GPIO_Pin  = PIN_NRF24L01_IRQ;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA12 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI1_Init();    		//��ʼ��SPI
		
	SPI_Cmd(SPI1, DISABLE); // 
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����յ͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//���ݲ����ڵ�һ��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	NRF24L01_CE = 0; 	//ʹ��24L01
	NRF24L01_CSN = 1;	//SPIƬѡȡ��	 	
}

/**
* @brief SPI �ٶ����ú���
* ˵��:
* SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
* SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
* SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
* SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
* @param[in] SpeedSet ��Ƶ��
* @return NULL
* @ref nrf24l01.h
*/
void SPI1_SetSpeed(uint8_t SpeedSet)
{
	SPI1->CR1&=0XFFC7; 
	SPI1->CR1|=SpeedSet;
	SPI_Cmd(SPI1,ENABLE); 
}

/**
* @brief SPIx ��дһ���ֽ�
* @param[in] TxData:Ҫд����ֽ�
* @return ��ȡ�����ֽ�
* @ref nrf24l01.h
*/
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)
			return 0;
	}
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}

/**
* @brief SPIд�Ĵ���
* @param[in] reg:ָ���Ĵ�����ַ
* @param[in] value:д���ֵ
* @return ����״ֵ̬
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;	
	NRF24L01_CSN=0;                 //ʹ��SPI����
	status =SPI1_ReadWriteByte(reg);//���ͼĴ����� 
	SPI1_ReadWriteByte(value);      //д��Ĵ�����ֵ
	NRF24L01_CSN = 1;                 //��ֹSPI����	   
	return(status);       			//����״ֵ̬
}

/**
* @brief SPI���Ĵ���
* @param[in] reg:ָ���Ĵ�����ַ
* @return ����״ֵ̬
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	    
	NRF24L01_CSN = 0;          //ʹ��SPI����		
	SPI1_ReadWriteByte(reg);   //���ͼĴ�����
	reg_val=SPI1_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
	NRF24L01_CSN = 1;          //��ֹSPI����		    
	return(reg_val);           //����״ֵ̬
}

/**
* @brief ��ָ��λ�ö���ָ�����ȵ�����
* @param[in] reg:ָ���Ĵ�����ַ
* @param[in] *pBuf:����ָ��
* @param[in] len:���ݳ���
* @return ���ش˴ζ�����״̬�Ĵ���ֵ 
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,uint8_t_ctr;	       
	NRF24L01_CSN = 0;           //ʹ��SPI����
	status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
	for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
		pBuf[uint8_t_ctr] = SPI1_ReadWriteByte(0XFF);//��������
	NRF24L01_CSN = 1;       //�ر�SPI����
	return status;        //���ض�����״ֵ̬
}

/**
* @brief ��ָ��λ��дָ�����ȵ�����
* @param[in] reg:ָ���Ĵ�����ַ
* @param[in] *pBuf:����ָ��
* @param[in] len:���ݳ���
* @return ���ش˴ζ�����״̬�Ĵ���ֵ 
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,uint8_t_ctr;	    
	NRF24L01_CSN = 0;          //ʹ��SPI����
	status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
		SPI1_ReadWriteByte(*pBuf++); //д������	 
	NRF24L01_CSN = 1;       //�ر�SPI����
	return status;          //���ض�����״ֵ̬
}

/**
* @brief ���24L01�Ƿ����
* @return 0���ɹ�;1:ʧ��
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5] = {0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0; i<5; i++)
		if(buf[i] != 0XA5)
			break;	 							   
	if(i!=5)
		return 1;//���24L01����	
	return 0;		 //��⵽24L01
}

/**
* @brief ����NRF24L01����һ������
* @param[in] txbuf:�����������׵�ַ
* @return �������״��
* @ref nrf24l01.h
*/
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);//spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	NRF24L01_CE = 0;
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE = 1;//��������	   
	while(NRF24L01_IRQ != 0);//�ȴ��������
	sta = NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta & MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta & TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}

/**
* @brief ����NRF24L01����һ������
* @param[in] rxbuf ����������׵�ַ
* @return �������״��
* @ref nrf24l01.h
*/
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}
	return 1;//û�յ��κ�����
}

/**
* @brief �ú�����ʼ��NRF24L01��RXģʽ
* ˵��:
* ����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
* ��CE��ߺ�,������RXģʽ,�����Խ���������
* @return NULL
* @ref nrf24l01.h
*/
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE = 0;	  
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    	//ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);	//ʹ��ͨ��0�Ľ��յ�ַ  	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	    	//����RFͨ��Ƶ��		  
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);	//����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);		//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
}

/**
* @brief �ú�����ʼ��NRF24L01��TXģʽ
* ˵��:
* ����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
* PWR_UP,CRCʹ�ܣ�CEΪ�ߴ���10us,����������.
* @return NULL
* @ref nrf24l01.h
*/
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE = 0;	    
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE = 1;//CEΪ��,10us����������
}
