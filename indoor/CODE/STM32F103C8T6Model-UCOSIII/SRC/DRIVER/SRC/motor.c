
#include "motor.h"

void Motor_MOS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  ************************************
  * @brief  PWM�ĳ�ʼ������
  * @param  arr: �Զ���װ����ֵ   psc:Ԥ��Ƶֵ
  * @retval arr=19999  psc=71     f=72000000/((19999+1)*(71+1))=50Hz
	************************************
*/
void Motor_Init(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//��ʼ��PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	//���ø�����Ϊ�����������,���TIM4 ��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //TIM4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channel2 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	//��ʼ��TIM4 PWMȫ��ͨ��
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}

/**
  ************************************
  * @brief  ���1��ת
  * @param  pwm��ռ�ձ�
  * @retval None
	************************************
*/
void Motor1_Foreward(uint16_t pwm)
{
	MOTOR1_DIRECRION(Bit_SET);
	MOTOR1_SPEED(pwm);
}

/**
  ************************************
  * @brief  ���1��ת
  * @param  pwm��ռ�ձ�
  * @retval None
	************************************
*/
void Motor1_Reversal(uint16_t pwm)
{
	MOTOR1_DIRECRION(Bit_RESET);
	MOTOR1_SPEED(pwm);
}

/**
  ************************************
  * @brief  ���2��ת
  * @param  pwm��ռ�ձ�
  * @retval None
	************************************
*/
void Motor2_Foreward(uint16_t pwm)
{
	MOTOR2_DIRECRION(Bit_SET);
	MOTOR2_SPEED(pwm);
}

/**
  ************************************
  * @brief  ���2��ת
  * @param  pwm��ռ�ձ�
  * @retval None
	************************************
*/
void Motor2_Reversal(uint16_t pwm)
{
	MOTOR2_DIRECRION(Bit_RESET);
	MOTOR2_SPEED(pwm);
}
