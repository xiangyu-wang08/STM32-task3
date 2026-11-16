#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE  );
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	
	
	
	TIM_InternalClockConfig(TIM3);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1  ;
	TIM_TimeBaseInitStructure.TIM_CounterMode =TIM_CounterMode_Up  ;
	TIM_TimeBaseInitStructure.TIM_Period = 100-1 ;          //ARR    占空比
	TIM_TimeBaseInitStructure.TIM_Prescaler =  7200-1 ;       //PSC     默认PWM频率100Hz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0 ;
	TIM_TimeBaseInit( TIM3 , &TIM_TimeBaseInitStructure );
	
	
	TIM_OCInitTypeDef TIM_OCInitSructure;
	TIM_OCStructInit(&TIM_OCInitSructure);
	TIM_OCInitSructure.TIM_OCMode =TIM_OCMode_PWM1 ;
	TIM_OCInitSructure.TIM_OCPolarity =TIM_OCPolarity_High ;
	TIM_OCInitSructure.TIM_OutputState =TIM_OutputState_Enable ;
	TIM_OCInitSructure.TIM_Pulse =0 ;       //CCR
	TIM_OC1Init(TIM3,&TIM_OCInitSructure);

	
	
	TIM_Cmd(TIM3, ENABLE);

}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM3,Compare);


}

void PWM_SetPrescaler(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM3 , Prescaler ,TIM_PSCReloadMode_Immediate);



}

void PWM_Setf(uint32_t f)
{
	TIM_PrescalerConfig(TIM3 , (uint32_t)(720000/f)-1 ,TIM_PSCReloadMode_Immediate);
}

