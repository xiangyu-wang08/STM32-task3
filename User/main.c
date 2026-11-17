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
#include "Key.h"


int f;
int zkb;
int ETRf;
int cnt;
uint8_t key;
uint8_t mode;

int main(void)
{
	Key_Init();
	OLED_Init();
	PWM_Init();
	Serial_Init();				  
	IC_Init();         				//模式1   输入捕获测周模式    （默认模式）
	mode = 1;
	
//	ETR_Init();						//模式2     ETR测频模式
//	Timer_Init();
//	cnt= ETR_GetCounter();
	
	OLED_ShowString(1,1,"f:      Hz");
	OLED_ShowString(2,1,"Duty:00%");
	OLED_ShowString(3,1,"Mode:1");
	
	PWM_Setf(100);			//默认频率100Hz
	PWM_SetCompare1 (10);             //默认占空比10%
	
	while (1)
	{
		key=Key_GetNum();
		if (key  ==1)			//按下按键    
		{
			if (mode ==1)    		//切换到ETR测频模式
			{
				mode =2;
				TIM_DeInit(TIM2);
				ETR_Init();	
				cnt= ETR_GetCounter();	
				Timer_Init();
				OLED_ShowNum(3,6,2,1);
				OLED_ShowString(2,1,"Duty:--%");
			}
			else          //切换到输入捕获测周模式
			{
				mode =1;
				TIM_DeInit(TIM4);
				TIM_DeInit(TIM2);
				IC_Init();
				OLED_ShowNum(3,6,1,1);
			}
			
		}
		

		
		if (Serial_RxFlag ==1)					//串口控制部分
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
		
		if (mode ==1) 			//输入捕获测周模式OLED显示
		{
			OLED_ShowNum(1,3,IC_GetFreq(),6);
			OLED_ShowNum(2,6,IC_GetDuty(),2);	
		}
		else 
		{
			OLED_ShowNum(1,3,ETRf,6);	      //ETR测频模式OLED显示
		}



		
	}
	
	
}




void TIM4_IRQHandler(void)                              //定时100ms中断  测频法
{
	if (TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		int nowcnt = ETR_GetCounter();
		if (nowcnt<cnt)                        //发生超出范围
		{
			ETRf = ( nowcnt - (cnt-65536) ) *10 ;                  //默认0.1ms数一次，所以*10直接得到频率
			cnt = nowcnt;			
		}
		else 									//计数器没有超出65535范围
		{
			ETRf = (nowcnt - cnt ) *10;
			cnt = nowcnt;
		}
	
	
	}

	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

