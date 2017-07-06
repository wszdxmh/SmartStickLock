
#include "app.h"

extern SysPara 	AS608Para;//指纹模块AS608参数
extern uint16_t 	ValidN;//模块内有效模板个数

void watchdogInit(uint8_t timeoutS)//?
{
#ifdef  WATCHDOG
    uint8_t prer = 4;
    uint16_t rlr = timeoutS * 625;
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* ??????IWDG_PR?IWDG_RLR????*/
    IWDG_SetPrescaler(prer);    /*??IWDG????:??IWDG????*/
    IWDG_SetReload(rlr);     /*??IWDG????*/
    IWDG_ReloadCounter();    /*??IWDG???????????IWDG???*/
    IWDG_Enable();        /*??IWDG*/
#endif
}

int main(void)
{
	short tempvaule = 0;
	OS_ERR err;
	//硬件初始化
	SystemInit();
	delay_init();
	Usart3_Init(115200);
	PS_Init();
	while(PS_HandShake(&AS608Addr) == 1);
//		LED_Open(0,1);
	PS_ValidTempleteNum(&ValidN);
	PS_ReadSysPara(&AS608Para);
	NRF24L01_Init();
	delay_ms(50);
	while(NRF24L01_Check() != 0);
	watchdogInit(2);

//	NVIC_Configuration();
//	Adc_Init();
	LCD_SPI2_Init();

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
	
	while(1);
	{
		tempvaule = return_Keyvalue();
		Display_P16x16_Number(10,4,tempvaule/100);
		Display_P16x16_Number(18,4,tempvaule%100/10);
		Display_P16x16_Number(26,4,tempvaule%10);
		LCD_SPI_Display_ALL();
	}
}
