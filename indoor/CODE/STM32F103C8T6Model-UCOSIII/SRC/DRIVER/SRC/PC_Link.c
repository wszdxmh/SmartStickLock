/**
  ************************************
  * @file    PC_Link.c
  * @author  wszdxmh
  * @version V3.5.0
  * @date    2015.09.27
  * @brief   ��е�ִ��ڵ��Ժ���
	************************************
*/

#include "PC_Link.h"
#include "ANO-Tech.h"
#include "includes.h"
#include "app.h"

uint8_t buf[4] = {0xff, 0xff, 0xff, 0xff};

extern struct MANIPULATOR_VALUE Action_table;

//����ȫ�ֱ���
uint16_t Flag,Time;

/**
  ************************************
  * @brief  ����int�͸�PC
  * @param  header:ͷ��   date:Ҫ���͵�����
  * @retval None
	************************************
*/
void Send_PC(uint8_t header, uint16_t date)
{
  uint8_t buf[4];
  buf[0] = header;
  buf[1] = date >> 8;
  buf[2] = date & 0xff;
  buf[3] = ~header;
  for(uint8_t i=0; i<4; i++)
  {
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
    USART_SendData(USART1, buf[i]);
  }
}

/**
  ************************************
  * @brief  �жϺ���
  * @param  None
  * @retval �Խ��յ������ݽ����ж�
	************************************
*/
//static void Judge(void)
//{
//	switch(buf[0])
//	{
//		case 0x00:
//		{
//			if(HEADER_JUDGE(0x00))
//				switch(buf[1])
//				{
//					case 0x01:Action_table.Waist += 10;Send_PC(0x00, Action_table.Waist);break;
//					case 0x02:Action_table.Waist -= 10;Send_PC(0x00, Action_table.Waist);break;
//					case 0x03:Action_table.Big_Arm += 10;Send_PC(0x01, Action_table.Big_Arm);break;
//					case 0x04:Action_table.Big_Arm -= 10;Send_PC(0x01, Action_table.Big_Arm);break;
//					case 0x05:Action_table.Little_Arm += 10;Send_PC(0x02, Action_table.Little_Arm);break;
//					case 0x06:Action_table.Little_Arm -= 10;Send_PC(0x02, Action_table.Little_Arm);break;
//					case 0x07:Action_table.Wrist += 10;Send_PC(0x03, Action_table.Wrist);break;
//					case 0x08:Action_table.Wrist -= 10;Send_PC(0x03, Action_table.Wrist);break;
//					case 0x0b:Action_table.Wrist_Turn += 10;Send_PC(0x05, Action_table.Wrist_Turn);break;
//					case 0x0c:Action_table.Wrist_Turn -= 10;Send_PC(0x05, Action_table.Wrist_Turn);break;
//					case 0x09:Action_table.Gripper += 10;Send_PC(0x04, Action_table.Gripper);break;
//					case 0x0a:Action_table.Gripper -= 10;Send_PC(0x04, Action_table.Gripper);break;
//					default:break;
//				}
//		}
//		case 0x01:if(HEADER_JUDGE(0x01)){UINT8_T_TO_INT(Action_table.Waist);Send_PC(0x00, Action_table.Waist);}break;
//		case 0x02:if(HEADER_JUDGE(0x02)){UINT8_T_TO_INT(Action_table.Big_Arm);Send_PC(0x01, Action_table.Big_Arm);}break;
//		case 0x03:if(HEADER_JUDGE(0x03)){UINT8_T_TO_INT(Action_table.Little_Arm);Send_PC(0x02, Action_table.Little_Arm);}break;
//		case 0x04:if(HEADER_JUDGE(0x04)){UINT8_T_TO_INT(Action_table.Wrist);Send_PC(0x03, Action_table.Wrist);}break;
//		case 0x06:if(HEADER_JUDGE(0x06)){UINT8_T_TO_INT(Action_table.Wrist_Turn);Send_PC(0x05, Action_table.Wrist_Turn);}break;
//		case 0x05:if(HEADER_JUDGE(0x05)){UINT8_T_TO_INT(Action_table.Gripper);Send_PC(0x04, Action_table.Gripper);}break;
//		case 0x07:if(HEADER_JUDGE(0x07)){Flag = 1;}break;
//		default:break;
//	}
//}

/**
  ************************************
  * @brief  ����1�жϺ���
  * @param  buf[0]Ϊͷ��  buf[3]Ϊβ��,��ͷ��ȡ��  buf[1]ָ��λ
  * @retval None
	************************************
*/
//void USART1_IRQHandler(void)
//{
//	static uint8_t count = 0;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		buf[count] = USART_ReceiveData(USART1);
//    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		count++;
//	}
//  if(count == 4)
//  {
//    count = 0;
//		Judge();
//  }
//}

//void USART1_IRQHandler(void)
//{
//	#if SYSTEM_SUPPORT_OS
//		OSIntEnter();
//	#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		//ANO_DT_Data_Receive_Prepare(USART_ReceiveData(USART1));
//		USART_SendData(USART1, USART_ReceiveData(USART1));
//    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//	}
//	#if SYSTEM_SUPPORT_OS
//		OSIntExit();
//	#endif
//}
