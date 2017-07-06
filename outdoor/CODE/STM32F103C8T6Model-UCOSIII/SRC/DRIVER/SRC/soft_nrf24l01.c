
#include "soft_nrf24l01.h"

uint8_t const TX_ADDRESS1[TX_ADR_WIDTH]= {0x01,0x13,0x5C,0x0C,0x03};	//本地地址
uint8_t const RX_ADDRESS1[RX_ADR_WIDTH]= {0x01,0x13,0x5C,0x0C,0x03};	//接收地址

/**@name 初始化IO
*
*/
void NRF_SoftSPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_7 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**@name SPI时序
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

/**@name SPI读数据
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

/**@name SPI写寄存器
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
* @brief 在指定位置读指定长度的数据
* @param[in] reg:寄存器地址
* @param[in] pBug:数据指针
* @param[in] len:数据长度
* @return status:此次读到的状态寄存器值
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
* @brief 在指定位置写指定长度的数据
* @param[in] reg:寄存器地址
* @param[in] pBug:数据指针
* @param[in] len:数据长度
* @return status:此次读到的状态寄存器值
* @ref nrf24l01.h
*/
void SPI_Write_Buf(uint8_t cmd_reg, uint8_t *pBuf, uint8_t num)
{
	uint8_t i;	
	CSN = 0;            //SPI使能       
	SPI_RW(cmd_reg);   
	for(i=0; i<num; i++)   SPI_RW(*pBuf++);
	CSN = 1;           //关闭SPI
}

uint8_t nRF24L01_RxPacket(uint8_t* rx_buf)
{
  uint8_t flag,RX_P_NO;
	uint8_t sta;
	CE = 1;			  //很重要！启动接收！
	delay_ms(1);
	sta=SPI_Read(NRF_READ_REG+STATUS);	// 读取状态寄存其来判断数据接收状况	   //寄存器前面要加是读还是写
	RX_P_NO=sta&0x0e;
	if(sta&RX_OK)				// 判断是否接收到数据  如果置1则说明接到数据并且放置在接收缓存器
	{
	  CE = 0; 				
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH); //这本来就是指令，不用加，意思说去缓存器里读
		flag =1;			//读取数据完成标志
	}
	SPI_Write_Reg(NRF_WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	return RX_P_NO;
}

uint8_t Check_ACK(uint8_t clear)
{
	uint8_t sta;
	while(IRQ);
	sta = SPI_RW(NOP);                    // 返回状态寄存器
	if(sta&MAX_TX)
		if(clear)                         // 是否清除TX FIFO，没有清除在复位MAX_RT中断标志后重发
			SPI_RW(FLUSH_TX);   //清空寄存器，很重要！！！
	SPI_Write_Reg(NRF_WRITE_REG + STATUS, sta);  // 清除TX_DS或MAX_RT中断标志
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
	SPI_Write_Buf(NRF_WRITE_REG+TX_ADDR, buf, 5);//写入5个字节的地址.	
	SPI_Read_Buf(TX_ADDR,buf, 5); //读出写入的地址
	for(i=0; i<5; i++)
		if(buf[i] != 0XA5)
			break;
	if(i != 5)
		return 1;//检测24L01错误
	return 0;		 //检测到24L01
}

uint8_t nRF24L01_TxPacket(unsigned char * tx_buf)
{
	uint8_t Return_Flag=0;
	CE=0;			//StandBy I模式	
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据  WR_TX_PLOAD本来就是指令  不用加   将数据放入发送缓存器	
	CE=1;		 //置高CE，激发数据发送
	delay_ms(2);
	Return_Flag = Check_ACK(1);
	return Return_Flag;
}

void NRF24L01_Init_TX(void)
{    
	/*待机模式*/
 	CE=0;
 	CSN=1;    
 	SCK=0;   
	/*装载数据*/
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, (uint8_t*)TX_ADDRESS1, TX_ADR_WIDTH);//写TX节点地址 
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (uint8_t*)RX_ADDRESS1, RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	 
	/*配置寄存器*/
	SPI_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01); //使能通道0的自动应答    
	SPI_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);//使能通道0的接收地址  
	SPI_Write_Reg(NRF_WRITE_REG + RF_CH, 40);//设置RF通道为40   收发必须一致！
	SPI_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //选择通道0的有效数据宽度 	  
	SPI_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启  收发必须一致！  		
	SPI_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0E);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断  			
}

void NRF24L01_Init_RX(void)
{    
	/*待机模式*/
 	CE=0;    
 	CSN=1;    
 	SCK=0;   
	/*装载数据*/
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, (uint8_t*)TX_ADDRESS1, TX_ADR_WIDTH);//写TX节点地址     	
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (uint8_t*)RX_ADDRESS1, RX_ADR_WIDTH);  //设置TX节点地址,主要为了使能ACK	 
	/*配置寄存器*/
	SPI_Write_Reg(NRF_WRITE_REG + EN_AA, 0x001);//使能通道所有的自动应答          
	SPI_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01); //使能通道所有的接收地址   
	SPI_Write_Reg(NRF_WRITE_REG + RF_CH, 40); //设置RF通道为40   收发必须一致！        
	SPI_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //选择通道0的有效数据宽度 	 
	SPI_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启  收发必须一致！  		  		
	SPI_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断  			  			
}
