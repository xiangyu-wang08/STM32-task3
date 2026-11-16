#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1,1,"f:");
	OLED_ShowString(2,1,"Duty:00%");
	
	PWM_Setf(100);			//默认频率100Hz
	PWM_SetCompare1 (10);             //默认占空比10%
	
	while (1)
	{
		OLED_ShowNum(1,3,IC_GetFreq(),5);
		OLED_ShowNum(2,6,IC_GetDuty(),2);
		
	}
	
	
}
