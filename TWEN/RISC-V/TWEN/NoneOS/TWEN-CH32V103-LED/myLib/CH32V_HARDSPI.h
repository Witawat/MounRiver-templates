/**********************************************************
 * �ÿ�ΪCH32V103��Ӳ��SPI��.
 * ����Ĭ��ʹ��PA5(SCK),PA7(MOSI),PA6(MISO)
 * *******************************************************/

#ifndef _HARDSPI_H_
#define _HARDSPI_H_

#include <CH32V103.h>

class HARDSPI {
public:
    void begin();   //��ʼ��

    void set_speed(uint16_t speed);
    void set_mode(uint8_t CPOL, uint8_t CPHL);

    inline static uint8_t inline_wr_data(uint8_t dat)  //��дһ���ֽ�����
    {
        uint8_t i=0;
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)  //���ͻ���ǿ�
        {
            i++;
            if(i>200)return 0;
        }

        SPI_I2S_SendData(SPI1, dat);
        i=0;

        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ջ���Ϊ��
        {
            i++;
            if(i>200)return 0;
        }

        return SPI_I2S_ReceiveData(SPI1);
    }

    uint8_t wr_data(uint8_t dat)    //��дһ���ֽ�����
    {
        return inline_wr_data(dat);
    }

    void write_data(uint8_t dat)    //д��һ���ֽ�����
    {
        wr_data(dat);
    }

    uint8_t read_data(void) //��ȡһ���ֽ�����
    {
        return wr_data(0xff);
    }

    //��ֹSPI����
    void end()
    {
        SPI_Cmd(SPI1, DISABLE);
    }
};

//========================================================================
// ����: SPI��ʼ��.
// ����: none.
// ����: none.
//========================================================================
void HARDSPI::begin()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //ȫ˫��ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
}

//========================================================================
// ����: SPI��������.
// ����: ����0-7��2��4��8��16��32��64��128��256��Ƶ��
// ����: none.
//========================================================================
void HARDSPI::set_speed(uint16_t speed)
{
    SPI_Cmd(SPI1, DISABLE);
    SPI1->CTLR1 &= ~(uint16_t)0x38;
    SPI1->CTLR1 |= (speed<<3);
    SPI_Cmd(SPI1, ENABLE);
}

//========================================================================
// ����: SPI����ģʽ.
// ����: CPOL:ʱ�Ӽ���(0:����ʱSCKΪ�͵�ƽ; 1:����ʱSCKΪ�ߵ�ƽ);
//       CPHL:ʱ����λ(0:���ݲ����ӵ�һ��ʱ���ؿ�ʼ; 1:���ݲ����ӵڶ���ʱ���ؿ�ʼ)��
// ����: none.
//========================================================================
void HARDSPI::set_mode(uint8_t CPOL, uint8_t CPHL)
{
    SPI_Cmd(SPI1, DISABLE);
    SPI1->CTLR1 &= ~(uint16_t)0x3;
    SPI1->CTLR1 |= (uint16_t)((CPOL<<1) | CPHL);
    SPI_Cmd(SPI1, ENABLE);
}

HARDSPI hardspi;
#endif
