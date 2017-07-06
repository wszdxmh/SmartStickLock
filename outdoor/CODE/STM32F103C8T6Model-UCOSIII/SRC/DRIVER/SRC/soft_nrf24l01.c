
#include "soft_nrf24l01.h"

uint8_t const TX_ADDRESS1[TX_ADR_WIDTH]= {0x01,0x13,0x5C,0x0C,0x03};	//���ص�ַ
uint8_t const RX_ADDRESS1[RX_ADR_WIDTH]= {0x01,0x13,0x5C,0x0C,0x03};	//���յ�ַ

/**@name ��ʼ��IO
*
*/
void NRF_SoftSPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_7 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**@name SPIʱ��
* 
*/
uint8_t SPI_RW(uint8_t dat)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		MOSI = (dat & 0x80);
		dat = (dat << 1);
		SCK = 1;
		dat |= MISO;
		SCK = 0;
	}
	return(dat);
}

/**@name SPI������
* 
*/
uint8_t SPI_Read(uint8_t cmd_reg)
{
	uint8_t value;
	
	CSN = 0;               
	SPI_RW(cmd_reg);            
	value = SPI_RW(0);    
	CSN = 1;                
	
	return(value);
}

/**@name SPIд�Ĵ���
* 
*/
void SPI_Write_Reg(uint8_t cmd_reg, uint8_t value)
{	
	CSN = 0;                  
	SPI_RW(cmd_reg);     
	SPI_RW(value);            
	CSN = 1;                   
}

/**
* @brief ��ָ��λ�ö�ָ�����ȵ�����
* @param[in] reg:�Ĵ�����ַ
* @param[in] pBug:����ָ��
* @param[in] len:���ݳ���
* @return status:�˴ζ�����״̬�Ĵ���ֵ
* @ref nrf24l01.h
*/
uint8_t SPI_Read_Buf(uint8_t cmd_reg, uint8_t *pBuf, uint8_t num)
{
	uint8_t status,i;
	
	CSN = 0;                    		
	status = SPI_RW(cmd_reg);       		
	for(i=0;i<num;i++)   
		pBuf[i] = SPI_RW(0);    	
	CSN = 1;                           	
	return(status);
}

/**
* @brief ��ָ��λ��дָ�����ȵ�����
* @param[in] reg:�Ĵ�����ַ
* @param[in] pBug:����ָ��
* @param[in] len:���ݳ���
* @return status:�˴ζ�����״̬�Ĵ���ֵ
* @ref nrf24l01.h
*/
void SPI_Write_Buf(uint8_t cmd_reg, uint8_t *pBuf, uint8_t num)
{
	uint8_t i;	
	CSN = 0;            //SPIʹ��       
	SPI_RW(cmd_reg);   
	for(i=0; i<num; i++)   SPI_RW(*pBuf++);
	CSN = 1;           //�ر�SPI
}

uint8_t nRF24L01_RxPacket(uint8_t* rx_buf)
{
  uint8_t flag,RX_P_NO;
	uint8_t sta;
	CE = 1;			  //����Ҫ���������գ�
	delay_ms(1);
	sta=SPI_Read(NRF_READ_REG+STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��	   //�Ĵ���ǰ��Ҫ���Ƕ�����д
	RX_P_NO=sta&0x0e;
	if(sta&RX_OK)				// �ж��Ƿ���յ�����  �����1��˵���ӵ����ݲ��ҷ����ڽ��ջ�����
	{
	  CE = 0; 				
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH); //�Ȿ������ָ����üӣ���˼˵ȥ���������
		flag =1;			//��ȡ������ɱ�־
	}
	SPI_Write_Reg(NRF_WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	return RX_P_NO;
}

uint8_t Check_ACK(uint8_t clear)
{
	uint8_t sta;
	while(IRQ);
	sta = SPI_RW(NOP);                    // ����״̬�Ĵ���
	if(sta&MAX_TX)
		if(clear)                         // �Ƿ����TX FIFO��û������ڸ�λMAX_RT�жϱ�־���ط�
			SPI_RW(FLUSH_TX);   //��ռĴ���������Ҫ������
	SPI_Write_Reg(NRF_WRITE_REG + STATUS, sta);  // ���TX_DS��MAX_RT�жϱ�־
	IRQ = 1;
	if(sta&TX_OK)
		return(TX_OK);
	else
		return(0xff);
}

uint8_t nRF24L01_Check(void)
{
	uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	uint8_t i;
	SPI_Write_Buf(NRF_WRITE_REG+TX_ADDR, buf, 5);//д��5���ֽڵĵ�ַ.	
	SPI_Read_Buf(TX_ADDR,buf, 5); //����д��ĵ�ַ
	for(i=0; i<5; i++)
		if(buf[i] != 0XA5)
			break;
	if(i != 5)
		return 1;//���24L01����
	return 0;		 //��⵽24L01
}

uint8_t nRF24L01_TxPacket(unsigned char * tx_buf)
{
	uint8_t Return_Flag=0;
	CE=0;			//StandBy Iģʽ	
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // װ������  WR_TX_PLOAD��������ָ��  ���ü�   �����ݷ��뷢�ͻ�����	
	CE=1;		 //�ø�CE���������ݷ���
	delay_ms(2);
	Return_Flag = Check_ACK(1);
	return Return_Flag;
}

void NRF24L01_Init_TX(void)
{    
	/*����ģʽ*/
 	CE=0;
 	CSN=1;    
 	SCK=0;   
	/*װ������*/
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, (uint8_t*)TX_ADDRESS1, TX_ADR_WIDTH);//дTX�ڵ��ַ 
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (uint8_t*)RX_ADDRESS1, RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	 
	/*���üĴ���*/
	SPI_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01); //ʹ��ͨ��0���Զ�Ӧ��    
	SPI_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);//ʹ��ͨ��0�Ľ��յ�ַ  
	SPI_Write_Reg(NRF_WRITE_REG + RF_CH, 40);//����RFͨ��Ϊ40   �շ�����һ�£�
	SPI_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //ѡ��ͨ��0����Ч���ݿ�� 	  
	SPI_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f); //����TX�������,0db����,2Mbps,���������濪��  �շ�����һ�£�  		
	SPI_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0E);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�  			
}

void NRF24L01_Init_RX(void)
{    
	/*����ģʽ*/
 	CE=0;    
 	CSN=1;    
 	SCK=0;   
	/*װ������*/
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, (uint8_t*)TX_ADDRESS1, TX_ADR_WIDTH);//дTX�ڵ��ַ     	
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (uint8_t*)RX_ADDRESS1, RX_ADR_WIDTH);  //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	 
	/*���üĴ���*/
	SPI_Write_Reg(NRF_WRITE_REG + EN_AA, 0x001);//ʹ��ͨ�����е��Զ�Ӧ��          
	SPI_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01); //ʹ��ͨ�����еĽ��յ�ַ   
	SPI_Write_Reg(NRF_WRITE_REG + RF_CH, 40); //����RFͨ��Ϊ40   �շ�����һ�£�        
	SPI_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //ѡ��ͨ��0����Ч���ݿ�� 	 
	SPI_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f); //����TX�������,0db����,2Mbps,���������濪��  �շ�����һ�£�  		  		
	SPI_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�  			  			
}
