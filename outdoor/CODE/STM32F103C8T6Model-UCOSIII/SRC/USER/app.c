
#include "app.h"


extern SysPara 	AS608Para;//指纹模块AS608参数
extern uint16_t 	ValidN;//模块内有效模板个数
extern uint8_t OLED_Buffer[8][128];
uint8_t Init_Passwd[6] = {1,2,3,4,5,6};

user_flag User_Flag;
uint8_t Key_Buffer[12];

/*开始任务*/
OS_TCB	StartTackTCB;
CPU_STK	START_TASK_STK[START_STK_SIZE];

/*任务MOTOR*/
OS_TCB MOTORTCB;
CPU_STK	MOTOR_STK[MOTOR_STK_SIZE];

//NRF24L01任务控制块
OS_TCB NRF24L01TCB;
CPU_STK NRF24L01_STK[NRF24L01_STK_SIZE];

//录指纹任务
OS_TCB AS608TCB;
CPU_STK AS608_STK[AS608_STK_SIZE];

/**刷指纹任务**/
OS_TCB Press_FR_TCB;
CPU_STK Press_FR_STK[Press_FR_STK_SIZE];

//DS18B20任务
OS_TCB DS18B20TCB;
CPU_STK DS18B20_STK[DS18B20_STK_SIZE];

/**OLED显示任务**/
OS_TCB OLED_Display_TCB;
CPU_STK OLED_Display_STK[OLED_Display_STK_SIZE];

/**按键扫描**/
OS_TCB Key_Scan_TCB;
CPU_STK Key_Scan_STK[Key_Scan_STK_SIZE];

/*定时器1回调函数*/
OS_TMR tmr1;

///*定时器2*/
//OS_TMR tmr2;

///*定时器3*/
//OS_TMR tmr3;

/**互斥信号量**/
OS_MUTEX FR_MUTEX;
//OS_SEM	SYNC_SEM;

//任务MOTOR控制函数
void Motor_task(volatile void *p_arg)
{
//	uint8_t cnt = 0;
//	uint8_t scanf = 0;
//	uint8_t x = 30;
//	uint8_t num = 0;
//	uint8_t count = 0;
//	uint8_t passwd_flag = 0;
	uint8_t beaty = 21;
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
//		OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, NULL, &err);//等待内嵌信号量
//		OSTimeDlyHMSM(0,0,1,500,OS_OPT_TIME_HMSM_STRICT,&err);
//		OSSchedLock(&err);
//		OSSchedUnlock(&err);
//		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
//		OSTaskDel(&MOTORTCB, &err);
		OSSchedLock(&err);
		for (uint8_t j=0; j<6; j++)
			User_Flag.passwd[j] = j;
		NRF_EN_Display(3,0);
		Display_Fingerprint_Flag(15,0);  
		Battery_Display(100,0,beaty);
		Display_Dividing_Line(1);
//		beaty>21?beaty=0:beaty++;
		OSSchedUnlock(&err);
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}


//录指纹
void Add_FR_task(volatile void *p_arg)
{
	uint8_t ensure = 5;
	uint8_t cnt = 0;
	OS_ERR err;
	p_arg = p_arg;
	uint8_t num = 0;
	uint8_t x = 30;
	uint8_t passwd_flag = 0;
	uint8_t randpasswd_flag = 0;
	uint8_t count = 0;
	uint8_t scanf = 0;
	uint16_t fr_id = 0;
	while(1)
	{
//		OSSchedLock(&err);
//		PS_ValidTempleteNum(&ValidN);
//		PS_ReadSysPara(&AS608Para);
//		PS_ValidTempleteNum(&ValidN);
//	PS_ReadSysPara(&AS608Para);
//		Add_FR(85);
//		if(a == 0)
//		{
//			OSTaskSuspend(&AS608TCB, &err);
//			LED_Open(1,1);
//		}
		
//		ensure = Press_FR();
//		if (ensure == 0)
//		{
//			User_Flag.mode |= 0x01;
//			User_Flag.mode |= 0x02;
//			Clear_OLED_Zone(0,3,128,4);
//			IDENTIFICATION_RIGHT(35,3);
//		}
//		else if (ensure == 3)
//			Clear_OLED_Zone(0,3,128,4),NOT_FINE_FINGER(17,3);
//		else
//			Clear_OLED_Zone(0,3,128,4),IDENTIFICATION_ERROR(35,3);
//		OSMutexPend (&FR_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
//		OSSemPend(&SYNC_SEM,0,OS_OPT_PEND_BLOCKING,0,&err);
//		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
		OSMutexPend (&FR_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
		OSSchedLock(&err);
		for (uint8_t i=0; i<5; i++)
			Key_Buffer[i] = 12;
		Clear_OLED_Zone(0,3,128,4);
		
		while(1)
		{
			PLEASE_INPUT_PASSWD(20,3);
			LCD_SPI_Display_ALL();
			if (scanf != 11)
			{
				scanf = return_Keyvalue();
				while(return_Keyvalue() != 12);
				if(scanf != 12)
				{
	//				Clear_OLED_Zone(0,6,128,7);
					passwd_flag = 0, randpasswd_flag = 0, cnt = 0, count = 0;
					Key_Buffer[num++] = scanf;
					if (num == 11)
					{
						Key_Buffer[11] = 11, scanf = 11, num = 0;
					}
					if (Key_Buffer[0] != 10)
						Display_P16x16_Number(x,6,10);
					else
						Display_P16x16_Number(x,6,scanf);
					x += 8;
				}
			}
			else
			{
				if (Key_Buffer[0] != 10)
				{
					while(*(Key_Buffer + count) != 11)
					{
						if (passwd_flag < 6)
						{
							if (*(Key_Buffer + count) == *(Init_Passwd + cnt))
								passwd_flag++, cnt++;
							else if (*(Key_Buffer + count) == *(Init_Passwd))
								passwd_flag = 1, cnt = 1;
							else
								passwd_flag = 0, cnt = 0;
						}
						count++;
					}
					count = 0, cnt = 0;
					scanf = 12, x = 30, num = 0;
					Clear_OLED_Zone(0,6,128,7);
				}
			}
			if (passwd_flag == 6)
			{
//				Display_P16x16_Number(50,6,passwd_flag);
				break;
			}
		}
		//输入指纹ID
		num = 0;
		while(1)
		{
			Clear_OLED_Zone(0,3,128,4);
			PLEASE_INPUT_ID(20,3);
			LCD_SPI_Display_ALL();
			if (scanf != 11)
			{
				scanf = return_Keyvalue();
				while(return_Keyvalue() != 12);
				if(scanf != 12)
				{
	//				Clear_OLED_Zone(0,6,128,7);
					passwd_flag = 0, randpasswd_flag = 0, cnt = 0, count = 0;
					Key_Buffer[num++] = scanf;
					if (num == 4)
					{
						Key_Buffer[11] = 11, scanf = 11, num = 0;
					}
					Display_P16x16_Number(x,6,scanf);
					x += 8;
				}
			}
			else
			{
				fr_id = Key_Buffer[0]*10 + Key_Buffer[1];
				Clear_OLED_Zone(0,6,128,7);
				break;
			}
		}
		User_Flag.randpaswwd = rand();
		User_Flag.randbuffer[0] = User_Flag.randpaswwd/10000;
		User_Flag.randbuffer[1] = User_Flag.randpaswwd/1000%10;
		User_Flag.randbuffer[2] = User_Flag.randpaswwd/100%10;
		User_Flag.randbuffer[3] = User_Flag.randpaswwd/10%10;
		User_Flag.randbuffer[4] = User_Flag.randpaswwd%10;
		ensure = Add_FR(fr_id);
		for (uint16_t a=0; a<100; a++)
			for (uint16_t b=0; b<10000; b++);
		OSTaskDel(&AS608TCB, &err);
		
		OSSchedUnlock(&err);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

/**刷指纹任务**/
void Press_FR_task(volatile void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	uint8_t ensure = 0;
	while(1)
	{
		OSSchedLock(&err);
		Clear_OLED_Zone(0,3,128,4);
		ensure = Press_FR();
		if (ensure == 0)
		{
			User_Flag.mode |= 0x01;
//			User_Flag.mode |= 0x02;
			Clear_OLED_Zone(0,3,128,4);
			IDENTIFICATION_RIGHT(35,3);
		}
		else if (ensure == 3)
			Clear_OLED_Zone(0,3,128,4),NOT_FINE_FINGER(17,3);
		else
			Clear_OLED_Zone(0,3,128,4),IDENTIFICATION_ERROR(35,3);
		OSSchedUnlock(&err);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

/**NRF24L01任务函数**/
void NRF24L01_task(volatile void *p_arg)
{
	OS_ERR err;
	uint16_t randpasswd = 0;
	uint8_t tbuf[32] = {0xfe};
	p_arg = p_arg;
	NRF24L01_TX_Mode();
	while(1)
	{
		OSSchedLock(&err);
//		Display_P16x16_Number(40,4,0);
		tbuf[0] = 0xfe;
		if ((User_Flag.mode & 0x01) == 0x01)
		{
			tbuf[1] = 0x11;
		}
		if ((User_Flag.mode & 0x02) == 0x02)
		{
			User_Flag.last_randpasswd = User_Flag.randpaswwd;
//			randpasswd = rand();
			while((randpasswd = rand()) < 10000 && (User_Flag.last_randpasswd != randpasswd));
			User_Flag.randpaswwd = randpasswd;
			User_Flag.randbuffer[0] = User_Flag.randpaswwd/10000;
			User_Flag.randbuffer[1] = User_Flag.randpaswwd/1000%10;
			User_Flag.randbuffer[2] = User_Flag.randpaswwd/100%10;
			User_Flag.randbuffer[3] = User_Flag.randpaswwd/10%10;
			User_Flag.randbuffer[4] = User_Flag.randpaswwd%10;
			tbuf[2] = User_Flag.randpaswwd >> 8;
			tbuf[3] = User_Flag.randpaswwd & 0x00ff;
		}
		if ((User_Flag.mode & 0x04) == 0x04)
		{
			tbuf[4] = User_Flag.temp >> 8;
			tbuf[5] = User_Flag.temp & 0x00ff;
		}
		tbuf[6] = 0x01;
		if (NRF24L01_TxPacket(tbuf) == TX_OK)
		{
			for (uint8_t i=0; i<6; i++)
				*(tbuf + i) = 0x00;
			tbuf[2] = User_Flag.randpaswwd >> 8;
			tbuf[3] = User_Flag.randpaswwd & 0x00ff;
			User_Flag.mode = 0;
//			Display_P16x16_Number(40,4,1);
		}
		OSSchedUnlock(&err);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

/**DS18B20任务**/
void DS18B20_task(volatile void *p_arg)
{
	OS_ERR err;
	uint8_t cnt = 0;
	p_arg = p_arg;
	while(1)
	{
		OSSchedLock(&err);
		if (cnt == 0)
		{
			DS18B20_Init();
			cnt = 1;
		}
		User_Flag.temp = DS18B20_Get_Temp();
		User_Flag.mode |= 0x04;
//		User_Flag.mode |= 0x02;
		OSSchedUnlock(&err);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

/**OLED显示函数**/
void OLED_Display_task(volatile void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LCD_SPI_Display_ALL();
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

/**按键扫描任务**/
void Key_Scan_task(volatile void *p_arg)
{
	uint8_t num = 0;
	uint8_t x = 30;
	uint8_t passwd_flag = 0;
	uint8_t randpasswd_flag = 0;
	uint8_t cnt = 0;
	uint8_t count = 0;
	uint8_t scanf = 0;
	OS_ERR err;
	p_arg = p_arg;
//	OSMutexPend (&FR_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
	while(1)
	{
		OSSchedLock(&err);
		if (scanf != 11)
		{
			scanf = return_Keyvalue();
			while(return_Keyvalue() != 12);
			if(scanf != 12)
			{
//				Clear_OLED_Zone(0,6,128,7);
				passwd_flag = 0, randpasswd_flag = 0, cnt = 0, count = 0;
				Key_Buffer[num++] = scanf;
				if (num == 11)
				{
					Key_Buffer[11] = 11, scanf = 11, num = 0;
				}
				if (Key_Buffer[0] != 10)
					Display_P16x16_Number(x,6,10);
				else
					Display_P16x16_Number(x,6,scanf);
				x += 8;
			}
		}
		else
		{
			if (Key_Buffer[0] != 10)
			{
				while(*(Key_Buffer + count) != 11)
				{
					if (passwd_flag < 6)
					{
						if (*(Key_Buffer + count) == *(User_Flag.passwd + cnt))
							passwd_flag++, cnt++;
						else if (*(Key_Buffer + count) == *(User_Flag.passwd))
							passwd_flag = 1, cnt = 1;
						else
							passwd_flag = 0, cnt = 0;
					}
					count++;
				}
				count = 0, cnt = 0;
				while(*(Key_Buffer + count) != 11)
				{
					if (randpasswd_flag < 5)
					{
						if (*(Key_Buffer + count) == *(User_Flag.randbuffer + cnt))
							randpasswd_flag++, cnt++;
						else if (*(Key_Buffer + count) == *(User_Flag.randbuffer))
							randpasswd_flag = 1, cnt = 1;
						else
							randpasswd_flag = 0, cnt = 0;
					}
					count++;
				}
				scanf = 12, x = 30, num = 0;
				Clear_OLED_Zone(0,6,128,7);
				if (passwd_flag == 6)
				{
					User_Flag.mode |= 0x01;
//					Display_P16x16_Number(50,6,passwd_flag);
				}
				else if (randpasswd_flag == 5)
				{
					User_Flag.mode |= 0x03;
//					Display_P16x16_Number(50,6,randpasswd_flag);
				}
			}
		}
		OSSchedUnlock(&err);
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

//定时器1回调函数
void tmr1_callback_led(void *p_tmr, void *p_arg)
{
	IWDG_FREE();
}

//开始任务控制函数
void start_task(volatile void *p_arg)
{
  OS_ERR err;
  CPU_SR_ALLOC();
  p_arg = p_arg;
  
  CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

	OS_CRITICAL_ENTER();//进入临界区代码保护
	/**创建信号量**/
	OSMutexCreate((OS_MUTEX*		)&FR_MUTEX,
							  (CPU_CHAR*		)"TEST_MUTEX",
							  (OS_ERR*			)&err);
//	OSSemCreate ((OS_SEM*		)&SYNC_SEM,
//							 (CPU_CHAR*	)"SYNC_SEM",
//							 (OS_SEM_CTR)0,		
//							 (OS_ERR*		)&err);
	/*创建任务MOTOR*/
	OSTaskCreate ((OS_TCB        *)&MOTORTCB,
								(CPU_CHAR      *)"MOTOR",
								(OS_TASK_PTR    )Motor_task,
								(void          *)0,
								(OS_PRIO        )MOTOR_TASK_PRIO,
								(CPU_STK       *)MOTOR_STK,
								(CPU_STK_SIZE   )MOTOR_STK_SIZE/10,
								(CPU_STK_SIZE   )MOTOR_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*创建任务NRF24L01*/
	OSTaskCreate ((OS_TCB        *)&NRF24L01TCB,
								(CPU_CHAR      *)"NRF24L01",
								(OS_TASK_PTR    )NRF24L01_task,
								(void          *)0,
								(OS_PRIO        )NRF24L01_TASK_PRIO,
								(CPU_STK       *)NRF24L01_STK,
								(CPU_STK_SIZE   )NRF24L01_STK_SIZE/10,
								(CPU_STK_SIZE   )NRF24L01_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*创建任务Add_FR*/
	OSTaskCreate ((OS_TCB        *)&AS608TCB,
								(CPU_CHAR      *)"Add_FR",
								(OS_TASK_PTR    )Add_FR_task,
								(void          *)0,
								(OS_PRIO        )AS608_TASK_PRIO,
								(CPU_STK       *)AS608_STK,
								(CPU_STK_SIZE   )AS608_STK_SIZE/10,
								(CPU_STK_SIZE   )AS608_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*创建任务Press_FR*/
	OSTaskCreate ((OS_TCB        *)&Press_FR_TCB,
								(CPU_CHAR      *)"Press_FR",
								(OS_TASK_PTR    )Press_FR_task,
								(void          *)0,
								(OS_PRIO        )Press_FR_TASK_PRIO,
								(CPU_STK       *)Press_FR_STK,
								(CPU_STK_SIZE   )Press_FR_STK_SIZE/10,
								(CPU_STK_SIZE   )Press_FR_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*创建任务DS18B20*/
	OSTaskCreate ((OS_TCB        *)&DS18B20TCB,
								(CPU_CHAR      *)"DS18B20",
								(OS_TASK_PTR    )DS18B20_task,
								(void          *)0,
								(OS_PRIO        )DS18B20_TASK_PRIO,
								(CPU_STK       *)DS18B20_STK,
								(CPU_STK_SIZE   )DS18B20_STK_SIZE/10,
								(CPU_STK_SIZE   )DS18B20_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*创建任务OLED*/
	OSTaskCreate ((OS_TCB        *)&OLED_Display_TCB,
								(CPU_CHAR      *)"OLED_Display",
								(OS_TASK_PTR    )OLED_Display_task,
								(void          *)0,
								(OS_PRIO        )OLED_Display_TASK_PRIO,
								(CPU_STK       *)OLED_Display_STK,
								(CPU_STK_SIZE   )OLED_Display_STK_SIZE/10,
								(CPU_STK_SIZE   )OLED_Display_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*创建任务Key_Scan*/
	OSTaskCreate ((OS_TCB        *)&Key_Scan_TCB,
								(CPU_CHAR      *)"Key_Scan",
								(OS_TASK_PTR    )Key_Scan_task,
								(void          *)0,
								(OS_PRIO        )Key_Scan_TASK_PRIO,
								(CPU_STK       *)Key_Scan_STK,
								(CPU_STK_SIZE   )Key_Scan_STK_SIZE/10,
								(CPU_STK_SIZE   )Key_Scan_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
//	/*创建软件定时器1*/
//	OSTmrCreate	((OS_TMR         			 *)&tmr1,
//							 (CPU_CHAR             *)"tmr1_led",
//							 (OS_TICK               )0,//初始延迟为0*10ms
//							 (OS_TICK               )100,//定时周期为50*10ms
//							 (OS_OPT                )OS_OPT_TMR_PERIODIC,//周期模式
//							 (OS_TMR_CALLBACK_PTR   )tmr1_callback_led,
//							 (void                 *)0,
//							 (OS_ERR               *)&err);
	
	OS_CRITICAL_EXIT();//退出临界区代码保护
//	OSMutexPend (&FR_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
							 
//	OSTaskSuspend(&AS608TCB, &err);
	OSTmrStart(&tmr1,&err);
	OSTaskDel((OS_TCB*)&StartTackTCB, &err);
}
