
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
  * @brief  TIM4��ʱ���ĳ�ʼ������
  * @param  period: �Զ���װ����ֵ   prescaler:Ԥ��Ƶֵ
  * @retval None
	************************************
*/
void Remote_Init(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//����JTAG��Ҫ�ȿ�AFIOʱ��
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
	
	//���ø�����Ϊ�����������,���TIM3 CH3 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	//��ʼ��TIM3 PWMȫ��ͨ��
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���	
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
//	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM1
	TIM_SetCompare1(TIM4,900);
}

void Remote_Send(void)
{
	uint8_t ircode;
	/*CMD1*/
	//������ʼ��
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
	//������
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
