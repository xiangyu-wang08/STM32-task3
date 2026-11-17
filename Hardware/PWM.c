#include "stm32f10x.h"                  // Device header


static uint32_t dutyPercent = 10U;        // 默认占空比 10%

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
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1 ;          //ARR    
	TIM_TimeBaseInitStructure.TIM_Prescaler =  12-1 ;       //PSC     默认PWM频率100Hz
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
//	TIM_PrescalerConfig(TIM3 , (uint32_t)(720000/f)-1 ,TIM_PSCReloadMode_Immediate);            //原来的频率计算代码，但是发现f 90000到90001输出有跳变
																								//下面是ai辅助完成的代码优化
	
	uint32_t periodTicks;
	uint32_t compare;

	if (f < 100U || f > 100000U)
	{
		return;
	}
	
	periodTicks = (6000000U + f / 2U) / f;               // 周期计数，四舍五入
	compare = (periodTicks * dutyPercent + 50U) / 100U;  // 按原占空比重算高电平计数

	TIM_Cmd(TIM3, DISABLE);
	TIM_SetAutoreload(TIM3, (uint16_t)(periodTicks - 1U));
	TIM_SetCompare1(TIM3, (uint16_t)compare);
	TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
	TIM_Cmd(TIM3, ENABLE);
}

void PWM_SetDutyPercent(uint32_t percent)
{
    uint32_t periodTicks;

    if (percent < 10U || percent > 90U  )
        return;

    dutyPercent = percent;
    periodTicks = TIM3->ARR + 1U;

    TIM_SetCompare1(TIM3,(uint16_t)((periodTicks * dutyPercent + 50U) / 100U));
}

