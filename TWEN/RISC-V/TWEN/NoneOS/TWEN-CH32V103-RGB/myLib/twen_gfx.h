#ifndef _TWEN_GFX_H
#define _TWEN_GFX_H

#include "stdlib.h"
#include "Stream.h"
#include "Print.h"
#include "twen_lcdfont.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

class TWEN_GFX: public Print
{
public:
    TWEN_GFX(int16_t w, int16_t h); //��Ⱥ͸߶ȳ�ʼ��

    virtual void draw_point(int16_t x, int16_t y, uint16_t color){}; //���㺯������������������д
	virtual void start_write(){};	//��ʼд��
	virtual void end_write(){};	//����д��
	virtual void set_direction(uint8_t direction);	//������ʾ����

	void show_char(int16_t x,int16_t y, uint8_t chr, uint16_t font_color, uint16_t background_color, uint8_t size);//��ʾ�ַ�
	void set_text_size(uint8_t s)	//�����ı������С
	{
		textsize_x = s/2;
        textsize_y = s;
	}
	void set_cursor(int16_t x, int16_t y) //�����ı����λ��
	{
		cursor_x = x;
		cursor_y = y;
	}
    int16_t get_cursor_x(void) const {return cursor_x;} //��ȡ����X��λ��
    int16_t get_cursor_y(void) const {return cursor_y;} //��ȡ����X��λ��
	void set_text_color(uint16_t c, uint16_t bg) //�����ı�������ɫ���Զ��屳����ɫ
	{
		textcolor = c;
		textbgcolor = bg;
	}
    void set_bg_mode(uint8_t mode){_bg_mode = mode;}

	//�����ı��Ƿ񳬹���Ļ����Զ����е���һ�С�
	void set_text_wrap(bool w) { wrap = w; }
	using Print::write;
	virtual size_t write(uint8_t);

    void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);   //����
    void draw_Vline(int16_t x, int16_t y, int16_t h,uint16_t color); //����ֱ��
    void draw_Hline(int16_t x, int16_t y, int16_t w,uint16_t color); //��ˮƽ��

    void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);  //�����ο�
    void fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color);//������

    void draw_circle(int16_t x0, int16_t y0, int16_t r,uint16_t color); //��Բ
    void fill_circle(int16_t x0, int16_t y0, int16_t r,uint16_t color);//���Բ

    void draw_circle_helper(int16_t x0, int16_t y0, int16_t r,uint8_t cornername, uint16_t color); //����Э����Բ�Ǿ���
    void fill_circle_helper(int16_t x0, int16_t y0, int16_t r,uint8_t corners, int16_t delta,uint16_t color); //����Э�����Բ�Ǿ���   

    void draw_round_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t radius, uint16_t color);  //��Բ�Ǿ���
    void fill_round_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t radius, uint16_t color);  //���Բ�Ǿ���

    void draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color); //��������
    void fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color); //���������

	//������ʾ������ֻ����������blockʹ�ã�
    void draw_hanzi_12(uint8_t lenth, const uint8_t *hz);   //��ʾ12*12����
    void draw_hanzi_16(uint8_t lenth, const uint8_t *hz);   //��ʾ16*16����
    void draw_hanzi_24(uint8_t lenth, const uint8_t *hz);   //��ʾ24*24����
    void draw_hanzi_32(uint8_t lenth, const uint8_t *hz);   //��ʾ32*32����

private:
    int16_t WIDTH;  //�����Ŀ�͸�
    int16_t HEIGHT;
    int16_t _width;     
    int16_t _height;
    uint8_t rotation;   //������ʾ����
    bool wrap; //x���Զ�����
	int16_t cursor_x;	//���λ��
	int16_t cursor_y;
	uint8_t textsize_x;	//�ı�����x����
    uint8_t textsize_y;	//�ı�����y����
	uint16_t textcolor;	//������ɫ
	uint16_t textbgcolor; //����ı�����ɫ
    uint8_t _bg_mode;
};

//========================================================================
// ����: ��ʼ��.
// ����: none.
// ����: none.
//========================================================================
TWEN_GFX::TWEN_GFX(int16_t w, int16_t h) : WIDTH(w), HEIGHT(h) 
{
    _width = WIDTH;
    _height = HEIGHT;
    rotation = 0;
    cursor_y = cursor_x = 0;
    textsize_x = 6;
	textsize_y = 12;
	textcolor = 0xFFFF;
	textbgcolor = 0xF800;
    wrap = true;
    _bg_mode = 0;
}

//========================================================================
// ����: ������ʾ����.
// ����: direction:0~3;
// ����: none.
//========================================================================
void TWEN_GFX::set_direction(uint8_t direction)
{
    rotation = (direction & 3);
    switch (rotation) {
    case 0:
    case 2:
        _width = WIDTH;
        _height = HEIGHT;
        break;
    case 1:
    case 3:
        _width = HEIGHT;
        _height = WIDTH;
        break;
    }
}

//========================================================================
// ����: ��ָ��λ����ʾһ���ַ�.
// ����: x:��ʼx����; y:��ʼy����; chr:��ʾ���ַ�; font_color:��ʾ�ַ�����ɫֵ; 
//       background_color:��ʾ�ַ��ı���ɫ; size:��ʾ�ַ��Ĵ�С(12/16);	  
// ����: none.
//========================================================================
void TWEN_GFX::show_char(int16_t x,int16_t y, uint8_t chr, uint16_t font_color, uint16_t background_color, uint8_t size)
{  
	uint8_t temp;
	uint8_t j,i;
	chr=chr-' ';						  //�õ�ƫ�ƺ��ֵ
	if(x < 0)
	{
		for(j=0;j<size;j++)
		{
			if(size==12)temp=asc2_1206[chr][j]; //����1206����
			else temp=asc2_1608[chr][j];		//����1608���� 
			for(i=0;i<size/2;i++)
		    {   
				if(((x + i) >= 0))
				{   
					if((x+i-size/2) > _width)return;
					if((y+size) > _height)return;
					if(!_bg_mode) //�ǵ��ӷ�ʽ
					{
						if(temp&0x01){
							draw_point(x+i,y+j,font_color);
						}
						else{
							draw_point(x+i,y+j,background_color);
						}
					}
					else
					{            
						if(temp&0x01){
							draw_point(x+i,y+j,font_color);	
						}
					}
				}
				temp>>=1; 
		    }
		}	
	}
	else{
		for(j=0;j<size;j++)
		{
			if(size==12)temp=asc2_1206[chr][j]; //����1206����
			else temp=asc2_1608[chr][j];		//����1608���� 
			for(i=0;i<size/2;i++)
		    {      
				if((x+i-size/2) > _width)return;
				if((y+size) > _height)return;
				if(!_bg_mode) //�ǵ��ӷ�ʽ
				{
					if(temp&0x01){
						draw_point(x+i,y+j,font_color);	
					}
					else{
						draw_point(x+i,y+j,background_color);	 
					}
				}
				else
				{            
					if(temp&0x01){
						draw_point(x+i,y+j,font_color);	
					}
				}
				temp>>=1; 
		    }
		}	
	}   	 	  
}

//========================================================================
// ����: ��дprint��write����.
// ����: none.
// ����: none.
//========================================================================
size_t TWEN_GFX::write(uint8_t c) 
{
	if(c == '\n'){
		cursor_x = 0;  //x��������
		cursor_y += textsize_y; //y��������һ�����峤��
	}else if(c != '\r'){
    	if (wrap && ((cursor_x + textsize_x) > _width)) //������峬���ұ߱߿�
		{ 
        	cursor_x = 0;     //x��������
        	cursor_y += textsize_y; //y��������һ������߶�
    	}
		show_char(cursor_x,cursor_y, c, textcolor, textbgcolor, textsize_y);
		cursor_x += textsize_x; //x��������һ��������
	}
	return 1;
}

//========================================================================
// ����: LCD�����߶�.
// ����: x1��y1:��ʼ����; x2,y2:��ֹ����;color:��ɫ.
// ����: none.
//========================================================================
void TWEN_GFX::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	int t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x1 - x0; //������������ 
	delta_y = y1 - y0;
	uRow = x0;
	uCol = y0;
	if (delta_x > 0)incx = 1; //���õ������� 
	else if (delta_x == 0)incx = 0;//��ֱ�� 
	else { incx = -1; delta_x = -delta_x; }
	if (delta_y > 0)incy = 1;
	else if (delta_y == 0)incy = 0;//ˮƽ�� 
	else { incy = -1; delta_y = -delta_y; }
	if (delta_x > delta_y)distance = delta_x; //ѡȡ�������������� 
	else distance = delta_y;
	for (t = 0; t <= distance + 1; t++)//������� 
	{
		draw_point(uRow, uCol, color);//���� 
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

//========================================================================
// ����: ����ֱ��.
// ����: x,y:��ʼ����; h:����.
// ����: none.
//========================================================================
void TWEN_GFX::draw_Vline(int16_t x, int16_t y, int16_t h,uint16_t color) 
{
    draw_line(x, y, x, y+h-1, color);
}

//========================================================================
// ����: ��ˮƽ��.
// ����: x,y:��ʼ����; w:����.
// ����: none.
//========================================================================
void TWEN_GFX::draw_Hline(int16_t x, int16_t y, int16_t w,uint16_t color)
{
    draw_line(x, y, x + w - 1, y, color);
}

//========================================================================
// ����: LCD���ƾ��ο�.
// ����: x0��y0:��ʼ����; x1,y1:��ֹ����; color:��ɫ;
// ����: none.
//========================================================================
void TWEN_GFX::draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	draw_line(x0, y0, x1, y0,color);
	draw_line(x0, y0, x0, y1,color);
	draw_line(x0, y1, x1, y1,color);
	draw_line(x1, y0, x1, y1,color);
}

//========================================================================
// ����: ��ָ��λ��������.
// ����: x0��y0:��ʼ����; x1,y1:��ֹ����; color:��ɫ;
// ����: none.
//========================================================================
void TWEN_GFX::fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color)
{
    for (int16_t i = x0; i < x1; i++) {
        draw_Vline(i, y0, abs(y1 - y0), color); //����ֱ��
    }
}

//========================================================================
// ����: Э����Բ����.
// ����: none.
// ����: none.
//========================================================================
void TWEN_GFX::draw_circle_helper(int16_t x0, int16_t y0, int16_t r,uint8_t cornername, uint16_t color) 
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            draw_point(x0 + x, y0 + y, color);
            draw_point(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            draw_point(x0 + x, y0 - y, color);
            draw_point(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            draw_point(x0 - y, y0 + x, color);
            draw_point(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            draw_point(x0 - y, y0 - x, color);
            draw_point(x0 - x, y0 - y, color);
        }
    }
}

//========================================================================
// ����: Э�����Բ����.
// ����: none.
// ����: none.
//========================================================================
void TWEN_GFX::fill_circle_helper(int16_t x0, int16_t y0, int16_t r,uint8_t corners, int16_t delta,uint16_t color) 
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;

    delta++; // Avoid some +1's in the loop

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (x < (y + 1)) {
            if (corners & 1)
                draw_Vline(x0 + x, y0 - y, 2 * y + delta, color);
            if (corners & 2)
                draw_Vline(x0 - x, y0 - y, 2 * y + delta, color);
        }
        if (y != py) {
            if (corners & 1)
                draw_Vline(x0 + py, y0 - px, 2 * px + delta, color);
            if (corners & 2)
                draw_Vline(x0 - py, y0 - px, 2 * px + delta, color);
            py = y;
        }
        px = x;
    }
}


//========================================================================
// ����: ��Բ.
// ����: none.
// ����: none.
//========================================================================
void TWEN_GFX::draw_circle(int16_t x0, int16_t y0, int16_t r,uint16_t color) 
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    draw_point(x0, y0 + r, color);
    draw_point(x0, y0 - r, color);
    draw_point(x0 + r, y0, color);
    draw_point(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        draw_point(x0 + x, y0 + y, color);
        draw_point(x0 - x, y0 + y, color);
        draw_point(x0 + x, y0 - y, color);
        draw_point(x0 - x, y0 - y, color);
        draw_point(x0 + y, y0 + x, color);
        draw_point(x0 - y, y0 + x, color);
        draw_point(x0 + y, y0 - x, color);
        draw_point(x0 - y, y0 - x, color);
    }
}

//========================================================================
// ����: ���Բ.
// ����: none.
// ����: none.
//========================================================================
void TWEN_GFX::fill_circle(int16_t x0, int16_t y0, int16_t r,uint16_t color) 
{
  draw_Vline(x0, y0 - r, 2 * r + 1, color);
  fill_circle_helper(x0, y0, r, 3, 0, color);
}

//========================================================================
// ����: ��Բ�Ǿ���.
// ����: none.
// ����: none.
//========================================================================
void TWEN_GFX::draw_round_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t radius, uint16_t color)
{
    int16_t w = abs(x1-x0);
    int16_t h = abs(y1-y0);
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (radius > max_radius)
        radius = max_radius;
    // smarter version
    draw_Hline(x0 + radius, y0, w - 2 * radius, color);         // Top
    draw_Hline(x0 + radius, y0 + h - 1, w - 2 * radius, color); // Bottom
    draw_Vline(x0, y0 + radius, h - 2 * radius, color);         // Left
    draw_Vline(x0 + w - 1, y0 + radius, h - 2 * radius, color); // Right
    // draw four corners
    draw_circle_helper(x0 + radius, y0 + radius, radius, 1, color);
    draw_circle_helper(x0 + w - radius - 1, y0 + radius, radius, 2, color);
    draw_circle_helper(x0 + w - radius - 1, y0 + h - radius - 1, radius, 4, color);
    draw_circle_helper(x0 + radius, y0 + h - radius - 1, radius, 8, color);
}

//========================================================================
// ����: ���Բ�Ǿ���.
// ����: none.
// ����: none.
//========================================================================
void TWEN_GFX::fill_round_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t radius, uint16_t color)
{
    int16_t w = abs(x1-x0);
    int16_t h = abs(y1-y0);
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (radius > max_radius)
        radius = max_radius;
    // smarter version
    fill_rectangle(x0 + radius, y0, x1 - radius, y1, color);

    // draw four corners
    fill_circle_helper(x0 + w - radius - 1, y0 + radius, radius, 1, h - 2 * radius - 1, color);
    fill_circle_helper(x0 + radius, y0 + radius, radius, 2, h - 2 * radius - 1, color);
}

//========================================================================
// ����: ��������.
// ����: none.
// ����: none.
//========================================================================
void TWEN_GFX::draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
  draw_line(x0, y0, x1, y1, color);
  draw_line(x1, y1, x2, y2, color);
  draw_line(x2, y2, x0, y0, color);
}

//========================================================================
// ����: ���������.
// ����: none.
// ����: none.
//========================================================================
void TWEN_GFX::fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1);
        _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }

    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)
            a = x1;
        else if (x1 > b)
            b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        draw_Hline(a, y0, b - a + 1, color);
        return;
    }

    int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
            dx12 = x2 - x1, dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2)
        last = y1; // Include y1 scanline
    else
        last = y1 - 1; // Skip it

    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            _swap_int16_t(a, b);
        draw_Hline(a, y, b - a + 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            _swap_int16_t(a, b);
        draw_Hline(a, y, b - a + 1, color);
    }
}

//========================================================================
// ����: ��ָ��λ����ʾ12*12���庺��.
// ����: hz:���ֵ�ָ��; lenth: ������ܳ���.
// ����: none.
//========================================================================
void TWEN_GFX::draw_hanzi_12(uint8_t lenth, const uint8_t *hz)
{
	uint8_t zz,i,j;
	int16_t zs;
	if(cursor_x < 0)
	{
		zs = (0-cursor_x) / 12;	
		for(zz = zs; zz < lenth; zz++)		//Ҫ��ʾ������ѭ��
		{
            if((wrap) && ((cursor_x + 12) > _width))
            {
                    cursor_x = 0;     //x��������
                    cursor_y += 12;                
            }
			for(i = 0;i < 12;i++)
			{
				for(j = 0;j < 12;j++)
				{
					if((cursor_x + j -12) > _width)return;
					if((cursor_y + i -12) > _height)return;
					if (!((cursor_x + j) < 0)){
						if(!_bg_mode)		//�޵���
						{
							if(hz[zz*24 + i * 2 + j/8] & (0x80 >> (j % 8))){
                                draw_point(cursor_x + j,cursor_y + i,textcolor);
							}
							else{
                                draw_point(cursor_x + j,cursor_y + i,textbgcolor);
							} 
						}
						else
						{
							if(hz[zz*24 + i * 2 + j/8] & (0x80>>(j % 8))){
                                draw_point(cursor_x + j,cursor_y + i,textcolor);
							}
						}
					}
				}
			}
            cursor_x += 12;
		}
	}
	else
	{
		for(zz = 0; zz < lenth; zz++)		//Ҫ��ʾ������ѭ��
		{
			if((wrap) && ((cursor_x + 12) > _width))
            {
                    cursor_x = 0;     //x��������
                    cursor_y += 12;                
            }
			for(i = 0;i < 12;i++)
			{
				for(j = 0;j < 12;j++)
				{
					if((cursor_x + j -12) > _width)return;                
					if((cursor_y + i -12) > _height)return;
					if(!_bg_mode)		//�޵���
					{
						if(hz[zz*24 + i * 2 + j/8] & (0x80 >> (j % 8))){
                            draw_point(cursor_x + j,cursor_y + i,textcolor);
						}
						else{
                            draw_point(cursor_x + j,cursor_y + i,textbgcolor);
						}
					}
					else
					{
						if(hz[zz*24 + i * 2 + j/8] & (0x80>>(j % 8))){
							draw_point(cursor_x + j,cursor_y + i,textcolor);					
						}
					}
				}
			}
            cursor_x+=12;
		}
	}
}

//========================================================================
// ����: ��ָ��λ����ʾ16*16���庺��.
// ����: hz:���ֵ�ָ��; lenth: ������ܳ���.
// ����: none.
//========================================================================
void TWEN_GFX::draw_hanzi_16(uint8_t lenth, const uint8_t *hz)
{
	uint8_t zz,i,j;
	int16_t zs;
	if(cursor_x < 0)
	{
		zs = (0-cursor_x) / 16;	
		for(zz = zs; zz < lenth; zz++)		//Ҫ��ʾ������ѭ��
		{
            if((wrap) && ((cursor_x + 16) > _width))
            {
                    cursor_x = 0;     //x��������
                    cursor_y += 16;                
            }
			for(i = 0;i < 16;i++)
			{
				for(j = 0;j < 16;j++)
				{
					if((j + cursor_x - 16) > _width)return;
					if((cursor_y + i -16) > _height)return;
					if (!((cursor_x + j) < 0)){
						if(!_bg_mode)		//�޵���
						{
							if(hz[zz*32+i * 2 + j/8] & (0x80 >> (j % 8))){
                                draw_point(cursor_x + j,cursor_y + i,textcolor);	
							}
							else{
                                draw_point(cursor_x + j,cursor_y + i,textbgcolor);	
							}
						}
						else
						{
							if(hz[zz*32 + i * 2 + j/8] & (0x80>>(j % 8))){
								draw_point(cursor_x + j,cursor_y + i,textcolor);							
							}
						}
					}
				}
			}
            cursor_x += 16;
		}
	}
	else
	{
		for(zz = 0; zz < lenth; zz++)		//Ҫ��ʾ������ѭ��
		{
            if((wrap) && ((cursor_x + 16) > _width))
            {
                    cursor_x = 0;     //x��������
                    cursor_y += 16;                
            }
			for(i = 0;i < 16;i++)
			{
				for(j = 0;j < 16;j++)
				{
					if((j + cursor_x -16) > _width)return;
					if((cursor_y + i -16) > _height)return;

					if(!_bg_mode)		//�޵���
					{
						if(hz[zz*32+i * 2 + j/8] & (0x80 >> (j % 8))){
							draw_point(cursor_x + j,cursor_y + i,textcolor);	
						}
						else{
							draw_point(cursor_x + j,cursor_y + i,textbgcolor);
						}
					}
					else
					{
						if(hz[zz*32 + i * 2 + j/8] & (0x80>>(j % 8))){
							draw_point(cursor_x + j,cursor_y + i,textcolor);	
						}
					}
				}
			}
            cursor_x += 16;
		}
	}
}
//========================================================================
// ����: ��ָ��λ����ʾ24*24���庺��.
// ����: hz:���ֵ�ָ��; lenth: ������ܳ���.
// ����: none.
//========================================================================
void TWEN_GFX::draw_hanzi_24(uint8_t lenth, const uint8_t *hz)
{
	uint8_t zz,i,j;
	int16_t zs;
	if(cursor_x < 0)
	{
		zs = (0-cursor_x) / 24;	
		for(zz = zs; zz < lenth; zz++)		//Ҫ��ʾ������ѭ��
		{
            if((wrap) && ((cursor_x + 24) > _width))
            {
                    cursor_x = 0;     //x��������
                    cursor_y += 24;                
            }            
			for(i = 0;i < 24;i++)
			{
				for(j = 0;j < 24;j++)
				{
					if((j + cursor_x -24) > _width)return;
					if((cursor_y + i -24) > _height)return;
					if (!((cursor_x + j) < 0))
					{
						if(!_bg_mode)		//�޵���
						{
							if(hz[zz*72+i * 3 + j/8] & (0x80 >> (j % 8))){
                                draw_point(cursor_x + j,cursor_y + i,textcolor);
							}
							else {
                                draw_point(cursor_x + j,cursor_y + i,textbgcolor);
							}
						}
						else
						{
							if(hz[zz*72 + i * 3 + j/8] & (0x80>>(j % 8))){
								draw_point(cursor_x + j,cursor_y + i,textcolor);
							}
						}
					}
				}
			}
            cursor_x+=24;
		}
	}
	else
	{
		for(zz = 0; zz < lenth; zz++)		//Ҫ��ʾ������ѭ��
		{
            if((wrap) && ((cursor_x + 24) > _width))
            {
                    cursor_x = 0;     //x��������
                    cursor_y += 24;                
            }  
			for(i = 0;i < 24;i++)
			{
				for(j = 0;j < 24;j++)
				{
					if((j + cursor_x -24) > _width)return;
					if((cursor_y + i -24) > _height)return;

					if(!_bg_mode)		//�޵���
					{
						if(hz[zz*72+i * 3 + j/8] & (0x80 >> (j % 8))){
							draw_point(cursor_x + j,cursor_y + i,textcolor);
						}
						else{
							draw_point(cursor_x + j,cursor_y + i,textbgcolor);
						}
					}
					else
					{
						if(hz[zz*72 + i * 3 + j/8] & (0x80>>(j % 8))){
							draw_point(cursor_x + j,cursor_y + i,textcolor);
						}
					}
				}
			}
            cursor_x+=24;
		}
	}
}

//========================================================================
// ����: ��ָ��λ����ʾ32*32���庺��.
// ����: hz:���ֵ�ָ��; lenth:������ܳ���.
// ����: none.
//========================================================================
void TWEN_GFX::draw_hanzi_32(uint8_t lenth, const uint8_t *hz)
{
	uint8_t zz,i,j;
	int16_t zs;
	if(cursor_x < 0)
	{
		zs = (0-cursor_x) / 32;	
		for(zz = zs; zz < lenth; zz++)		//Ҫ��ʾ������ѭ��
		{
            if((wrap) && ((cursor_x + 32) > _width))
            {
                    cursor_x = 0;     //x��������
                    cursor_y += 32;                
            }
			for(i = 0;i < 32;i++)
			{
				for(j = 0;j < 32;j++)
				{
					if((j + cursor_x -34) > _width)return;
					if((cursor_y + i -34) > _height)return;
					if (!((cursor_x + j) < 0)){
						if(!_bg_mode)		//�޵���
						{
							if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
                                draw_point(cursor_x + j,cursor_y + i,textcolor);
							}
							else{ 
								draw_point(cursor_x + j,cursor_y + i,textbgcolor);
							}
						}
						else
						{
							if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
								draw_point(cursor_x + j,cursor_y + i,textcolor);
							}
						}
					}
				}
			}
            cursor_x+=32;
		}
	}
	else
	{
		for(zz = 0; zz < lenth; zz++)		//Ҫ��ʾ������ѭ��
		{
            if((wrap) && ((cursor_x + 32) > _width))
            {
                    cursor_x = 0;     //x��������
                    cursor_y += 32;                
            }
			for(i = 0;i < 32;i++)
			{
				for(j = 0;j < 32;j++)
				{
					if((j + cursor_x -32) > _width)return;
					if((cursor_y + i -32) > _height)return;

					if(!_bg_mode)		//�޵���
					{
						if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
							draw_point(cursor_x + j,cursor_y + i,textcolor);
						}
						else{
							draw_point(cursor_x + j,cursor_y + i,textbgcolor);
						}
					}
					else
					{
						if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
							draw_point(cursor_x + j,cursor_y + i,textcolor);
						}
					}
				}
			}
            cursor_x+=32;
		}
	}
}

#endif