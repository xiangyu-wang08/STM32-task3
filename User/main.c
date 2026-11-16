#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
#include "Serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int f;
int zkb;

int main(void)
{
	OLED_Init();
	PWM_Init();
	Serial_Init();
	IC_Init();
	
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
		
		OLED_ShowNum(1,3,IC_GetFreq(),6);
		OLED_ShowNum(2,6,IC_GetDuty(),2);
		
	}
	
	
}
