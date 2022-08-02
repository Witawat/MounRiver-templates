#ifndef _CH32V_SSD1306_h_
#define _CH32V_SSD1306_h_

#include "CH32V_SOFTIIC.h"
#include "CH32V_HARDIIC.h"
#include "twen_oledfont.h"
#include "twen_gfx.h"
#include "string.h"
#include "stdio.h"

class SSD1306 : public TWEN_GFX
{
private:	
	unsigned char disbuffer[128][8];
	uint8_t SSD1306ADDR=0x3c;
	KSoftIIC ssd1306;
    uint8_t SDA_pin;
    uint8_t SCL_pin;
    int16_t _w;
    int16_t _h;
    uint8_t writec(uint8_t icommand);   //д������
	uint8_t writed(uint8_t idata);  //д������
public:
	SSD1306(int16_t w, int16_t h, uint8_t sda_pin,uint8_t scl_pin);
	void begin(uint8_t _addr=0x3c);  //��ʼ��
	void clear(uint8_t dian);   //����
	uint8_t display(void);  //��ʾ

    void draw_point(int16_t x, int16_t y,uint16_t dian);    //���㺯��
};

//========================================================================
// ����: ���캯��.
// ����: sda_pin��SDA����; scl_pin: scl����.
// ����: none.
//========================================================================
SSD1306::SSD1306(int16_t w, int16_t h, uint8_t sda_pin,uint8_t scl_pin) : TWEN_GFX(w, h)
{
    _w = w;
    _h = h;
    SDA_pin=sda_pin;
	SCL_pin=scl_pin;
}

//========================================================================
// ����: ��ʼ������.
// ����: _addr��iic��ַ��Ĭ��Ϊ0x3c��.
// ����: none.
//========================================================================
void SSD1306::begin(uint8_t _addr)
{
    ssd1306.begin(SDA_pin, SCL_pin);
    
    SSD1306ADDR = (_addr<<1);
    delay(300);//��ʼ��֮ǰ����ʱ����Ҫ��
    if( (_w == 128)&&(_h == 64) )
    {
        writec(0xAE);//--turn off oled panel
        writec(0x00);//---set low column address
        writec(0x10);//---set high column address
        writec(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
        writec(0x81);//--set contrast control register
        writec(0xCF); // Set SEG Output Current Brightness
        writec(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
        writec(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
        writec(0xa6);//--set normal display
        writec(0xa8);//--set multiplex ratio(1 to 64)
        writec(0x3f);//--1/64 duty
        writec(0xd3);//-set display offset  Shift Mapping RAM Counter (0x00~0x3F)
        writec(0x00);//-not offset
        writec(0xd5);//--set display clock divide ratio/oscillator frequency
        writec(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
        writec(0xd9);//--set pre-charge period
        writec(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
        writec(0xda);//--set com pins hardware configuration
        writec(0x12);
        writec(0xdb);//--set vcomh
        writec(0x40);//Set VCOM Deselect Level
        writec(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
        writec(0x00);//
        writec(0x8d);//--set Charge Pump enable/disable
        writec(0x14);//--set(0x10) disable
        writec(0xa4);// Disable Entire Display On (0xa4/0xa5)
        writec(0xa6);// Disable Inverse Display On (0xa6/a7) 
        writec(0xaf);//--turn on oled panel
    }else if( (_w == 128)&&(_h == 32) )
    {
        writec(0xAE);//�ر���ʾ	
        writec(0x40);//---set low column address
        writec(0xB0);//---set high column address
        writec(0xC8);//-not offset
        writec(0x81);//���öԱȶ�
        writec(0xff);
        writec(0xa1);//���ض�������
        writec(0xa6);//
        writec(0xa8);//��������·��
        writec(0x1f);
        writec(0xd3);
        writec(0x00);
        writec(0xd5);
        writec(0xf0);
        writec(0xd9);
        writec(0x22);
        writec(0xda);
        writec(0x02);
        writec(0xdb);
        writec(0x49);
        writec(0x8d);
        writec(0x14);
        writec(0xaf);//--turn on oled panel
    }else if( (_w == 72)&&(_h == 40) )
    {
        writec(0xAE); /*display off*/
        writec(0xD5); /*set osc division*/
        writec(0xF0);
        writec(0xA8); /*multiplex ratio*/
        writec(0x27); /*duty = 1/40*/
        writec(0xD3); /*set display offset*/
        writec(0x00);
        writec(0x40); /*Set Display Start Line */
        writec(0x8d); /*set charge pump enable*/
        writec(0x14);
        writec(0x20); /*Set page address mode*/
        writec(0x02);
        writec(0xA1); /*set segment remap*/
        writec(0xC8); /*Com scan direction*/
        writec(0xDA); /*set COM pins*/
        writec(0x12);
        writec(0xAD); /*Internal IREF Setting*/
        writec(0x30);
        writec(0x81); /*contract control*/
        writec(0xfF); /*128*/
        writec(0xD9); /*set pre-charge period*/
        writec(0x22);
        writec(0xdb); /*set vcomh*/
        writec(0x20);
        writec(0xA4); /*Set Entire Display On/Off*/
        writec(0xA6); /*normal / reverse*/
        writec(0x0C); /*set lower column address*/
        writec(0x11); /*set higher column address*/ 
    }else
    {
        // Other screen varieties
    }
    set_bg_mode(1);
}

//========================================================================
// ����: д������.
// ����: icommand������.
// ����: none.
//========================================================================
uint8_t SSD1306::writec(uint8_t icommand)
{
    ssd1306.start(SSD1306ADDR|KI2C_WRITE);
    ssd1306.write(0x80);
    ssd1306.write(icommand);
    ssd1306.stop();
    return true;
}

//========================================================================
// ����: д������.
// ����: idata������.
// ����: none.
//========================================================================
uint8_t SSD1306::writed(uint8_t idata)
{
    ssd1306.start(SSD1306ADDR|KI2C_WRITE);
    ssd1306.write(0x40);
    ssd1306.write(idata);
    ssd1306.stop();
    return true;
}

//========================================================================
// ����: ����.
// ����: none.
// ����: none.
//========================================================================
void SSD1306::clear(uint8_t dian)
{
	unsigned char x,y;
    if(dian){
        for(y=0;y<(_h/8);y++)
        {
            for(x=0;x<_w;x++){
                disbuffer[x][y]=0xff;
            }
        }
    }else{
        for(y=0;y<(_h/8);y++)
        {
            for(x=0;x<_w;x++){
                disbuffer[x][y]=0x00;
            }
        }
    }
}

//========================================================================
// ����: ������ʾ.
// ����: none.
// ����: none.
//========================================================================
uint8_t SSD1306::display(void)
{
	unsigned char y,x;
	writec(0x21);
	writec(0);
	writec(_w - 1);
	writec(0x22);
	writec(0);
	writec(_h/8 - 1); 
	ssd1306.start(SSD1306ADDR|KI2C_WRITE);
	ssd1306.write(0x40);    //д������
	for(y=0;y<(_h/8);y++)
	{
		for(x=0;x<_w;x++){
            ssd1306.write(disbuffer[x][y]);
		}
	}
	ssd1306.stop();
	return true;
}

//========================================================================
// ����: ����.
// ����: x,y:����; dian:1,����0��.
// ����: none.
//========================================================================
void SSD1306::draw_point(int16_t x, int16_t y,uint16_t dian)
{ 
    if ((x > (_w-1)) || (y > (_h-1))) {
        return;
    }
    else 
    {
        if (dian) {
            disbuffer[x][y / 8] |= (1 << (y % 8));
        }
        else {
            disbuffer[x][y / 8] &= ~(1 << (y % 8));
        }
    }
} 

#endif  //CH32V_SSD1306.h