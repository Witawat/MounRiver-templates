/**********************************************************
 * �ÿ�ΪCH32V103�ĺ����շ���.
 * *******************************************************/
#ifndef _CH32V_IRSENDREV_H_
#define _CH32V_IRSENDREV_H_

#include "CH32V103.h"
#include "ch32v_pwm.h"

/******************** ���ⷢ�����ʱ��궨��, �û���Ҫ�����޸�  *******************/
#define     NEC_HDR_MARK	9000     //������ߵ�ƽ
#define     NEC_HDR_SPACE	4500     //������͵�ƽ
#define     NEC_BIT_MARK	560
#define     NEC_ONE_SPACE	1690
#define     NEC_ZERO_SPACE	560

/*******************************  ������պ궨��  *********************************/
#define     RAWBUF          68
#define     D_LEN           0
#define     D_STARTH        1
#define     D_STARTL        2
#define     D_SHORT         3
#define     D_LONG          4
#define     D_DATALEN       5
#define     D_DATA          6

//�������״̬
#define     STATE_IDLE      2
#define     STATE_MARK      3
#define     STATE_SPACE     4
#define     STATE_STOP      5
/******************** ������ղ���ʱ��궨��, �û���Ҫ�����޸�  *******************/
#define     USECPERTICK     50
#define     _GAP            6000    // Minimum map between transmissions
#define     GAP_TICKS       (_GAP/USECPERTICK)

typedef struct {
    uint8_t recvpin;              //�����������
    uint8_t rcvstate;             //״̬
    uint32_t timer;           // state timer, counts 50uS ticks.
    unsigned int rawbuf[RAWBUF];  //�������ԭʼ����
    uint8_t rawlen;               //ԭʼ���ݳ���
} irparams_t;

class decode_results
{
public:
    volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
    uint8_t rawlen;           // Number of records in rawbuf.
};

class IRSendRev
{
//�������
private:
    volatile irparams_t irparams;
    decode_results results;
    uint8_t decode(decode_results *results);
    void enableIRIn();
public:
    void begin(uint8_t pin);    //������ճ�ʼ��
    uint8_t recv_addr(void);    //������յ�ַ
    uint8_t recv(void);     //�����������
    uint8_t recv(uint8_t *revData); 
    uint8_t available();    //��������Ƿ���յ�����
    void recv_callback();   //������ջص�������50uS����һ��
//���ⷢ��
private: 
    PWM_CHANNEL_Name ir_send_channel;
    uint8_t ir_send_pin;
    void mark(uint32_t usec);
    void space(uint32_t usec);
	void enableIROut(uint32_t hz);
public:
    void begin(PWM_CHANNEL_Name channel, uint8_t pin);  //���ⷢ�ͳ�ʼ��
    void sendRaw(uint32_t buf[], uint32_t len, uint32_t hz);  
    void send_nec(uint8_t address, uint8_t command);    //���ⷢ��NEC��
};

/*************************************************   ���ⷢ��  *********************************************************/
void IRSendRev::begin(PWM_CHANNEL_Name channel, uint8_t pin)
{
    ir_send_channel = channel;
    ir_send_pin = pin;
}

//========================================================================
// ����: ���ⷢ��ʹ��.
// ����: none
// ����: none.
//========================================================================
void IRSendRev::enableIROut(uint32_t hz) 
{
   PWM_Init(ir_send_channel, ir_send_pin, hz, 0); 
}

//========================================================================
// ����: ���ⷢ���ز��ź�.
// ����: time:�����ʱ��.
// ����: none.
//========================================================================
void IRSendRev::mark(uint32_t time) 
{
    PWM_Duty_Updata(ir_send_channel, 33);
    delayMicroseconds(time);
}

//========================================================================
// ����: ���ⷢ������ź�.
// ����: time:�����ʱ��.
// ����: none.
//========================================================================
void IRSendRev::space(uint32_t time) 
{
    PWM_Duty_Updata(ir_send_channel, 100);
    delayMicroseconds(time);
}

//========================================================================
// ����: ���ⷢ��
// ����: buf: ���ⷢ������(buf[0]Ϊ���Ϳ����ź�ʱ�䣬buf[1]Ϊ�����ز��ź�ʱ�䣬��������); 
//        len: ���͵ĳ���;
//         hz:  �ز�Ƶ��.
// ����: none.
//========================================================================
void IRSendRev::sendRaw(uint32_t buf[], uint32_t len, uint32_t hz)
{
    enableIROut(hz);
    for(uint32_t i = 0; i < len; i++)
    {
        if(i & 1){
            space(buf[i]);
        }else{
            mark(buf[i]);
        }
    }
    space(0);
}

//========================================================================
// ����: ���ⷢ��NEC��
// ����: address:���͵ĵ�ַ�� command������.
// ����: none.
//========================================================================
void IRSendRev::send_nec(uint8_t address, uint8_t command)
{
    uint8_t i;
    uint8_t inverse_address = ~address;
    uint8_t inverse_command = ~command;

    // 1. �ز�����
    enableIROut(38000);
    // 2. ����������
    mark(NEC_HDR_MARK);
    space(NEC_HDR_SPACE);
    // 3. ���͵�ַ
    for (i=0;i<8;i++)
    {
        if (address & 0x01) 
        {
            mark(NEC_BIT_MARK);
            space(NEC_ONE_SPACE);
        } 
        else 
        {
            mark(NEC_BIT_MARK);
            space(NEC_ZERO_SPACE);
        }
        address >>= 1;
    }
    // 4. ���͵�ַ����
    
    for (i=0;i<8;i++)
    {
        if (inverse_address & 0x01) 
        {
            mark(NEC_BIT_MARK);
            space(NEC_ONE_SPACE);
        } 
        else 
        {
            mark(NEC_BIT_MARK);
            space(NEC_ZERO_SPACE);
        }
        inverse_address >>= 1;
    }

    // 5. ��������
    for (i=0;i<8;i++)
    {
        if (command & 0x01) 
        {
            mark(NEC_BIT_MARK);
            space(NEC_ONE_SPACE);
        } 
        else 
        {
            mark(NEC_BIT_MARK);
            space(NEC_ZERO_SPACE);
        }
        command >>= 1;
    }

    // 6. ���������
    for (i=0;i<8;i++)
    {
        if ((inverse_command) & 0x01) 
        {
            mark(NEC_BIT_MARK);
            space(NEC_ONE_SPACE);
        } 
        else 
        {
            mark(NEC_BIT_MARK);
            space(NEC_ZERO_SPACE);
        }
        inverse_command >>= 1;
    }
    mark(NEC_BIT_MARK);
    space(1);
}

/*************************************************   �������  *********************************************************/
//========================================================================
// ����: ������ճ�ʼ��
// ����: pin:����.
// ����: none.
//========================================================================
void IRSendRev::begin(uint8_t pin)
{
    irparams.recvpin = pin;
    irparams.rcvstate = STATE_IDLE;
    irparams.rawlen = 0;  
    pinMode(pin, GPIO_Mode_IN_FLOATING);  
}

//========================================================================
// ����: ����������ݽ���
// ����: none.
// ����: none.
//========================================================================
uint8_t IRSendRev::decode(decode_results *results) {
    results->rawbuf = irparams.rawbuf;
    results->rawlen = irparams.rawlen;
    if (irparams.rcvstate != STATE_STOP) {
        return 0;
    }
    irparams.rcvstate = STATE_IDLE;
    irparams.rawlen = 0;
    return 1;
}

//========================================================================
// ����: ������ջ�ȡ��ַ
// ����: none.
// ����: ����.
//========================================================================
uint8_t IRSendRev::recv_addr(void)
{
    uint8_t revData[10];
    recv(revData);
    return revData[6];
}

//========================================================================
// ����: ������ջ�ȡ����
// ����: none.
// ����: ����.
//========================================================================
uint8_t IRSendRev::recv(void)
{
    uint8_t revData[10];
    recv(revData);
    return revData[8];
}

//========================================================================
// ����: �����������
// ����: revData:���ص���������ָ��; 
// ����: none.
//========================================================================
uint8_t IRSendRev::recv(uint8_t *revData)
{
    uint32_t count       = results.rawlen;
    uint32_t nshort      = 0;
    uint32_t nlong       = 0;
    uint32_t count_data  = 0;

    count_data = (count-4)/16;      //count_data = 4

    for(uint32_t i = 0; i < 10; i++)           // count nshort
    {
        nshort += results.rawbuf[3+2*i];
    }
    nshort /= 10;

    uint32_t i = 0;
    uint32_t j = 0;
    while(1)        // count nlong
    {
        if((uint32_t)results.rawbuf[4+2*i] > (1.5*nshort))
        {
            nlong += results.rawbuf[4+2*i];
            j++;
        }
        i++;
        if(j==10)break;
        if((4+2*i)>(count-10))break;
    }
    nlong /= j;

    for(i = 0; i<count_data; i++)
    {
        revData[i+D_DATA] = 0x00;
        for(j = 0; j<8; j++)
        {
            if((uint32_t)results.rawbuf[4 + 16*i + j*2] > (uint32_t)(1.5*nshort))
            {
                revData[i+D_DATA] |= (0x01<< j);
            }
            else
            {
                revData[i+D_DATA] &= ~(0x01<<j);
            }
        }
    }
    revData[D_LEN]      = count_data+5;
    revData[D_STARTH]   = results.rawbuf[1];
    revData[D_STARTL]   = results.rawbuf[2];
    revData[D_SHORT]    = nshort;
    revData[D_LONG]     = nlong;
    revData[D_DATALEN]  = count_data;

    irparams.rcvstate = STATE_IDLE; //׼��������һ������
    irparams.rawlen = 0;

    return revData[D_LEN]+1;
}

//========================================================================
// ����: �����Ƿ���յ�����.
// ����: none.
// ����: 0��û�н��յ�����;����ֵ:���ݳ���.
//========================================================================
uint8_t IRSendRev::available()
{
    if(decode(&results))
    {
        uint32_t count  = results.rawlen;
        if(count < 20 || (count -4)%8 != 0)
        {
            irparams.rcvstate = STATE_IDLE; //׼��������һ������
            irparams.rawlen = 0;
            return 0;
        }
        return (uint8_t)((count-4)/16+6);
    }
    else 
    {
        return 0;
    }
}

//========================================================================
// ����: ��������жϻص�����
// ����: none.
// ����: none.
//========================================================================
void IRSendRev::recv_callback()
{
    uint8_t irdata;
    irdata = digitalRead(irparams.recvpin);
    irparams.timer++;
    if (irparams.rawlen > RAWBUF) {    //������ݳ������
        irparams.rcvstate = STATE_STOP;
        irparams.timer = 0;
    }
    switch(irparams.rcvstate) {
        case STATE_IDLE: //����̬
            if (irdata == LOW) {        //���յ���ʼ����
                if (irparams.timer < GAP_TICKS) {
                    irparams.timer = 0;
                }else{
                    irparams.rawlen = 0;
                    irparams.rawbuf[irparams.rawlen++] = irparams.timer;
                    irparams.timer = 0;
                    irparams.rcvstate = STATE_MARK;
                }
            }
        break;

        case STATE_MARK: // timing MARK
            if(irdata == HIGH) 
            {
                irparams.rawbuf[irparams.rawlen++] = irparams.timer;
                irparams.timer = 0;
                irparams.rcvstate = STATE_SPACE;
            }
        break;

        case STATE_SPACE: // timing SPACE
            if (irdata == LOW) { // SPACE just ended, record it
                irparams.rawbuf[irparams.rawlen++] = irparams.timer;
                irparams.timer = 0;
                irparams.rcvstate = STATE_MARK;
            }else { // SPACE
                if (irparams.timer > GAP_TICKS) {
                    irparams.rcvstate = STATE_STOP;
                } 
            }
        break;

        case STATE_STOP: // waiting, measuring gap
            if (irdata == LOW) { // reset gap timer
                irparams.timer = 0;
            }
        break;
    }
}


#endif
