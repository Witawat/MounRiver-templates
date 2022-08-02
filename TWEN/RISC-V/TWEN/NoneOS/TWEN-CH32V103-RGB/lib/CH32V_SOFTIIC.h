#ifndef _CH32V_SOFTIIC_H_
#define _CH32V_SOFTIIC_H_

#include "CH32V103.h"

#define KI2C_DELAY_USEC 1

//��λ
uint8_t const KI2C_READ = 1;

//дλ
uint8_t const KI2C_WRITE = 0;

//------------------------------------------------------------------------------
class KIICBase {
public:
  //������
  //last:Ӧ�����λ
  virtual uint8_t read(uint8_t last) = 0;
  //���¿�ʼ�ź�
  //addr:�豸��ַ
  virtual bool restart(uint8_t addr) = 0;
  //��ʼ�ź�
  //addr:�豸��ַ
  virtual bool start(uint8_t addr) = 0;
  //ֹͣ�ź�
  virtual void stop(void) = 0;
  //д����
  //last:Ҫд������
  virtual bool write(uint8_t data) = 0;
};
//---------------------------------------------------------------------

class KSoftIIC : public KIICBase {
public:
  KSoftIIC();
  void begin(uint8_t sdapin, uint8_t sclpin); //SOFTIIC��ʼ��
  uint8_t read(uint8_t last); //������
  bool restart(uint8_t addr); //�ظ���ʼ�ź�
  bool start(uint8_t addr); //��ʼ�ź�
  void stop(void);  //ֹͣ�ź�
  bool write(uint8_t b);  //д����
private:
  //SoftI2cMaster() {}
  uint8_t _sdapin;
  uint8_t _sclpin;
};

//========================================================================
// ����: ���캯��.
// ����: none
// ����: none.
//========================================================================
KSoftIIC::KSoftIIC()
{}

//========================================================================
// ����: ��ʼ������.
// ����: sdapin��SDA����; sclpin: scl����.
// ����: none.
//========================================================================
void KSoftIIC::begin(uint8_t sdapin,uint8_t sclpin)
{
	_sdapin = sdapin;
	pinMode(_sdapin,GPIO_Mode_Out_PP);
	digitalWrite(_sdapin,HIGH);
	delayMicroseconds(2);
	_sclpin = sclpin;
	pinMode(_sclpin,GPIO_Mode_Out_PP);
	digitalWrite(_sclpin,HIGH);
}

//========================================================================
// ����: SOFTIIC������ʼ�ź�.
// ����: addr���豸��ַ.
// ����: none.
//========================================================================
bool KSoftIIC::start(uint8_t addr)
{
	digitalWrite(_sdapin, LOW);
	delayMicroseconds(KI2C_DELAY_USEC);
	digitalWrite(_sclpin, LOW);
	delayMicroseconds(1);
	return write(addr);
}

//========================================================================
// ����: SOFTIIC�����ظ���ʼ�ź�.
// ����: none.
// ����: none.
//========================================================================
bool KSoftIIC::restart(uint8_t addr)
{
	digitalWrite(_sdapin, HIGH);
	delayMicroseconds(1);
	digitalWrite(_sclpin, HIGH);
	delayMicroseconds(KI2C_DELAY_USEC);
	return start(addr);
}

//========================================================================
// ����: SOFTIIC����ֹͣ�ź�.
// ����: none.
// ����: none.
//========================================================================
void KSoftIIC::stop()
{
	digitalWrite(_sdapin,LOW);
	delayMicroseconds(KI2C_DELAY_USEC);
	digitalWrite(_sclpin,HIGH);
	delayMicroseconds(KI2C_DELAY_USEC);
	digitalWrite(_sdapin,HIGH);
	delayMicroseconds(KI2C_DELAY_USEC);
}

//========================================================================
// ����: SOFTIIC������.
// ����: last:0��ʾӦ��,1��ʾ��Ӧ��.
// ����: ��ȡ��������.
//========================================================================
uint8_t KSoftIIC::read(uint8_t last) {
  uint8_t b = 0;
  digitalWrite(_sdapin, HIGH);
  pinMode(_sdapin, GPIO_Mode_IPU);
  for (uint8_t i = 0; i < 8; i++) {
    b <<= 1;
    delayMicroseconds(KI2C_DELAY_USEC);
    digitalWrite(_sclpin, HIGH);
	  delayMicroseconds(1);
    if (digitalRead(_sdapin)) b |= 1;
    digitalWrite(_sclpin, LOW);
    delayMicroseconds(KI2C_DELAY_USEC);
  }
  pinMode(_sdapin, GPIO_Mode_Out_PP);
  digitalWrite(_sdapin, last);
  digitalWrite(_sclpin, HIGH);
  delayMicroseconds(KI2C_DELAY_USEC);
  digitalWrite(_sclpin, LOW);
  delayMicroseconds(KI2C_DELAY_USEC);
  digitalWrite(_sdapin, LOW);
  return b;
}

//========================================================================
// ����: д����.
// ����: data������.
// ����: 0,û�н��յ�Ӧ��;1,���յ�Ӧ��.
//========================================================================
bool KSoftIIC::write(uint8_t data) {
  for (uint8_t m = 0X80; m != 0; m >>= 1) {
    if(m&data){
        digitalWrite(_sdapin, 1);
	  }else{
        digitalWrite(_sdapin, 0);
	  }
    digitalWrite(_sclpin, HIGH);
    //delayMicroseconds(KI2C_DELAY_USEC);
    digitalWrite(_sclpin, LOW);
    //delayMicroseconds(KI2C_DELAY_USEC);
  }
  // get Ack or Nak
  pinMode(_sdapin, GPIO_Mode_IPU);
  // enable pullup
  digitalWrite(_sdapin, HIGH);
  delayMicroseconds(1);
  digitalWrite(_sclpin, HIGH);
  delayMicroseconds(1);
  uint8_t rtn = digitalRead(_sdapin);
  digitalWrite(_sclpin, LOW);
  delayMicroseconds(1);
  pinMode(_sdapin, GPIO_Mode_Out_PP);
  digitalWrite(_sdapin, LOW);
  return rtn == 0;
}

#endif  // HDUSB_softiic.h
