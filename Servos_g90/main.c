#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "systick.h"
#include "stm32f10x_adc.h"

uint16_t adc;
uint8_t goc;

void RCC_Config(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}

void TIM_Config() {
    TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
    TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimBaseInitStructure.TIM_Prescaler = 72-1;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimBaseInitStructure);
    
    TIM_Cmd(TIM2, ENABLE);
}

void GPIO_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	//pin ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//pin PWM
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void PWM_Config() {
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =0; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
}

void ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode =DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
}

void PWM_setDuty(TIM_TypeDef *TIMx, uint32_t Channel, uint16_t duty)
{
    uint16_t Period = TIMx->ARR + 1;
    uint32_t Pulse = (duty * Period) / 100;

    switch (Channel)
    {
    case TIM_Channel_1:
        TIMx->CCR1 = Pulse;
        break;
    case TIM_Channel_2:
        TIMx->CCR2 = Pulse;
        break;
    case TIM_Channel_3:
        TIMx->CCR3 = Pulse;
        break;
    case TIM_Channel_4:
        TIMx->CCR4 = Pulse;
        break;
    default:
        break;
    }
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servo_Write( TIM_TypeDef *tim, uint16_t Channel, uint8_t Angel)
{
	// Angel: 0->180
	// CCR: 1->2(chu ki xung cap(Ton))	
	//uint16_t CCR = (Angel -0)*(2000-1000) /(180-0) + 1000 // 100 la goc nho nhat co the thay = 0
	uint16_t CCR = map(Angel, 0, 180, 544, 2400);
	switch(Channel)
		{
		case TIM_Channel_1:
			tim ->CCR1= CCR;
			break;
		case TIM_Channel_2:
			tim ->CCR2= CCR;
			break;
		case TIM_Channel_3:
			tim ->CCR3= CCR;
			break;
		case TIM_Channel_4:
			tim ->CCR4= CCR;
			break;
			
		}
}
int main() {
	SystemInit();
	SystickInit();
    RCC_Config();
	ADC_Config();
    GPIO_Config();
    PWM_Config();
    TIM_Config();
    

    
    while(1) {
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while (! ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
		adc= ADC_GetConversionValue(ADC1);
		goc = adc * 180 / 4095;
		servo_Write(TIM2, TIM_Channel_1, goc);
    }
    return 0;
}
