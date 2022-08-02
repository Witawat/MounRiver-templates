#ifndef _CH32V_NRF24L01_H_
#define _CH32V_NRF24L01_H_

#include <CH32V103.h>
#include "CH32V_HARDSPI.h"

#define RX_DR	    0x40
#define TX_DS	    0x20
#define MAX_RT      0x10

// SPI(nRF24L01) commands
#define READ_REG    0x00		// Define read command to register
#define WRITE_REG   0x20		// Define write command to register
#define RD_RX_PLOAD 0x61		// Define RX payload register address
#define WR_TX_PLOAD 0xA0		// Define TX payload register address
#define FLUSH_TX    0xE1		// Define flush TX register command
#define FLUSH_RX    0xE2		// Define flush RX register command
#define REUSE_TX_PL 0xE3		// Define reuse TX payload register command
#define NOP         0xFF		// Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define CONFIG      0x00		// 'Config' register address
#define EN_AA       0x01		// 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02		// 'Enabled RX addresses' register address
#define SETUP_AW    0x03		// 'Setup address width' register address
#define SETUP_RETR  0x04		// 'Setup Auto. Retrans' register address
#define RF_CH       0x05		// 'RF channel' register address
#define RF_SETUP    0x06		// 'RF setup' register address
#define STATUS      0x07		// 'Status' register address
#define OBSERVE_TX  0x08		// 'Observe TX' register address
#define CD          0x09		// 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A		// 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B		// 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C		// 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D		// 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E		// 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F		// 'RX address pipe5' register address
#define TX_ADDR     0x10		// 'TX address' register address
#define RX_PW_P0    0x11		// 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12		// 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13		// 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14		// 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15		// 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16		// 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17		// 'FIFO Status Register' register address

#define TX_ADR_WIDTH   5		// 5�ֽڿ�ȵķ���/���յ�ַ
#define TX_PLOAD_WIDTH 32		// ����ͨ����Ч���ݿ��

#define NRF_HARDSPI     0
#define NRF_SOFTSPI     1

class NRF24L01
{
public:
    //ʹ��ģ��spi
    NRF24L01(uint8_t ce, uint8_t csn, uint8_t irq, uint8_t sck, uint8_t miso, uint8_t mosi); //���ŷ���
    //ʹ��Ӳ��spi
    NRF24L01(uint8_t ce, uint8_t csn, uint8_t irq);
    void begin();     //��ʼ��
    void set_rx_ch(uint8_t channel); //���ý���ͨ��
    void set_rx_addr(uint8_t* address);   //���ý��յ�ַ
    void rx_setup();   //���ý���
    void set_tx_ch(uint8_t channel); //���÷���ͨ��
    void set_tx_addr(uint8_t* address);  //���÷��͵�ַ
    void tx_setup(); //���÷���
    void write(uint8_t * BUF);   //��������
    void write(int shu);    //������ֵ
    void write(String str);  //�����ַ���
    void write(String name,int shu);    //���ͱ���
    uint8_t read(uint8_t* buf); //�Ƿ���յ����ݲ����浽buf��
    uint8_t read(); //�Ƿ���յ�����
    int read_num();  //��ȡ��ֵ
    String read_str();   //��ȡ�ַ���
    int read_name(String name);  //��ȡ������ֵ

private:
    uint8_t _CE_pin;
    uint8_t _CSN_pin;
    uint8_t _IRQ_pin; 
    uint8_t _SCK_pin;
    uint8_t _MISO_pin;
    uint8_t _MOSI_pin;

    uint8_t _connect;

    uint8_t TX_ADDRESS[TX_ADR_WIDTH] = { 0x00, 0x66, 0x66, 0x66, 0x66 };	// ����һ����̬���͵�ַ
    uint8_t RX_ADDRESS[TX_ADR_WIDTH] = { 0x00, 0x66, 0x66, 0x66, 0x66 };	// ����һ����̬���͵�ַ

    uint8_t RX_BUF[TX_PLOAD_WIDTH]; //���ջ���
    uint8_t ch=7;   //Ĭ�Ϸ���ͨ��
    uint8_t rch=7;   //Ĭ�Ͻ���ͨ��

    uint8_t SPI_RW(uint8_t dat);
    uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value);
    uint8_t SPI_Read(uint8_t reg);
    uint8_t SPI_Read_Buf (uint8_t reg, uint8_t * pBuf, uint8_t bytes);
    uint8_t SPI_Write_Buf (uint8_t reg, uint8_t * pBuf, uint8_t bytes);
};

//========================================================================
// ����: ���ŷ���.
// ����: none
// ����: none.
//========================================================================
NRF24L01::NRF24L01(uint8_t ce, uint8_t csn, uint8_t irq, uint8_t sck, uint8_t miso, uint8_t mosi)
{
    _connect = NRF_SOFTSPI;
    _CE_pin = ce;
    _CSN_pin = csn;
    _IRQ_pin = irq; 
    _SCK_pin = sck;
    _MISO_pin = miso;
    _MOSI_pin = mosi;
}

//========================================================================
// ����: ���ŷ���.
// ����: none
// ����: none.
//========================================================================
NRF24L01::NRF24L01(uint8_t ce, uint8_t csn, uint8_t irq)
{
    _connect = NRF_HARDSPI;
    _CE_pin = ce;
    _CSN_pin = csn;
    _IRQ_pin = irq; 
}

//========================================================================
// ����: ����SPIЭ�飬дһ�ֽ����ݵ�nRF24L01��ͬʱ��nRF24L01����һ�ֽ�.
// ����: д����ֽ�����.
// ����: �������ֽ�����.
//========================================================================
uint8_t NRF24L01::SPI_RW(uint8_t dat)
{
    uint8_t i;
    uint8_t datavalue=0;
    if(_connect == NRF_HARDSPI)     //Ӳ��SPI
    {
        return hardspi.wr_data(dat);
    }else{                          //ģ��SPI
        for(i=0;i<8;i++){
            if(dat&0x80){
                digitalWrite(_MOSI_pin, 1);
            }else{
                digitalWrite(_MOSI_pin, 0);
            }
            dat <<=1;
            digitalWrite(_SCK_pin, 1);
            datavalue <<=1;
            if(digitalRead(_MISO_pin)){                  
                datavalue |= 0x01;
            }
            digitalWrite(_SCK_pin, 0);
        }
        return datavalue;
    }
}

//========================================================================
// ����: д����value��reg�Ĵ���.
// ����: reg:�Ĵ���;value:����.
// ����: ״̬.
//========================================================================
uint8_t NRF24L01::SPI_RW_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	digitalWrite (_CSN_pin, LOW);
	// CSN�õͣ���ʼ��������
	status = SPI_RW (reg);		// ѡ��Ĵ�����ͬʱ����״̬��
	SPI_RW (value);				// Ȼ��д���ݵ��üĴ���
	digitalWrite (_CSN_pin, HIGH);		// CSN���ߣ��������ݴ���
	return (status);			// ����״̬�Ĵ���
}

//========================================================================
// ����: ��reg�Ĵ�����һ�ֽ�����.
// ����: reg:�Ĵ���.
// ����: �Ĵ�������.
//========================================================================
uint8_t NRF24L01::SPI_Read(uint8_t reg)
{
	uint8_t reg_val;

	digitalWrite (_CSN_pin, LOW);		// CSN�õͣ���ʼ��������
	SPI_RW (reg);				// ѡ��Ĵ���
	reg_val = SPI_RW (0);		// Ȼ��ӸüĴ���������
	digitalWrite (_CSN_pin, HIGH);		// CSN���ߣ��������ݴ���
	return (reg_val);			// ���ؼĴ�������
}

//========================================================================
// ����: ��reg�Ĵ�������bytes���ֽڣ�ͨ��������ȡ����ͨ�����ݻ����/���͵�ַ.
// ����: reg:�Ĵ���;pBuf:���ص�����ָ��;bytes:��Ҫ��ȡ���ֽ���;.
// ����: ״̬�Ĵ���.
//========================================================================
uint8_t NRF24L01::SPI_Read_Buf (uint8_t reg, uint8_t * pBuf, uint8_t bytes)
{
	uint8_t status, i;

	digitalWrite (_CSN_pin, LOW);		// CSN�õͣ���ʼ��������
	status = SPI_RW (reg);		// ѡ��Ĵ�����ͬʱ����״̬��
	for (i = 0; i < bytes; i++)
    {
		pBuf[i] = SPI_RW (0);	// ����ֽڴ�nRF24L01����
    }
	digitalWrite (_CSN_pin, HIGH);		// CSN���ߣ��������ݴ���
	return (status);			// ����״̬�Ĵ���
}

//========================================================================
// ����: ��pBuf�����е�����д�뵽nRF24L01��ͨ������д�뷢��ͨ�����ݻ����/���͵�ַ.
// ����: reg:�Ĵ���;pBuf:д�������ָ��;bytes:��Ҫд����ֽ���.
// ����: ״̬�Ĵ���.
//========================================================================
uint8_t NRF24L01::SPI_Write_Buf (uint8_t reg, uint8_t * pBuf, uint8_t bytes)
{
	uint8_t status, i;

	digitalWrite (_CSN_pin, LOW);		// CSN�õͣ���ʼ��������
	status = SPI_RW (reg);		// ѡ��Ĵ�����ͬʱ����״̬��
	for (i = 0; i < bytes; i++)
    {
        SPI_RW (pBuf[i]);		// ����ֽ�д��nRF24L01
    }
	digitalWrite (_CSN_pin, HIGH);		// CSN���ߣ��������ݴ���
	return (status);			// ����״̬�Ĵ���
}

//=======================================================================
// ����: ���߳�ʼ��.
// ����: none.
// ����: ���յ��ַ���.
//========================================================================
void NRF24L01::begin()
{
	SPI_InitTypeDef  SPI_InitStructure;
    if(_connect == NRF_HARDSPI) //Ӳ��spi��ʼ��
    {
		hardspi.begin();
        hardspi.set_mode(0,0);
        hardspi.set_speed(2);   //nrf24l01��spi�������֧��8Mbps��������Ϊ8��Ƶ
    }else{                      //ģ��spi��ʼ��
        pinMode(_SCK_pin, GPIO_Mode_Out_PP);
        pinMode(_MOSI_pin, GPIO_Mode_Out_PP);
        pinMode(_MISO_pin, GPIO_Mode_IN_FLOATING);
    }
	pinMode(_IRQ_pin,GPIO_Mode_IN_FLOATING);
    pinMode(_CE_pin,GPIO_Mode_Out_PP);
    pinMode(_CSN_pin,GPIO_Mode_Out_PP);
	digitalWrite(_IRQ_pin, 0);
	digitalWrite(_CE_pin, 0);		// chip enable  
	digitalWrite(_CSN_pin, 1);		// Spi disable     
}

/***************************************************���߽�������************************************/
//=======================================================================
// ����: �������߽���ͨ��.
// ����: ͨ��.
// ����: none.
//========================================================================
void NRF24L01::set_rx_ch (uint8_t channel)
{
    rch=channel;
}

//========================================================================
// ����: �������߽��յ�ַ.
// ����: ���͵�ַָ��.
// ����: none.
//========================================================================
void NRF24L01::set_rx_addr (uint8_t* address)
{
 for(int i=0;i<5;i++)*(RX_ADDRESS+i)=*(address+i);
}

//========================================================================
// ����: �������߽���.
// ����: none.
// ����: none.
//========================================================================
void NRF24L01::rx_setup ()
{
	digitalWrite (_CE_pin, LOW);

	SPI_Write_Buf (WRITE_REG + RX_ADDR_P0, RX_ADDRESS, TX_ADR_WIDTH);	// �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	SPI_RW_Reg (WRITE_REG + EN_AA, 0x01);	// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	SPI_RW_Reg (WRITE_REG + EN_RXADDR, 0x01);	// ʹ�ܽ���ͨ��0
	SPI_RW_Reg (WRITE_REG + RF_CH, rch);	// ѡ����Ƶͨ��0x40
	SPI_RW_Reg (WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);	// ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	SPI_RW_Reg (WRITE_REG + RF_SETUP, 0x07);	// 0x07���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	SPI_RW_Reg (WRITE_REG + CONFIG, 0x0f);	// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
    //SPI_RW_Reg (WRITE_REG + CONFIG, 0x03);	

	digitalWrite (_CE_pin, HIGH);// ����CE���������豸
}

/***************************************************���߷�������************************************/
//=======================================================================
// ����: �������߷���ͨ��.
// ����: ͨ��.
// ����: none.
//========================================================================
void NRF24L01::set_tx_ch (uint8_t channel)
{
    ch=channel;
}

//=======================================================================
// ����: �������߷��͵�ַ.
// ����: ͨ��.
// ����: none.
//========================================================================
void NRF24L01::set_tx_addr (uint8_t* address)
{
    for(int i=0;i<5;i++)*(TX_ADDRESS+i)=*(address+i);
}

//========================================================================
// ����: �������߷���.
// ����: none.
// ����: none.
//========================================================================
void NRF24L01::tx_setup()
{
	digitalWrite (_CE_pin, LOW);

  	SPI_Write_Buf (WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	// д�뷢�͵�ַ
	SPI_Write_Buf (WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	//SPI_Write_Buf (WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);	// д���ݰ���TX FIFO
	//SPI_RW_Reg (WRITE_REG + EN_AA, 0x01);	// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	SPI_RW_Reg (WRITE_REG + EN_RXADDR, 0x01);	// ʹ�ܽ���ͨ��0
	SPI_RW_Reg (WRITE_REG + SETUP_RETR, 0x03);	// �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	SPI_RW_Reg (WRITE_REG + RF_CH, ch);	// ѡ����Ƶͨ��0x40
	SPI_RW_Reg (WRITE_REG + RF_SETUP, 0x07);	// ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	SPI_RW_Reg (WRITE_REG + CONFIG, 0x0e);	// CRCʹ�ܣ�16λCRCУ�飬�ϵ�
    //SPI_RW_Reg (WRITE_REG + CONFIG, 0x02);

	digitalWrite (_CE_pin, HIGH);
}

//=======================================================================
// ����: ���߷���32���ֽ�����.
// ����: buf:����.
// ����: none.
//========================================================================
void  NRF24L01::write(uint8_t * BUF)
{
    uint8_t sta;

	digitalWrite (_CE_pin, LOW);
	//SPI_Write_Buf (WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	// д�뷢�͵�ַ
	//SPI_Write_Buf (WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	SPI_Write_Buf (WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);	// д���ݰ���TX FIFO
	//SPI_RW_Reg (WRITE_REG + EN_AA, 0x01);	// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	//SPI_RW_Reg (WRITE_REG + EN_RXADDR, 0x01);	// ʹ�ܽ���ͨ��0
	//SPI_RW_Reg (WRITE_REG + SETUP_RETR, 0x0a);	// �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	//SPI_RW_Reg (WRITE_REG + RF_CH, ch);	// ѡ����Ƶͨ��0x40
	//SPI_RW_Reg (WRITE_REG + RF_SETUP, 0x07);	// ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	//SPI_RW_Reg (WRITE_REG + CONFIG, 0x0e);	// CRCʹ�ܣ�16λCRCУ�飬�ϵ�
	digitalWrite (_CE_pin, HIGH);
    //delay(1);

    while(digitalRead(_IRQ_pin)!=0);
    sta = SPI_Read(STATUS);	  // ��״̬�Ĵ���
    SPI_RW_Reg(WRITE_REG + STATUS, sta);  // ���RX_DS�жϱ�־
    if(sta&MAX_RT){
         SPI_RW_Reg(FLUSH_TX, 0xff);  // ���RX_DS�жϱ�־
     }
}

//=======================================================================
// ����: ���߷�������.
// ����: none.
// ����: none.
//========================================================================
void NRF24L01::write (int shu)
{
    uint8_t buf[32];
    String tx="";
    tx=tx+String(shu)+"\r\n";
    uint8_t len = tx.length();
    for(int i=0;i<len;i++)
    {
        if(i>32)break;
        buf[i]=tx.charAt(i);
    }
    write(buf);
}

//=======================================================================
// ����: ���߷����ַ���.
// ����: none.
// ����: none.
//========================================================================
void NRF24L01::write(String str)
{
    uint8_t buf[32];
    String tx="";
    tx=tx+str+"\r\n";
    uint8_t len = tx.length();
    for(int i=0;i<len;i++)
    {
    if(i>32)break;
    buf[i]=tx.charAt(i);

    }
    write(buf);
}

//=======================================================================
// ����: ���߷��ͱ���.
// ����: none.
// ����: none.
//========================================================================
void NRF24L01::write (String name,int shu)
{
    uint8_t buf[32];
    String tx="";
    tx=tx+name+"="+String(shu)+"\r\n";
    uint8_t len = tx.length();
    for(int i=0;i<len;i++)
    {
        if(i>32)break;
        buf[i]=tx.charAt(i);
    }
    write(buf);
}

//=======================================================================
// ����: ���߽��մ���buf��(������32�ֽ�).
// ����: buf:���յ�������ָ��.
// ����: 1,���յ�����;0,û�н��յ�����.
//========================================================================
uint8_t NRF24L01::read(uint8_t* buf)
{
    uint8_t sta;
    sta = SPI_Read(STATUS);	  // ��״̬�Ĵ���
    SPI_RW_Reg(WRITE_REG + STATUS, sta);  // ���RX_DS�жϱ�־
    if(RX_DR&sta)				  // �ж��Ƿ���ܵ�����
	{
        SPI_Read_Buf(RD_RX_PLOAD, buf, TX_PLOAD_WIDTH);  // ��RX FIFO��������
        SPI_RW_Reg(FLUSH_RX, 0xff);  // ���RX_DS�жϱ�־ 
        return 1;
    }else{
         return 0;
    }
}

//=======================================================================
// ����: �����Ƿ���յ�����.
// ����: none.
// ����: 1,���յ�����;0,û�н��յ�����.
//========================================================================
uint8_t NRF24L01::read()
{
    uint8_t sta;
    sta = SPI_Read(STATUS);	  // ��״̬�Ĵ���
    SPI_RW_Reg(WRITE_REG + STATUS, sta);  // ���RX_DS�жϱ�־
	if(RX_DR&sta)				  // �ж��Ƿ���ܵ�����
    {
        SPI_Read_Buf(RD_RX_PLOAD,RX_BUF, TX_PLOAD_WIDTH);  // ��RX FIFO��������
        SPI_RW_Reg(FLUSH_RX, 0xff);  // ���RX_DS�жϱ�־
        return 1;
    }else{
        return 0;
    }
}

//=======================================================================
// ����: ���߽�������.
// ����: none.
// ����: ���յ�����.
//========================================================================
int NRF24L01::read_num()
{
    String s;
    for(int i=0;i<32;i++)
    {
        if(RX_BUF[i]==0x0d||RX_BUF[i]==0x0a)break;
        s+=char(RX_BUF[i]);
    }
    return s.toInt();
}

//=======================================================================
// ����: ���߽����ַ���.
// ����: none.
// ����: ���յ��ַ���.
//========================================================================
String NRF24L01::read_str()
{
    String s;
    for(int i=0;i<32;i++)
    {
        if(RX_BUF[i]==0x0d||RX_BUF[i]==0x0a)break;
        s+=char(RX_BUF[i]);
    }
    return s;    
}

//=======================================================================
// ����: ���߽��ձ���.
// ����: none.
// ����: ���յ��ַ���.
//========================================================================
int NRF24L01::read_name(String name)
{
    String s;
    int value;
    int namelen=name.length();
    for(int i=0;i<32;i++)
    {
        if(RX_BUF[i]==0x0d||RX_BUF[i]==0x0a)break;
        s+=char(RX_BUF[i]);
    }

    if(s.indexOf(name)>-1)value=s.substring(namelen+1,s.length()).toInt();
    return value;       
}

#endif  //CH32V_NRF24L01.h