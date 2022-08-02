/**********************************************************
 * �ÿ�ΪCH32V103��16λ����������(HR2046).
 * *******************************************************/
#ifndef _CH32V_SPITOUCH_H
#define _CH32V_SPITOUCH_H

#include <CH32V103.h>
#include "CH32V_HARDSPI.h"

#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 
#define CT_MAX_TOUCH  5    //������֧�ֵĵ���,�̶�Ϊ5��

#define READ_TIMES 5 	//��ȡ����
#define LOST_VAL 1	  	//����ֵ

class TOUCHPOINT
{
public:
    TOUCHPOINT(uint8_t tcs_pin, uint8_t pen_pin);
    void begin();
    uint16_t read_xoy(uint8_t xy);
    uint8_t read_xy(uint16_t *x,uint16_t *y);
    uint8_t read_xy2(uint16_t *x,uint16_t *y);
    uint8_t is_pressed();
	
private:
    uint8_t _tcs_pin;
    uint8_t _pen_pin;
	void write_byte(uint8_t num);
	uint16_t read_ad(uint8_t CMD);
	//Ĭ��Ϊtouchtype=0������.
    uint8_t CMD_RDX=0XD0;
    uint8_t CMD_RDY=0X90;
};

TOUCHPOINT::TOUCHPOINT(uint8_t tcs_pin, uint8_t pen_pin)
{
    _tcs_pin = tcs_pin;
    _pen_pin = pen_pin;
}

//========================================================================
// ����: ��ʼ��.
// ����: none.
// ����: none.
//========================================================================
void TOUCHPOINT::begin()
{
    pinMode(_pen_pin, GPIO_Mode_IPU);
    pinMode(_tcs_pin, GPIO_Mode_Out_PP);
    hardspi.begin();
}

//========================================================================
// ����: �Ƿ��а�������.
// ����: none.
// ����: 0:�а�������; 1:û�а�������.
//========================================================================
uint8_t TOUCHPOINT::is_pressed() 
{ 
    if(digitalRead(_pen_pin) == 0){
        return 1;
    }else{
        return 0;
    }
}

//========================================================================
// ����: ������ICд����.
// ����: д�������.
// ����: none.
//========================================================================
void TOUCHPOINT::write_byte(uint8_t num)    
{  
	hardspi.wr_data(num);
} 

//========================================================================
// ����: �Ӵ�����IC������.
// ����: ָ��.
// ����: ����������.
//========================================================================
uint16_t TOUCHPOINT::read_ad(uint8_t CMD)	  
{ 	 
	uint16_t Num=0; 
	hardspi.set_speed(3);
	digitalWrite(_tcs_pin, 0); 		//ѡ�д�����IC
	write_byte(CMD);   //����������
	delayMicroseconds(6);   //ADS7846��ת��ʱ���Ϊ6us

    Num = hardspi.wr_data(0) << 8;
	Num = Num | hardspi.wr_data(0);
    Num >>= 3;  //ֻ�и�12λ��Ч 
    digitalWrite(_tcs_pin, 1); 		////�ͷ�Ƭѡ 
	hardspi.set_speed(1);
	return(Num);   
}

//========================================================================
// ����: ��ȡһ������ֵ(x����y).
// ����: ָ�CMD_RDX/CMD_RDY��.
// ����: ����������.
//========================================================================
uint16_t TOUCHPOINT::read_xoy(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=read_ad(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
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
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

//========================================================================
// ����: ��ȡx,y����.
// ����: x,y:��ȡ��������ֵ.
// ����: 0,ʧ��;1,�ɹ�.
//========================================================================
uint8_t TOUCHPOINT::read_xy(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp = read_xoy(CMD_RDX);
	ytemp = read_xoy(CMD_RDY);	  												   
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}

//========================================================================
// ����: ����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���ERR_RANGE.
// ����: x,y:��ȡ��������ֵ.
// ����: 0,ʧ��;1,�ɹ�.
//========================================================================
#define ERR_RANGE 50 //��Χ 
uint8_t TOUCHPOINT::read_xy2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag = read_xy(&x1,&y1);   
    if(flag == 0)return(0);
    flag = read_xy(&x2,&y2);	   
    if(flag == 0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  

#endif
















