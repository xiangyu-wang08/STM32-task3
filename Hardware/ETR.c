#include "stm32f10x.h"                  // Device header

void ETR_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE  );
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	
	TIM_ETRClockMode2Config(TIM2 , TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_Inverted,0x02);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1  ;
	TIM_TimeBaseInitStructure.TIM_CounterMode =TIM_CounterMode_Up  ;
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1 ;          //ARR  
	TIM_TimeBaseInitStructure.TIM_Prescaler =  1-1 ;       //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0 ;
	TIM_TimeBaseInit( TIM2 , &TIM_TimeBaseInitStructure );

	
	
	
	TIM_Cmd(TIM2,ENABLE);
}

uint32_t ETR_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}
