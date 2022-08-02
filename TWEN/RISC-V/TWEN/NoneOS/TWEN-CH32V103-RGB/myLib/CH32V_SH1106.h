#ifndef _SH1106_h_
#define _SH1106_h_

#include <CH32V103.h>
#include "CH32V_SOFTIIC.h"
#include "CH32V_HARDIIC.h"
#include "CH32V_OLEDFONT.h"
#include "string.h"
#include "stdio.h"

class SH1106
{
private:	
	unsigned char disbuffer[128][8];
	uint8_t SH1106_ADDR=0x3c;
	KSoftIIC sh1106;
    uint8_t SDA_pin;
    uint8_t SCL_pin;
    uint8_t writec(uint8_t icommand);   //д������
	uint8_t writed(uint8_t idata);  //д������
public:
	SH1106(uint8_t sda_pin,uint8_t scl_pin);
	void begin(uint8_t _addr = 0x3c);  //��ʼ��
	void clear(void);   //����
	uint8_t display(void);  //������ʾ
	void set_pixel(unsigned char x, unsigned char y,uint8_t dian);  //����
	void draw_string(unsigned char x, unsigned char y,String str);  //��ʾ�ַ���
	void draw_num(unsigned char y,unsigned char x,float f); //��ʾ����
    void draw_hanzi_12(uint8_t zi, const uint8_t* hz, int x, int y);    //��ʾ12*12����
    void draw_hanzi_16(uint8_t zi, const uint8_t* hz, int x, int y);    //��ʾ16*16����
    void draw_hanzi_24(uint8_t zi, const uint8_t* hz, int x, int y);    //��ʾ24*24����
	void draw_hanzi_32(uint8_t zi, const uint8_t* hz, int x, int y);    //��ʾ32*32����
};

//========================================================================
// ����: ��������.
// ����: sda_pin:sda����; scl_pin��SCL����.
// ����: none.
//========================================================================
SH1106::SH1106(uint8_t sda_pin, uint8_t scl_pin)
{
    SDA_pin = sda_pin;
    SCL_pin = scl_pin;
}

//========================================================================
// ����: ��ʼ��.
// ����: none.
// ����: none.
//========================================================================
void SH1106::begin(uint8_t _addr)
{
    sh1106.begin(SDA_pin, SCL_pin);

    SH1106_ADDR = (_addr << 1);
    delay(300);//��ʼ��֮ǰ����ʱ����Ҫ��
    writec(0xae);//--turn off oled panel
    writec(0x00);//---set low column address
    writec(0x10);//---set high column address
    writec(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    writec(0x81);//--set contrast control register
    writec(0xFF); // Set SEG Output Current Brightness,CF
    writec(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    writec(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    writec(0xa6);//--set normal display
    writec(0xa8);//--set multiplex ratio(1 to 64)
    writec(0x3f);//--1/64 duty
    writec(0xd3);//-set display offset  Shift Mapping RAM Counter (0x00~0x3F)
    writec(0x00);//-not offset
    writec(0xd5);//--set display clock divide ratio/oscillator frequency
    writec(0x50);//--set divide ratio, Set Clock as 100 Frames/Sec-80
    writec(0xd9);//--set pre-charge period
    writec(0x22);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    writec(0xda);//--set com pins hardware configuration
    writec(0x12);
    writec(0xdb);//--set vcomh
    writec(0x40);//Set VCOM Deselect Level
    writec(0xa4);// Disable Entire Display On (0xa4/0xa5)
    writec(0xa6);// Disable Inverse Display On (0xa6/a7) 
    writec(0xaf);//--turn on oled panel
}

//========================================================================
// ����: д����.
// ����: none.
// ����: none.
//========================================================================
uint8_t SH1106::writec(uint8_t icommand)
{
    sh1106.start(SH1106_ADDR | KI2C_WRITE);

    sh1106.write(0x80);
    sh1106.write(icommand);

    sh1106.stop();
    return true;
}

//========================================================================
// ����: д����.
// ����: none.
// ����: none.
//========================================================================
uint8_t SH1106::writed(uint8_t idata)
{
    sh1106.start(SH1106_ADDR | KI2C_WRITE);

    sh1106.write(0x40);
    sh1106.write(idata);

    sh1106.stop();
    return true;
}

//========================================================================
// ����: ����.
// ����: none.
// ����: none.
//========================================================================
void SH1106::clear(void)
{
    unsigned char y, x;
    for (y = 0; y < 8; y++)
    {
        for (x = 0; x < 128; x++) {

            disbuffer[x][y] = 0x00;

        }
    }
}

//========================================================================
// ����: ������ʾ.
// ����: none.
// ����: none.
//========================================================================
uint8_t SH1106::display(void)
{
    unsigned char y, x;
    
    for (y = 0; y < 8; y++)
    {
    writec(0xb0+y);
    writec(0x02);
    writec(0x10);
 
    sh1106.start(SH1106_ADDR | KI2C_WRITE);
    sh1106.write(0x40);
        for (x = 0; x < 128; x++) {
            sh1106.write(disbuffer[x][y]);
        }
        sh1106.stop();
    }
    /*  for(y=0;y<8;y++)
     {
       writec(0xb0+y);
       writec(0x01);
       writec(0x10);
       for(x=0;x<128;x++){
         writed(disbuffer[x][y]);
        }
     } */

    return true;
}

//========================================================================
// ����: ����.
// ����: x,y:λ��; dian:������.
// ����: none.
//========================================================================
void SH1106::set_pixel(unsigned char x, unsigned char y, uint8_t dian)
{
    if (dian) {
        disbuffer[x][y / 8] |= (1 << (y % 8));
    }
    else {
        disbuffer[x][y / 8] &= ~(1 << (y % 8));
    }
}

//========================================================================
// ����: ��ʾ�ַ���.
// ����: x,y:λ��; str:Ҫ��ʾ���ַ���.
// ����: none.
//========================================================================
void SH1106::draw_string(unsigned char x, unsigned char y, String str)
{
    uint8_t i = 0, ci = 0, x1 = 0, y1 = y;
    char c;
    uint8_t j = str.length();
    if (y > 7)return;
    for (i = 0; i < j; i++) {
        c = str.charAt(i);
        for (ci = 0; ci < 5; ci++) {
            x1 = x + ci + i * 5;
            if (x1 > 122) { x1 = x1 - 122; y1 = y + 1; }
            disbuffer[x1][y1] = fontx[c][ci];
        }
    }
}

//========================================================================
// ����: ��ʾ����.
// ����: zi:����; hz��Ҫ��ʾ�ĺ���; x,y:λ��.
// ����: none.
//========================================================================
void SH1106::draw_num(unsigned char x, unsigned char y, float f)
{
    char Mem[15];
    char* buf = Mem;

    sprintf(buf, "%d", (int)f);
    String str = buf;
    draw_string(x, y, str);

}

//========================================================================
// ����: ��ʾ12*12����.
// ����: zi:����; hz��Ҫ��ʾ�ĺ���; x,y:λ��.
// ����: none.
//========================================================================
void SH1106::draw_hanzi_12(uint8_t zi, const uint8_t* hz, int x, int y)
{
    if (x < 0)
    {
        int zs = (0 - x) / 12;

        for (int zz = zs; zz < zi; zz++)
        {
            for (int i = 0; i < 12; i++)
            {
                for (int j = 0; j < 12; j++)
                {
                    if ((zz * 12 + j + x - 12) > 127)return;
                    if ((y + i - 12) > 63)return;
                    if (!((zz * 12 + x + j) < 0))
                        if ((hz[zz * 24 + i * 2 + j / 8]) & (0x80 >> (j % 8)))set_pixel(zz * 12 + x + j, y + i, 1);
                }
            }

        }

    }
    else
    {
        for (int zz = 0; zz < zi; zz++) {
            for (int i = 0; i < 12; i++)
            {
                for (int j = 0; j < 12; j++)
                {
                    if ((zz * 12 + j + x - 12) > 127)return;
                    if ((y + i - 12) > 63)return;
                    if ((hz[zz * 24 + i * 2 + j / 8]) & (0x80 >> (j % 8)))set_pixel(zz * 12 + x + j, y + i, 1);
                }
            }
        }
    }
}

//========================================================================
// ����: ��ʾ16*16����.
// ����: zi:����; hz��Ҫ��ʾ�ĺ���; x,y:λ��.
// ����: none.
//========================================================================
void SH1106::draw_hanzi_16(uint8_t zi, const uint8_t* hz, int x, int y)
{
    if (x < 0)
    {
        int zs = (0 - x) / 16;

        for (int zz = zs; zz < zi; zz++)
        {
            for (int i = 0; i < 16; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    if ((zz * 16 + j + x - 16) > 127)return;
                    if ((y + i - 16) > 63)return;
                    if (!((zz * 16 + x + j) < 0))
                        if ((hz[zz * 32 + i * 2 + j / 8]) & (0x80 >> (j % 8)))set_pixel(zz * 16 + x + j, y + i, 1);
                }
            }

        }
    }
    else {
        for (int zz = 0; zz < zi; zz++) {
            for (int i = 0; i < 16; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    if ((zz * 16 + j + x - 16) > 127)return;
                    if ((y + i - 16) > 63)return;
                    if ((hz[zz * 32 + i * 2 + j / 8]) & (0x80 >> (j % 8)))set_pixel(zz * 16 + x + j, y + i, 1);
                }
            }
        }
    }

}

//========================================================================
// ����: ��ʾ24*24����.
// ����: zi:����; hz��Ҫ��ʾ�ĺ���; x,y:λ��.
// ����: none.
//========================================================================
void SH1106::draw_hanzi_24(uint8_t zi, const uint8_t* hz, int x, int y)
{
    if (x < 0)
    {
        int zs = (0 - x) / 24;

        for (int zz = zs; zz < zi; zz++)
        {
            for (int i = 0; i < 24; i++)
            {
                for (int j = 0; j < 24; j++)
                {
                    if ((zz * 24 + j + x - 24) > 127)return;
                    if ((y + i - 24) > 63)return;
                    if (!((zz * 24 + x + j) < 0))
                        if ((hz[zz * 72 + i * 3 + j / 8]) & (0x80 >> (j % 8)))set_pixel(zz * 24 + x + j, y + i, 1);
                }
            }

        }

    }
    else {
        for (int zz = 0; zz < zi; zz++) {

            for (int i = 0; i < 24; i++)
            {
                for (int j = 0; j < 24; j++)
                {
                    if ((zz * 24 + j + x - 24) > 127)return;
                    if ((y + i - 24) > 63)return;
                    if ((hz[zz * 72 + i * 3 + j / 8]) & (0x80 >> (j % 8)))set_pixel(zz * 24 + x + j, y + i, 1);
                }
            }
        }
    }
}
//========================================================================
// ����: ��ʾ32*32����.
// ����: zi:����; hz��Ҫ��ʾ�ĺ���; x,y:λ��.
// ����: none.
//========================================================================
void SH1106::draw_hanzi_32(uint8_t zi, const uint8_t* hz, int x, int y)
{    
    uint16_t zs;
    uint8_t zz, i, j;
    if (x < 0)
    {
        zs = (0 - x) / 32;

        for (zz = zs; zz < zi; zz++)
        {
            for (i = 0; i < 32; i++)
            {
                for (j = 0; j < 32; j++)
                {
                    if ((zz * 32 + j + x - 32) > 127)return;
                    if ((y + i - 32) > 63)return;
                    if (!((zz * 32 + x + j) < 0))
                        if ((hz[zz * 128 + j * 4 + i / 8]) & (0x80 >> (i % 8)))set_pixel(zz * 32 + x + j, y + i, 1);
                }
            }
        }
    }
    else {
        for (zz = 0; zz < zi; zz++) {
            for (i = 0; i < 32; i++)
            {
                for (j = 0; j < 32; j++)
                {
                    if ((zz * 32 + j + x - 32) > 127)return;
                    if ((y + i - 32) > 63)return;
                    if ((hz[zz * 128 + j * 4 + i / 8]) & (0x80 >> (i % 8)))set_pixel(zz * 32 + x + j, y + i, 1);
                }
            }
        }
    }
}
#endif