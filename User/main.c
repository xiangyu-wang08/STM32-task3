#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
#include "Serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ETR.h"
#include "Timer.h"


int f;
int zkb;
int ETRf;
int cnt;

int main(void)
{
	OLED_Init();
	PWM_Init();
	Serial_Init();
//	IC_Init();
	ETR_Init();
	Timer_Init();
	cnt= ETR_GetCounter();
	
	OLED_ShowString(1,1,"f:");
	OLED_ShowString(2,1,"Duty:00%");
	
	PWM_Setf(100);			//默认频率100Hz
	PWM_SetCompare1 (10);             //默认占空比10%
	
	while (1)
	{
		if (Serial_RxFlag ==1)
		{
			if (Serial_RxPacket[0]=='f')    			//调整频率
			{
				f = atoi (Serial_RxPacket+1);
				PWM_Setf(f);
			}
			else if (Serial_RxPacket[strlen(Serial_RxPacket)-1] =='%'   )			//调整占空比
			{
				Serial_RxPacket[strlen(Serial_RxPacket)-1] = '\0';
				
				zkb = atoi(Serial_RxPacket);
				PWM_SetCompare1(zkb);
				
			}
			
			
			
			
			
			
			
			Serial_RxFlag=0;
		}
		
//		OLED_ShowNum(1,3,IC_GetFreq(),6);
//		OLED_ShowNum(2,6,IC_GetDuty(),2);

		OLED_ShowNum(1,3,ETRf,6);

		
	}
	
	
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		int nowcnt = ETR_GetCounter();
		if (nowcnt<cnt)                        //发生超出范围
		{
			ETRf = ( nowcnt - (cnt-65536) ) *10 ;                  //默认0.1ms计数一次，所以*10直接得到频率
			cnt = nowcnt;			
		}
		else 
		{
			ETRf = (nowcnt - cnt ) *10;
			cnt = nowcnt;
		}
	
	
	}

	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

