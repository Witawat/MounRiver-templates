/*
CH32V��ʱ����������
Ŀǰ֧��TIM2/3/4�����Ķ�ʱ���ܣ�������Ӱ���TIM1�����벶������Ƚϡ��������ȹ��ܡ�

ʹ�÷�����
void myfun(void)
{
    
}
TIM_Init(TIM2,5000,myfun);

*/
#ifndef _CH32V_TIM_H_
#define _CH32V_TIM_H_ 

#include <CH32V103.h>
#include "ch32v10x_gpio.h"
typedef void (*voidFuncPtr)(void);
volatile static voidFuncPtr tim_fun[3] = { NULL };
//========================================================================
// ����: TIM��ʼ��.
// ����: ��ʱ��(TIM2/3/4)�����ڣ�us�����ص�����
// ����: none.
//========================================================================
void TIM_attachInterrupt(TIM_TypeDef* TIMx, uint32_t period, void (*userFunc)(void))
{
    // //��ʼ��TIM NVIC�������ж����ȼ�����
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    //1.ʹ��ʱ�ӣ���¼�ص�����
    if(TIMx == TIM2)
	{
        tim_fun[0] = userFunc;
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //TIM2�ж�
        TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��TIM2�жϣ���������ж�
	}
	else if(TIMx == TIM3)
	{
        tim_fun[1] = userFunc;
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //TIM3�ж� 
        TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��TIM3�жϣ���������ж�
	}
	else if(TIMx == TIM4)
	{
        tim_fun[2] = userFunc;
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //TIM4�ж�
        TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��TIM4�жϣ���������ж�
	}

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //������ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //������Ӧ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ��ͨ��1�ж�
    NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC

    //2.���ö�ʱ����
    if (period > 50000) 
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;  	//Ԥ��Ƶ������
        TIM_TimeBaseInitStructure.TIM_Period = period / 100-1;  	//�趨�������Զ���װֵ
    }
    else 
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 71;   	//Ԥ��Ƶ������
        TIM_TimeBaseInitStructure.TIM_Period = period-1;  	//�趨�������Զ���װֵ
    }

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIMx, &TIM_TimeBaseInitStructure);

    TIM_Cmd( TIMx, ENABLE);
}

//========================================================================
// ����: ��������
// ����: ��ʱ��������
// ����: none.
//========================================================================
void TIM_Duty_Updata(TIM_TypeDef* TIMx, uint32_t period)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //1.���ö�ʱ����
    if (period > 50000) 
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;  	//Ԥ��Ƶ������
        TIM_TimeBaseInitStructure.TIM_Period = period / 100-1;  	//�趨�������Զ���װֵ
    }
    else 
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 71;   	//Ԥ��Ƶ������
        TIM_TimeBaseInitStructure.TIM_Period = period-1;  	//�趨�������Զ���װֵ
    }
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIMx, &TIM_TimeBaseInitStructure);
}

//========================================================================
// ����: �رն�ʱ��
// ����: ��ʱ��
// ����: none.
//========================================================================
void TIM_Disable(TIM_TypeDef* TIMx)
{
	TIM_Cmd( TIMx, DISABLE);   //�رն�ʱ��
}

//========================================================================
// ����: ������ʱ��
// ����: ��ʱ��
// ����: none.
//========================================================================
void TIM_Enable(TIM_TypeDef* TIMx)
{
	TIM_Cmd( TIMx, ENABLE);   //ʹ�ܶ�ʱ��
}

//========================================================================
// ����: ��ʱ�����ռ��
// ����: ��ʱ��
// ����: none.
//========================================================================
void TIM_detachInterrupt(TIM_TypeDef* TIMx)
{
    if(TIMx == TIM2)
	{
        tim_fun[0] = NULL;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, DISABLE );
	}
	else if(TIMx == TIM3)
	{
        tim_fun[1] = NULL;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, DISABLE );
	}
	else if(TIMx == TIM4)
	{
        tim_fun[2] = NULL;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, DISABLE );
	}
}

//========================================================================
// ����: TIM�жϺ���.
// ����: none.
// ����: none.
//========================================================================

extern "C"{
    void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
    void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
    void TIM4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

    void TIM2_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���TIM2�ж��Ƿ�����
        {

            TIM_ClearITPendingBit(TIM2,TIM_IT_Update);    //���TIM2���жϹ���λ��

            if(tim_fun[0]!=NULL)tim_fun[0]();
        }
    }

    void TIM3_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���TIM3�ж��Ƿ�����
        {

            TIM_ClearITPendingBit(TIM3,TIM_IT_Update);    //���TIM3���жϹ���λ��
           
            if(tim_fun[1]!=NULL)tim_fun[1]();
        }
    }

    void TIM4_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���TIM4�ж��Ƿ�����
        {

            TIM_ClearITPendingBit(TIM4,TIM_IT_Update);    //���TIM4���жϹ���λ��

            if(tim_fun[2]!=NULL)tim_fun[2]();
        }
    }

} // extern "C"

#endif
