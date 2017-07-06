
#include "infrared.h"
#include "stm32f10x.h"

uint8_t num[9]={0x79,0x09,0x00,0x50,0x02,0x11,0x20,0x00,0xF0};

void Delay_us(uint16_t us)
{
	for(uint16_t a=us; a>0; a--)
		for(uint16_t b=8; b>0; b--);
}

void Delay_ms(uint16_t ms)
{
	for(uint16_t a=ms; a>0; a--)
		for(uint16_t b=10000; b>0; b--);
}

/**
  ************************************
  * @brief  TIM4定时器的初始化函数
  * @param  period: 自动重装周期值   prescaler:预分频值
  * @retval None
	************************************
*/
void Remote_Init(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器1时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE); //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁用JTAG，要先开AFIO时钟
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
	
	//设置该引脚为复用输出功能,输出TIM3 CH3 CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	//初始化TIM3 PWM全部通道
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器	
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	
//	TIM_Cmd(TIM4, ENABLE);  //使能TIM1
	TIM_SetCompare1(TIM4,900);
}

void Remote_Send(void)
{
	uint8_t ircode;
	/*CMD1*/
	//发送起始码
	LIG_HIGHT();
	Delay_ms(9);
	LIG_LOW();
	Delay_ms(4);
	Delay_us(500);
	for(uint8_t a=0; a<5; a++)
	{
		ircode = num[a];
		if(a < 4)	
			for(uint8_t b=0; b<8; b++)
				LEFT(ircode)
		else		
			for(uint8_t c=0; c<3; c++)
				LEFT(ircode)
		
	}
	//连接码
	LIG_HIGHT();
	Delay_us(600);
	LIG_LOW();
	Delay_ms(20);
	/*CMD2*/
	for(uint8_t a=5; a<9; a++)
	{
		ircode = num[a];
		for(uint8_t b=0; b<8; b++)
				LEFT(ircode)
	}
	if(num[8] & 0x80)
		HIGHT()
	else
		LOW()
}

void Set_Temp(uint8_t temp)
{
	num[1] = Temperature[temp - 16];
	num[8] = Verification_Code[temp - 16];
	Remote_Send();
}

void Air_Conditioner_Off(void)
{
	num[0] = 0x71;
	num[1] = 0x09;
	num[8] = 0x70;
	Remote_Send();
	num[0] = 0x79;
}

void Light_Station(uint8_t station)
{
	if(station == 1)
		num[3] |= (1<<5);
	else
		num[3] &= (0<<5);
	Remote_Send();
}
