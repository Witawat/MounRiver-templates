/**
 * @file CH32V_WS2812.h
 * @brief RGB������ģ��������ͷ�ļ�
 * @version 0.1
 * @copyright Copyright (c) 2021 TWen Technology Co., Ltd.
 * 
 */

#ifndef _CH32V_WS2812_H_
#define _CH32V_WS2812_H_

#include <CH32V103.h>

typedef union {
	struct
	{
		int g;
		int r;
		int b;
	}simple;
	int grb[3];
}color_t;

typedef struct _rgb_strip_t{
 	int pin_num;
 	int num_leds;
 	color_t *leds;
}rgb_strip_t;


class WS2812
{
public:
	WS2812( uint8_t pin);
	void pixel_init(uint16_t num_leds);	//RGB��ʼ��
	uint8_t pixel_set_color(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);	//RGB������ɫ
	uint8_t pixel_set_color(uint16_t index, uint32_t c);	//RGB������ɫ
    void pixel_set_all_color(uint8_t red, uint8_t green, uint8_t blue);	//��������RGB��ɫ
	void pixel_show();	//RGB������ʾ
	uint8_t pixel_set_color_and_show(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);	//RGB������ɫ����ʾ
	void pixel_clear();	//RGB���
	void rainbow(uint8_t wait);//�ʺ�ѭ������Ч��
	void rainbowCycle(uint8_t wait);//�������Ч��
	void setBrightness(uint8_t b);//��������ֵ

	    void sendOne(uint8_t GPIO_Pin);
	void sendZero(uint8_t GPIO_Pin);
private:

	void sendRGB(uint8_t pin,uint8_t red,uint8_t green,uint8_t blue);
	void rstRGB(uint8_t pin);
	uint32_t color(uint8_t r, uint8_t g, uint8_t b) {
   		return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
 	}
	uint32_t wheel(uint8_t WheelPos);
	rgb_strip_t strip;
	uint8_t _pin;
	uint8_t _brightness;
};

//========================================================================
// ����: ���ų�ʼ��.
// ����: pin:����.
// ����: none.
//========================================================================
WS2812::WS2812(uint8_t pin)
{
	_pin = pin;
	setBrightness(50);
}

//========================================================================
// ����: RGB����1��.
// ����: GPIOx���˿�; GPIO_Pin�����ź�.
// ����: none.
//========================================================================
void WS2812::sendOne(uint8_t GPIO_Pin)
{
	//�ߵ�ƽ850ns
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (GPIO_Pin/16)))->BSHR = 1<<(GPIO_Pin%16);
	__asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
		             nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
		             nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
		             nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
		             nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
		             nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
		             nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
					 nop\nnop\n						   		   \
		           ");
	//�͵�ƽ400ns
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (GPIO_Pin/16)))->BCR = 1<<(GPIO_Pin%16);
	__asm volatile ("nop\n");
}

//========================================================================
// ����: RGB����0��.
// ����: GPIOx���˿�; GPIO_Pin�����ź�.
// ����: none.
//========================================================================
void WS2812::sendZero(uint8_t GPIO_Pin)
{
	//�ߵ�ƽ400ns
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (GPIO_Pin/16)))->BSHR = 1<<(GPIO_Pin%16);
	__asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
		             nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
		             nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
					 nop\nnop\n						   		   \
		           ");

	//�͵�ƽ850ns
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (GPIO_Pin/16)))->BCR = 1<<(GPIO_Pin%16);
	__asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
		             nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
					 nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n  \
					 nop\nnop\nnop\n					   	   \
		           ");
}

//========================================================================
// ����: RGB��������.
// ����: pin��RGB����;  red,green,blue:RGB��ɫֵ.
// ����: none.
//========================================================================
void WS2812::sendRGB(uint8_t pin,uint8_t red,uint8_t green,uint8_t blue)
{
	for(uint8_t i=0;i<8;i++)
	{
		if(green&0x80){
			sendOne(pin);
		}else{
			sendZero(pin);
		}
		 green<<=1;
	}

	for(uint8_t i=0;i<8;i++)
	{
		if(red&0x80){
			sendOne(pin);
		}else{
			sendZero(pin);
		}
		 red<<=1;
	}
	for(uint8_t i=0;i<8;i++)
	{
		if(blue&0x80){
			sendOne(pin);
		}else{
			sendZero(pin);
		}
		 blue<<=1;
	}
}

//========================================================================
// ����: RGB��������.
// ����: pin��RGB����.
// ����: none.
//========================================================================
void WS2812::rstRGB(uint8_t pin)
{
    pinMode(_pin, GPIO_Mode_Out_PP);
    digitalWrite(_pin,0);
}

//========================================================================
// ����: RGB��ʼ��.
// ����: num_leds��RGB����.
// ����: none.
//========================================================================
void WS2812::pixel_init(uint16_t num_leds)
{	
	strip.leds = (color_t*) malloc(sizeof(color_t) * num_leds);
	strip.pin_num = _pin;
	strip.num_leds = num_leds;

	rstRGB(strip.pin_num); 

	for (int i = 0; i < num_leds; i++)
	{	
		strip.leds[i].simple.g = 0;
		strip.leds[i].simple.r = 0;
		strip.leds[i].simple.b = 0;
	}
}

//========================================================================
// ����: RGB��ʾ.
// ����: none.
// ����: none.
//========================================================================
void WS2812::pixel_show()
{
	
	rstRGB(strip.pin_num); 
	delay(1);
	   //�����ٽ���
	for (int i = 0; i < strip.num_leds; i++)
	{
		sendRGB(strip.pin_num,strip.leds[i].simple.r,strip.leds[i].simple.g,strip.leds[i].simple.b);
	}
	   //�˳��ٽ���
}

//========================================================================
// ����: RGBд����ɫֵ.
// ����: index:�ڼ���RGB; red,green,blue:RGB��ɫֵ.
// ����: �ɹ� 1��ʧ�� 0.
//========================================================================
uint8_t WS2812::pixel_set_color(uint16_t index, uint8_t red, uint8_t green, uint8_t blue)
{
	if(_brightness) 
	{ 
		red = (red * _brightness) >> 8;
		green = (green * _brightness) >> 8;
		blue = (blue * _brightness) >> 8;
	}

	if (index < strip.num_leds)
	{
		strip.leds[index].simple.r = red;
		strip.leds[index].simple.g = green;
		strip.leds[index].simple.b = blue;
	}
	else{
		return 1;
	}
	return 0;
}

//========================================================================
// ����: RGBд����ɫֵ.
// ����: index:�ڼ���RGB; 32λWRGBֵ.
// ����: �ɹ� 1��ʧ�� 0.
//========================================================================
uint8_t WS2812::pixel_set_color(uint16_t index, uint32_t c)
{
	uint8_t r = (uint8_t)(c >> 16);
  	uint8_t g = (uint8_t)(c >>  8);
  	uint8_t b = (uint8_t)c;

	if(_brightness) 
	{ 
		r = (r * _brightness) >> 8;
		g = (g * _brightness) >> 8;
		b = (b * _brightness) >> 8;
	}

	if (index < strip.num_leds)
	{
		strip.leds[index].simple.r = r;
		strip.leds[index].simple.g = g;
		strip.leds[index].simple.b = b;
	}
	else{
		return 1;
	}
	return 0;
}

//========================================================================
// ����: ȫ��RGBд����ɫֵ.
// ����: red,green,blue:RGB��ɫֵ.
// ����: none.
//========================================================================
void WS2812::pixel_set_all_color(uint8_t red, uint8_t green, uint8_t blue)
{ 
	if(_brightness) 
	{ 
		red = (red * _brightness) >> 8;
		green = (green * _brightness) >> 8;
		blue = (blue * _brightness) >> 8;
	}

    for (int i = 0; i < strip.num_leds; i++)
	{
		strip.leds[i].simple.g = green;
		strip.leds[i].simple.r = red;
		strip.leds[i].simple.b = blue;
	}
}

//========================================================================
// ����: RGB������ɫ����ʾ.
// ����: index:�ڼ���RGB;red,green,blue:RGB��ɫֵ.
// ����: none.
//========================================================================
uint8_t WS2812::pixel_set_color_and_show(uint16_t index, uint8_t red, uint8_t green, uint8_t blue )
{
	if(_brightness) 
	{ 
		red = (red * _brightness) >> 8;
		green = (green * _brightness) >> 8;
		blue = (blue * _brightness) >> 8;
	}
	
	if (index < strip.num_leds)
	{
		strip.leds[index].simple.r = red;
		strip.leds[index].simple.g = green;
		strip.leds[index].simple.b = blue;
		
		pixel_show();
	}
	else
		return 1;
	return 0;
}

//========================================================================
// ����: �ر�RGB.
// ����: none
// ����: none.
//========================================================================
void WS2812::pixel_clear()
{
	for (int i = 0; i < strip.num_leds; i++)
	{
		strip.leds[i].simple.g = 0;
		strip.leds[i].simple.r = 0;
		strip.leds[i].simple.b = 0;
	}
	
	pixel_show();
}

//========================================================================
// ����: ��ɫת��.
// ����: 0-255��ɫֵ
// ����: 32λWRGBֵ.
//========================================================================
uint32_t WS2812::wheel(uint8_t WheelPos)
{
	WheelPos = 255 - WheelPos;
  	if(WheelPos < 85) {
  	  return color(255 - WheelPos * 3, 0, WheelPos * 3);
  	}
  	if(WheelPos < 170) {
  	  WheelPos -= 85;
  	  return color(0, WheelPos * 3, 255 - WheelPos * 3);
  	}
  	WheelPos -= 170;
  	return color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
 
//========================================================================
// ����: �ʺ�ѭ������Ч��.
// ����: ���ʱ��
// ����: none.
//========================================================================
void WS2812::rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.num_leds; i++) {
      pixel_set_color(i, wheel((i+j) & 255));
    }
    pixel_show();
    delay(wait);
  }
}

//========================================================================
// ����: �������Ч��.
// ����: ���ʱ��
// ����: none.
//========================================================================
void WS2812::rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.num_leds; i++) {
      	pixel_set_color(i, wheel(((i * 256 / strip.num_leds) + j) & 255));
    }
    pixel_show();
    delay(wait);
  }
}

//========================================================================
// ����: ��������.
// ����: ����ֵ
// ����: none.
//========================================================================
void WS2812::setBrightness(uint8_t b){
	_brightness = b;
}


#endif  //asr_ws2812.h