/**********************************************************
 * �ÿ�ΪCH32V103��8λ����������.
 * *******************************************************/
#ifndef _CH32V_TOUCH_H
#define _CH32V_TOUCH_H

#include <CH32V103.h>
#include "CH32V_TFTLCD.h"

#define FAC_MIN							0.9
#define FAC_MAX							1.1

#define TOUCH_READ_TIMES 				5 	//��ȡ����
#define TOUCH_LOST_VAL 					1	//����ֵ

/***********************************************  SM245�������Ŷ���  *****************************************************/
// #define SM245_YP_PIN                    PA1        //AD
// #define SM245_SET_YP_OUT                pinMode(PA1, GPIO_Mode_Out_PP)
// #define SM245_SET_YP_IN                 pinMode(PA1, GPIO_Mode_AIN) 
// #define SM245_YP_ADC_PIN                ADC_PA1  

// #define SM245_YM_PIN                    PB15
// #define SM245_SET_YM_OUT                pinMode(PB15, GPIO_Mode_Out_PP)
// #define SM245_SET_YM_IN                 pinMode(PB15, GPIO_Mode_IN_FLOATING)

// #define SM245_XP_PIN                    PB14
// #define SM245_SET_XP_OUT                pinMode(PB14, GPIO_Mode_Out_PP)
// #define SM245_SET_XP_IN                 pinMode(PB14, GPIO_Mode_IN_FLOATING)

// #define SM245_XM_PIN                    PA2        //AD
// #define SM245_SET_XM_OUT                pinMode(PA2, GPIO_Mode_Out_PP)
// #define SM245_SET_XM_IN                 pinMode(PA2, GPIO_Mode_AIN)
// #define SM245_XM_ADC_PIN                ADC_PA2  

/***********************************************  ili9341�������Ŷ���  *****************************************************/
// #define LCD9341_XP_PIN                  PB9
// #define LCD9341_SET_XP_OUT              pinMode(PB9, GPIO_Mode_Out_PP)
// #define LCD9341_SET_XP_IN               pinMode(PB9, GPIO_Mode_IN_FLOATING)

// #define LCD9341_XM_PIN                  PA3        //AD
// #define LCD9341_SET_XM_OUT              pinMode(PA3, GPIO_Mode_Out_PP)
// #define LCD9341_SET_XM_IN               pinMode(PA3, GPIO_Mode_AIN)
// #define LCD9341_XM_ADC_PIN              ADC_PA3  

// #define LCD9341_YP_PIN                  PA2        //AD
// #define LCD9341_SET_YP_OUT              pinMode(PA2, GPIO_Mode_Out_PP)
// #define LCD9341_SET_YP_IN               pinMode(PA2, GPIO_Mode_AIN) 
// #define LCD9341_YP_ADC_PIN              ADC_PA2  

// #define LCD9341_YM_PIN                  PB8
// #define LCD9341_SET_YM_OUT              pinMode(PB8, GPIO_Mode_Out_PP)
// #define LCD9341_SET_YM_IN               pinMode(PB8, GPIO_Mode_IN_FLOATING)

uint16_t touch_read_raw_x(uint8_t xp,uint8_t xm,uint8_t yp,uint8_t ym);     //����x�������������
uint16_t touch_read_raw_y(uint8_t xp,uint8_t xm,uint8_t yp,uint8_t ym);	    //����y�������������
uint16_t touch_read_pressure(uint8_t xp,uint8_t xm,uint8_t yp,uint8_t ym);	    //����ѹ��ֵ
uint8_t tp_read_xy2(uint16_t *x,uint16_t *y,uint8_t xp,uint8_t xm,uint8_t yp,uint8_t ym);  //ͨ��2�β������õ���׼ȷ������ֵ

//========================================================================
// ����: ��ȡX��������.
// ����: none.
// ����: ����������.y
//========================================================================
uint16_t touch_read_raw_x(uint8_t xp,uint8_t xm,uint8_t yp,uint8_t ym)
{
	uint16_t i, j;
	uint16_t buf[TOUCH_READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
    ADC_Name adc_name;

    if(yp < 8)adc_name = (ADC_Name)yp;
    else if(yp==PB0)adc_name = ADC_PB0;
    else if(yp==PB1)adc_name = ADC_PB1;
    else if((yp>=PC0) && (yp<=PC5))adc_name = (ADC_Name)(yp - 22);
    else  return 0;

    pinMode(xm, GPIO_Mode_Out_PP); // LCD9341_SET_XM_OUT;
    pinMode(xp, GPIO_Mode_Out_PP); // LCD9341_SET_XP_OUT;
    pinMode(yp, GPIO_Mode_AIN);   // LCD9341_SET_YP_IN;   
    pinMode(ym, GPIO_Mode_AIN);   // LCD9341_SET_YM_IN;
    digitalWrite(xp, 1);     // LCD9341_XP_PIN = 1;
    digitalWrite(yp, 0);     // LCD9341_YP_PIN = 0;
    digitalWrite(ym, 0);     // LCD9341_YM_PIN = 0;
    digitalWrite(xm, 0);     // LCD9341_XM_PIN = 0;
    CH32V_ADC_Init(adc_name, ADC_SAMPLE_55_5, ADC_BIT_12);
    for(i=0;i<TOUCH_READ_TIMES;i++)buf[i] = ADC_Read(adc_name); 		    
    for(i=0;i<TOUCH_READ_TIMES-1; i++)//����
    {
        for(j=i+1;j<TOUCH_READ_TIMES;j++)
        {
            if(buf[i]>buf[j])//��������
            {
                temp=buf[i];
                buf[i]=buf[j];
                buf[j]=temp;
            }
        }
    }	 
    sum=0;
    for(i=TOUCH_LOST_VAL;i<TOUCH_READ_TIMES-TOUCH_LOST_VAL;i++)sum+=buf[i];
    temp=sum/(TOUCH_READ_TIMES-2*TOUCH_LOST_VAL);
    pinMode(yp, GPIO_Mode_Out_PP);
    pinMode(ym, GPIO_Mode_Out_PP);
    return temp;
}

//========================================================================
// ����: ��ȡY��������.
// ����: none.
// ����: ����������.
//========================================================================
uint16_t touch_read_raw_y(uint8_t xp,uint8_t xm,uint8_t yp,uint8_t ym)
{
	uint16_t i, j;
	uint16_t buf[TOUCH_READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
    ADC_Name adc_name;

    if(xm < 8)adc_name = (ADC_Name)xm;
    else if(xm==PB0)adc_name = ADC_PB0;
    else if(xm==PB1)adc_name = ADC_PB1;
    else if((xm>=PC0) && (xm<=PC5))adc_name = (ADC_Name)(xm - 22);
    else  return 0;

    pinMode(yp, GPIO_Mode_Out_PP); // LCD9341_SET_YP_OUT;
    pinMode(ym, GPIO_Mode_Out_PP); // LCD9341_SET_YM_OUT;
    pinMode(xp, GPIO_Mode_AIN); // LCD9341_SET_XP_IN;   
    pinMode(xm, GPIO_Mode_AIN); // LCD9341_SET_XM_IN;
    digitalWrite(yp, 1);    // LCD9341_YP_PIN = 1;
    digitalWrite(ym, 0);    // LCD9341_YM_PIN = 0;
    digitalWrite(xp, 0);    // LCD9341_XP_PIN = 0;
    digitalWrite(xm, 0);    // LCD9341_XM_PIN = 0;

    CH32V_ADC_Init(adc_name, ADC_SAMPLE_55_5, ADC_BIT_12);
    for(i=0;i<TOUCH_READ_TIMES;i++)buf[i]=ADC_Read(adc_name); 		    
    for(i=0;i<TOUCH_READ_TIMES-1; i++)//����
    {
        for(j=i+1;j<TOUCH_READ_TIMES;j++)
        {
            if(buf[i]>buf[j])//��������
            {
                temp=buf[i];
                buf[i]=buf[j];
                buf[j]=temp;
            }
        }
    }	 
    sum=0;
    for(i=TOUCH_LOST_VAL;i<TOUCH_READ_TIMES-TOUCH_LOST_VAL;i++)sum+=buf[i];
    temp=sum/(TOUCH_READ_TIMES-2*TOUCH_LOST_VAL);
    pinMode(xm, GPIO_Mode_Out_PP);  //LCD9341_SET_XM_OUT;
    pinMode(xp, GPIO_Mode_Out_PP);  //LCD9341_SET_XP_OUT;
    return temp;
}

//========================================================================
// ����: ��ȡ������ѹ��ֵ.
// ����: none.
// ����: ������ѹ��ֵ.
//========================================================================
uint16_t touch_read_pressure(uint8_t xp,uint8_t xm,uint8_t yp,uint8_t ym)
{
    int z1,z2;
    ADC_Name adc_name1;
    ADC_Name adc_name2;

    if(xm < 8)adc_name1 = (ADC_Name)xm;
    else if(xm==PB0)adc_name1 = ADC_PB0;
    else if(xm==PB1)adc_name1 = ADC_PB1;
    else if((xm>=PC0) && (xm<=PC5))adc_name1 = (ADC_Name)(xm - 22);
    else  return 0;

    if(yp < 8)adc_name2 = (ADC_Name)yp;
    else if(yp==PB0)adc_name2 = ADC_PB0;
    else if(yp==PB1)adc_name2 = ADC_PB1;
    else if((yp>=PC0) && (yp<=PC5))adc_name2 = (ADC_Name)(yp - 22);
    else  return 0;

    pinMode(xp, GPIO_Mode_Out_PP);  // LCD9341_SET_XP_OUT;
    pinMode(ym, GPIO_Mode_Out_PP);  // LCD9341_SET_YM_OUT;
    pinMode(xm, GPIO_Mode_AIN);  // LCD9341_SET_XM_IN;
    pinMode(yp, GPIO_Mode_AIN);  // LCD9341_SET_YP_IN;
    digitalWrite(xp, 0);    // LCD9341_XP_PIN = 0;
    digitalWrite(ym, 1);    // LCD9341_YM_PIN = 1; 
    digitalWrite(xm, 0);    // LCD9341_XM_PIN = 0;
    digitalWrite(yp, 0);    // LCD9341_YP_PIN = 0;

    CH32V_ADC_Init(adc_name1, ADC_SAMPLE_55_5, ADC_BIT_12);
    CH32V_ADC_Init(adc_name2, ADC_SAMPLE_55_5, ADC_BIT_12);
    z1 = ADC_Read(adc_name1); 
    z2 = ADC_Read(adc_name2);

    pinMode(xm, GPIO_Mode_Out_PP);        // LCD9341_SET_XM_OUT;
    pinMode(yp, GPIO_Mode_Out_PP);        // LCD9341_SET_YP_OUT;
    digitalWrite(yp, 0);     //LCD9341_YP_PIN = 1;
    digitalWrite(xm, 0);    //LCD9341_XM_PIN = 1;
    return (4095 - (z2-z1));

}

//========================================================================
// ����: ͨ��2�β������õ���׼ȷ������ֵ.
// ����: x,y:�����õ�����������.
// ����: �����50���ڷ���1�����򷵻�0.
//========================================================================
#define ERR_RANGE 	50      //��Χ 
uint8_t tp_read_xy2(uint16_t *x,uint16_t *y,uint8_t xp,uint8_t xm,uint8_t yp,uint8_t ym) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;

    x1 = touch_read_raw_x(xp,xm,yp,ym);
    y1 = touch_read_raw_y(xp,xm,yp,ym);
    x2 = touch_read_raw_x(xp,xm,yp,ym);
    y2 = touch_read_raw_y(xp,xm,yp,ym);

    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ��2�β��������50����
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 

#endif



