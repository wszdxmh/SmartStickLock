
#include "nrf24l01.h"

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x45,0x45,0x10,0x10,0x01}; //发送地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x45,0x45,0x10,0x10,0x01}; //发送地址

/**
* @brief SPI1 初始化
* @return NULL
* @ref nrf24l01.h
*/
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	SPI1_ReadWriteByte(0xff);//启动传输		 
}

/**
* @brief NRF24L01 初始化IO口
* @return NULL
* @ref nrf24l01.h
*/
void NRF24L01_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
 	
	GPIO_InitStructure.GPIO_Pin = PIN_NRF24L01_CE;	//推挽 	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化指定IO
	
	GPIO_InitStructure.GPIO_Pin = PIN_NRF24L01_CSN;	//推挽 	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化指定IO
	
	GPIO_SetBits(GPIOB, PIN_NRF24L01_CE);//上拉	
	GPIO_SetBits(GPIOA, PIN_NRF24L01_CSN);
  
	GPIO_InitStructure.GPIO_Pin  = PIN_NRF24L01_IRQ;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA12 输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI1_Init();    		//初始化SPI
		
	SPI_Cmd(SPI1, DISABLE); // 
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//选择了串行时钟的稳态:时钟悬空低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//数据捕获于第一个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	NRF24L01_CE = 0; 	//使能24L01
	NRF24L01_CSN = 1;	//SPI片选取消	 	
}

/**
* @brief SPI 速度设置函数
* 说明:
* SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
* SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
* SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
* SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
* @param[in] SpeedSet 分频数
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
* @brief SPIx 读写一个字节
* @param[in] TxData:要写入的字节
* @return 读取到的字节
* @ref nrf24l01.h
*/
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}

/**
* @brief SPI写寄存器
* @param[in] reg:指定寄存器地址
* @param[in] value:写入的值
* @return 返回状态值
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;	
	NRF24L01_CSN=0;                 //使能SPI传输
	status =SPI1_ReadWriteByte(reg);//发送寄存器号 
	SPI1_ReadWriteByte(value);      //写入寄存器的值
	NRF24L01_CSN = 1;                 //禁止SPI传输	   
	return(status);       			//返回状态值
}

/**
* @brief SPI读寄存器
* @param[in] reg:指定寄存器地址
* @return 返回状态值
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	    
	NRF24L01_CSN = 0;          //使能SPI传输		
	SPI1_ReadWriteByte(reg);   //发送寄存器号
	reg_val=SPI1_ReadWriteByte(0XFF);//读取寄存器内容
	NRF24L01_CSN = 1;          //禁止SPI传输		    
	return(reg_val);           //返回状态值
}

/**
* @brief 在指定位置读出指定长度的数据
* @param[in] reg:指定寄存器地址
* @param[in] *pBuf:数据指针
* @param[in] len:数据长度
* @return 返回此次读到的状态寄存器值 
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,uint8_t_ctr;	       
	NRF24L01_CSN = 0;           //使能SPI传输
	status = SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
	for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
		pBuf[uint8_t_ctr] = SPI1_ReadWriteByte(0XFF);//读出数据
	NRF24L01_CSN = 1;       //关闭SPI传输
	return status;        //返回读到的状态值
}

/**
* @brief 在指定位置写指定长度的数据
* @param[in] reg:指定寄存器地址
* @param[in] *pBuf:数据指针
* @param[in] len:数据长度
* @return 返回此次读到的状态寄存器值 
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,uint8_t_ctr;	    
	NRF24L01_CSN = 0;          //使能SPI传输
	status = SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
	for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
		SPI1_ReadWriteByte(*pBuf++); //写入数据	 
	NRF24L01_CSN = 1;       //关闭SPI传输
	return status;          //返回读到的状态值
}

/**
* @brief 检测24L01是否存在
* @return 0：成功;1:失败
* @ref nrf24l01.h
*/
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5] = {0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0; i<5; i++)
		if(buf[i] != 0XA5)
			break;	 							   
	if(i!=5)
		return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}

/**
* @brief 启动NRF24L01发送一次数据
* @param[in] txbuf:待发送数据首地址
* @return 发送完成状况
* @ref nrf24l01.h
*/
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);//spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   
	NRF24L01_CE = 0;
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF24L01_CE = 1;//启动发送	   
	while(NRF24L01_IRQ != 0);//等待发送完成
	sta = NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta & MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta & TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}

/**
* @brief 启动NRF24L01接收一次数据
* @param[in] rxbuf 接收数组的首地址
* @return 接收完成状况
* @ref nrf24l01.h
*/
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}
	return 1;//没收到任何数据
}

/**
* @brief 该函数初始化NRF24L01到RX模式
* 说明:
* 设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
* 当CE变高后,即进入RX模式,并可以接收数据了
* @return NULL
* @ref nrf24l01.h
*/
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE = 0;	  
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    	//使能通道0的自动应答    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);	//使能通道0的接收地址  	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	    	//设置RF通信频率		  
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);	//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);		//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
	NRF24L01_CE = 1; //CE为高,进入接收模式 
}

/**
* @brief 该函数初始化NRF24L01到TX模式
* 说明:
* 设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
* PWR_UP,CRC使能，CE为高大于10us,则启动发送.
* @return NULL
* @ref nrf24l01.h
*/
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE = 0;	    
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF24L01_CE = 1;//CE为高,10us后启动发送
}
