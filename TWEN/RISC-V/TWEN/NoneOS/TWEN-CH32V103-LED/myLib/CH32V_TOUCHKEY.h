/**********************************************************
 * �ÿ�ΪCH32V103�Ĵ�����.
 * *******************************************************/
#ifndef _CH32V_TOUCHKEY_H
#define _CH32V_TOUCHKEY_H

#include "CH32V103.h"

class TOUCHKEY{
public:
    TOUCHKEY(ADC_Name adcn);
    void begin();
    uint16_t value();

private:
    ADC_Name _adcn;
};

//========================================================================
// ����: ���캯��.
// ����: none
// ����: none.
//========================================================================
TOUCHKEY::TOUCHKEY(ADC_Name adcn)
{
    _adcn = adcn;
}

//========================================================================
// ����: ��ʼ��.
// ����: none
// ����: none.
//========================================================================
void TOUCHKEY::begin()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    if (_adcn < 8)
    {
        pinMode(_adcn, GPIO_Mode_AIN);
    }
    else if (_adcn < 10)
    {
        pinMode(_adcn + 8, GPIO_Mode_AIN);
    }
    else if (_adcn < 15)
    {
        pinMode(_adcn + 14, GPIO_Mode_AIN);
    }

    ADC_Cmd(ADC1, ENABLE);
    
	ADC1->CTLR1 |= 0x51000000;   //����TKEY_V��Ԫ
}

//========================================================================
// ����: ��ȡ����ֵ.
// ����: ADC_Channel_0 ~ ADC_Channel_15
// ����: none.
//========================================================================
uint16_t TOUCHKEY::value()
{
    uint16_t val;
    ADC1->RSQR3 = _adcn;
    while(!(ADC1->CTLR1 & 0x08000000)); //�ȴ�ת�����
    val = (uint16_t)ADC1->RDATAR;
    return val;
}

#endif





