
#include "app.h"

dataPoint_t currentDataPoint;

#define GPIO_KEY_NUM 2                                  ///< ????????
keyTypedef_t singleKey[GPIO_KEY_NUM];                   ///< ????????????
keysTypedef_t keys;                                     ///< ?????????????
extern SysPara 	AS608Para;//指纹模块AS608参数
extern uint16_t 	ValidN;//模块内有效模板个数
//extern dataSynchro synchro_data;



void userInit(void)
{
    uartxInit();
		
    watchdogInit(2);
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
}

/**
* key1??????
* @param none
* @return none
*/
void key1ShortPress(void)
{
    printf("KEY1 PRESS\n");
}

/**
* key1??????
* @param none
* @return none
*/
void key1LongPress(void)
{
    printf("KEY1 PRESS LONG ,Wifi Reset\n");
    gizwitsSetMode(WIFI_RESET_MODE);
		
}

/**
* key2??????
* @param none
* @return none
*/
void key2ShortPress(void)
{
    printf("KEY2 PRESS ,Soft AP mode\n");
    gizwitsSetMode(WIFI_SOFTAP_MODE);
		CLEAR_ALL_LIGHT();
		LED_Open(1,0);
}

/**
* key2??????
* @param none
* @return none
*/
void key2LongPress(void)
{
	//AirLink mode
	printf("KEY2 PRESS LONG ,AirLink mode\n");
	gizwitsSetMode(WIFI_AIRLINK_MODE);
	CLEAR_ALL_LIGHT();
	LED_Open(0,0);
}

void keyInit(void)
{
//    singleKey[0] = keyInitOne(RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_10, key1ShortPress, key1LongPress);
    singleKey[1] = keyInitOne(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_15, key2ShortPress, key2LongPress);
    keys.singleKey = (keyTypedef_t *)&singleKey;
    keyParaInit(&keys); 
}

void userHandle(void)
{
    /*
	
    */
//	currentDataPoint.valuetemp = 26;
//	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}

int main(void)
{
//	char passwd[] = "";
//	char *ch = NULL;
	uint8_t rbuf[] = {0};
	uint8_t tbuf[] = {0xff};
	u16 adc_vaule = 0;
//	synchro_data.temp = 26;
	OS_ERR err;
	//硬件初始化
	SystemInit();
	delay_init();
	LED_Init();
//	Usart3_Init(115200);
	Key_Init();
	Usart_Init(115200);
	Motor_MOS_Init();
	NRF24L01_Init();
	delay_ms(50);
	LED_Open(0,0);
	while(NRF24L01_Check() != 0);
	LED_Open(0,1);
//	printf("Start Work!");
//	NVIC_Configuration();
	uartInit();
	keyInit();
	Key_Init();
	currentDataPoint.valuedump_energy = 100;
////	KEY_Init();
	gizwitsInit();
//	Adc_Init();
	CPU_SR_ALLOC();
	OSInit(&err);
	OS_CRITICAL_ENTER();//进入临界区代码保护
	/*创建任务*/
	OSTaskCreate ((OS_TCB        *)&StartTackTCB,
								(CPU_CHAR      *)"start_task",
								(OS_TASK_PTR    )start_task,
								(void          *)0,
								(OS_PRIO        )START_TASK_PRIO,
								(CPU_STK       *)START_TASK_STK,
								(CPU_STK_SIZE   )START_STK_SIZE/10,
								(CPU_STK_SIZE   )START_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	OS_CRITICAL_EXIT();//退出临界区代码保护
	OSStart(&err);
	OSTaskDel((OS_TCB*)&StartTackTCB, (OS_ERR*)&err);
	
//	printf("Key Start!\r\n");
	while(1);
	{
		NRF24L01_RX_Mode();
		if(NRF24L01_RxPacket(rbuf) == 0)
		{
			if (rbuf[0] == 0xff)
				LED_Open(1,1);
		}
		delay_ms(50);
		LED_Open(1,0);
		LCD_SPI2_P6x8Num(40,2,adc_vaule);
//		adc_vaule = Get_Adc_Average(ADC_Channel_1, 100);
//		printf("ADC = %d\r\n",adc_vaule);
//		NRF24L01_TX_Mode();
//		if(NRF24L01_TxPacket(tbuf) == TX_OK)
//			LED_Open(1,1);
//		delay_ms(50);
//		LED_Open(1,0);
	}
}
