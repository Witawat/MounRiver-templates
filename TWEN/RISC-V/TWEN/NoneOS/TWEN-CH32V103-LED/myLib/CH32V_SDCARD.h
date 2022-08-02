/*****************************************************
 * �ÿ�ΪCH32V��spi������SD����
******************************************************/
#ifndef _CH32V_SDCARD_H
#define _CH32V_SDCARD_H

#include <CH32V103.h>
#include "CH32V_HARDSPI.h"

// SD�����Ͷ���
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06

// SD��ָ���
#define CMD0    0       //����λ
#define CMD1    1
#define CMD8    8       //����8 ��SEND_IF_COND
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define CMD23   23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define CMD41   41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00

//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

//SD����Ӧ�����
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF


class SDCARD
{
public:
    SDCARD(uint8_t ss_pin);
    uint8_t begin(void);    //��ʼ��
    uint8_t get_CID(uint8_t *cid_data);
    uint8_t get_CSD(uint8_t *csd_data);
    uint32_t get_sector_count(void);    //��ȡSD������������
    uint8_t read_disk(uint8_t *buf,uint32_t sector,uint8_t cnt);    //SD��������
    uint8_t write_disk(uint8_t *buf,uint32_t sector,uint8_t cnt);   //SD��д����
    uint8_t wait_ready(void);
    void dis_select(void);  //ȡ��Ƭѡ
    uint8_t select(void);   //Ƭѡ
private:
    uint8_t  SD_Type=0;//SD��������
    uint8_t  _ss_pin;
    uint8_t wr_byte(uint8_t data);
    uint8_t detect(void);
    uint8_t get_response(uint8_t Response);
    uint8_t recv_data(uint8_t *buf,uint16_t len);
    uint8_t send_block(uint8_t *buf,uint8_t cmd);
    uint8_t send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc);
};

SDCARD::SDCARD(uint8_t ss_pin)
{
    _ss_pin = ss_pin;
}

//========================================================================
// ����: SPI��д����.
// ����: Ҫд�������.
// ����: ����������.
//========================================================================
uint8_t SDCARD::wr_byte(uint8_t data)
{
    SPI_I2S_SendData(SPI1, data);
    return SPI_I2S_ReceiveData(SPI1);

    //return hardspi.wr_data(data);
}

//========================================================================
// ����: ���SD��.
// ����: none.
// ����: 0����⵽SD����1��û�м�⵽sd��.
//========================================================================
uint8_t SDCARD::detect(void)
{
    return 0;
}

//========================================================================
// ����: �ȴ���׼����.
// ����: none.
// ����: 0,׼������;����,�������.
//========================================================================
uint8_t SDCARD::wait_ready(void)
{
    uint32_t t=0;
    do
    {
        if(wr_byte(0XFF)==0XFF)return 0;//OK
        t++;
    }while(t<0XFFFFFF);//�ȴ�
    return 1;
}

//========================================================================
// ����: ȡ��ѡ���ͷ�SPI����.
// ����: none.
// ����: none.
//========================================================================
void SDCARD::dis_select(void)
{
    digitalWrite(_ss_pin, 1); 
    wr_byte(0xff);//�ṩ�����8��ʱ��
}

//========================================================================
// ����: ѡ��sd��,���ҵȴ���׼��OK.
// ����: none.
// ����: 0,�ɹ�;1,ʧ��.
//========================================================================
uint8_t SDCARD::select(void)
{
    digitalWrite(_ss_pin, 0); 
    if(wait_ready()==0)return 0;//�ȴ��ɹ�
    dis_select();
    return 1;//�ȴ�ʧ��
}

//========================================================================
// ����: �ȴ�SD����Ӧ.
// ����: Ҫ�õ��Ļ�Ӧֵ.
// ����: 0,�ɹ��õ��˸û�Ӧֵ;����,�õ���Ӧֵʧ��.
//========================================================================
uint8_t SDCARD::get_response(uint8_t Response)
{
    uint16_t Count=0xFFFF;//�ȴ�����
    while ((wr_byte(0XFF)!=Response)&&Count)Count--;//�ȴ��õ�׼ȷ�Ļ�Ӧ
    if (Count==0)return MSD_RESPONSE_FAILURE;//�õ���Ӧʧ��
    else return MSD_RESPONSE_NO_ERROR;//��ȷ��Ӧ
}

//========================================================================
// ����: ��sd����ȡһ�����ݰ�������.
// ����: buf:���ݻ�����;
//       len:Ҫ��ȡ�����ݳ���.   
// ����: 0,�ɹ�;����,ʧ��.
//========================================================================
uint8_t SDCARD::recv_data(uint8_t *buf,uint16_t len)
{
    if(get_response(0xFE))return 1;//�ȴ�SD������������ʼ����0xFE
    while(len--)//��ʼ��������
    {
        *buf = wr_byte(0xFF);
        buf++;
    }
    //������2��αCRC��dummy CRC��
    wr_byte(0xFF);
    wr_byte(0xFF);
    return 0;//��ȡ�ɹ�
}

//========================================================================
// ����: ��sd��д��һ�����ݰ������� 512�ֽ�.
// ����: buf:���ݻ�����;
//       cmd:ָ��.   
// ����: 0,�ɹ�;����,ʧ��.
//========================================================================
uint8_t SDCARD::send_block(uint8_t *buf,uint8_t cmd)
{
    uint16_t t;
    if(wait_ready())return 1;//�ȴ�׼��ʧЧ
    wr_byte(cmd);
    if(cmd!=0XFD)//���ǽ���ָ��
    {
        //for(t=0;t<512;t++)hardspi.wr_data(buf[t]);//����ٶ�,���ٺ�������ʱ��
        for(t=0;t<512;t++)wr_byte(buf[t]);//����ٶ�,���ٺ�������ʱ��
        wr_byte(0xFF);//����crc
        wr_byte(0xFF);
        t = wr_byte(0xFF);//������Ӧ
        if((t&0x1F)!=0x05)return 2;//��Ӧ����
    }
    return 0;//д��ɹ�
}

//========================================================================
// ����: ��SD������һ������.
// ����: cmd:����
//       arg:�������  
//       crc:У��ֵ
// ����: SD�����ص���Ӧ.
//========================================================================
uint8_t SDCARD::send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t r1;
    uint8_t Retry=0;
    dis_select();//ȡ���ϴ�Ƭѡ
    if(select())return 0XFF;//ƬѡʧЧ
    //����
    wr_byte(cmd | 0x40);//�ֱ�д������
    wr_byte(arg >> 24);
    wr_byte(arg >> 16);
    wr_byte(arg >> 8);
    wr_byte(arg);
    wr_byte(crc);
    if(cmd==CMD12)wr_byte(0xff);//Skip a stuff byte when stop reading
    //�ȴ���Ӧ����ʱ�˳�
    Retry=0X1F;
    do
    {
        r1=wr_byte(0xFF);
    }while((r1&0X80) && Retry--);
    //����״ֵ̬
    return r1;
}

//========================================================================
// ����: ��ȡSD����CID��Ϣ��������������Ϣ.
// ����: cid_data:���CID���ڴ棬����16Byte
// ����: SD�����ص���Ӧ.
//========================================================================
uint8_t SDCARD::get_CID(uint8_t *cid_data)
{
    uint8_t r1;
    //��CMD10�����CID
    r1 = send_cmd(CMD10,0,0x01);
    if(r1 == 0x00)
    {
        r1 = recv_data(cid_data,16);//����16���ֽڵ�����
    }
    dis_select();//ȡ��Ƭѡ
    if(r1)return 1;
    else return 0;
}

//========================================================================
// ����: ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ.
// ����: csd_data:���CSD���ڴ棬����16Byte
// ����: SD�����ص���Ӧ.
//========================================================================
uint8_t SDCARD::get_CSD(uint8_t *csd_data)
{
    uint8_t r1;
    r1 = send_cmd(CMD9,0,0x01);//��CMD9�����CSD
    if(r1 == 0)
    {
        r1 = recv_data(csd_data, 16);//����16���ֽڵ�����
    }
    dis_select();//ȡ��Ƭѡ
    if(r1)return 1;
    else return 0;
}

//========================================================================
// ����: ��ȡSD����������������������.
// ����: none
// ����: 0�� ȡ��������; ����:SD��������(������/512�ֽ�).
//========================================================================
uint32_t SDCARD::get_sector_count(void)
{
    uint8_t csd[16];
    uint64_t Capacity;
    uint8_t n;
    uint16_t csize;
    //ȡCSD��Ϣ������ڼ��������0
    if(get_CSD(csd)!=0) return 0;
    //���ΪSDHC�����������淽ʽ����
    if((csd[0]&0xC0)==0x40)  //V2.00�Ŀ�
    {
        csize = csd[9] + ((u16)csd[8] << 8) + 1;
        Capacity = (u32)csize << 10;//�õ�������
    }else//V1.XX�Ŀ�
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
        Capacity= (u32)csize << (n - 9);//�õ�������
    }
    return Capacity;
}

//========================================================================
// ����: ��ʼ��SD��.
// ����: none
// ����: 0�� ȡ��������; ����:SD��������(������/512�ֽ�).
//========================================================================
uint8_t SDCARD::begin(void)
{
    uint8_t r1;      // ���SD���ķ���ֵ
    uint16_t retry;  // �������г�ʱ����
    uint8_t buf[4];
    uint16_t i;

    pinMode(_ss_pin, GPIO_Mode_Out_PP);
    hardspi.begin();
    //hardspi.set_speed(0);
    digitalWrite(_ss_pin, 1);  

    for(i=0;i<10;i++)wr_byte(0XFF);//��������74������
    retry=20;
    do
    {
        r1 = send_cmd(CMD0,0,0x95);//����IDLE״̬
    }while((r1!=0X01) && retry--);
    SD_Type=0;//Ĭ���޿�
    if(r1 == 0X01)
    {
        if(send_cmd(CMD8,0x1AA,0x87)==1)//SD V2.0
        {
            for(i=0;i<4;i++) //Get trailing return value of R7 resp
            {
                buf[i]=wr_byte(0XFF);
            }
            if(buf[2]==0X01 && buf[3]==0XAA)//���Ƿ�֧��2.7~3.6V
            {
                retry = 0XFFFE;
                do
                {
                    send_cmd(CMD55,0,0X01);   //����CMD55
                    r1 = send_cmd(CMD41,0x40000000,0X01);//����CMD41
                }while(r1&&retry--);
                if(retry&&send_cmd(CMD58,0,0X01)==0)//����SD2.0���汾��ʼ
                {
                    for(i=0;i<4;i++)buf[i]=wr_byte(0XFF);//�õ�OCRֵ
                    if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //���CCS
                    else SD_Type=SD_TYPE_V2;
                }
            }
        }else//SD V1.x/ MMC V3
        {
            send_cmd(CMD55,0,0X01);       //����CMD55
            r1 = send_cmd(CMD41,0,0X01);    //����CMD41
            if(r1<=1)
            {
                SD_Type=SD_TYPE_V1;
                retry=0XFFFE;
                do //�ȴ��˳�IDLEģʽ
                {
                    send_cmd(CMD55,0,0X01);   //����CMD55
                    r1 = send_cmd(CMD41,0,0X01);//����CMD41
                }while(r1&&retry--);
            }else//MMC����֧��CMD55+CMD41ʶ��
            {
                SD_Type=SD_TYPE_MMC;//MMC V3
                retry=0XFFFE;
                do //�ȴ��˳�IDLEģʽ
                {
                    r1 = send_cmd(CMD1,0,0X01);//����CMD1
                }while(r1&&retry--);
            }
            if(retry==0||send_cmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;//����Ŀ�
        }
    }
    dis_select();//ȡ��Ƭѡ
    if(SD_Type)return 0;
    else if(r1)return r1;
    return 0xaa;//��������
}

//========================================================================
// ����: ��SD��.
// ����: buf:���ݻ�����
//       sector:����
//       cnt:������
// ����: 0,ok;����,ʧ��.
//========================================================================
uint8_t SDCARD::read_disk(uint8_t *buf,uint32_t sector,uint8_t cnt)
{
    uint8_t r1;
    if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;//ת��Ϊ�ֽڵ�ַ
    if(cnt == 1)
    {
        r1 = send_cmd(CMD17,sector,0X01);//������
        if(r1==0)//ָ��ͳɹ�
        {
            r1 = recv_data(buf,512);//����512���ֽ�
        }
    }else
    {
        r1 = send_cmd(CMD18,sector,0X01);//����������
        do
        {
            r1 = recv_data(buf,512);//����512���ֽ�
            buf+=512;
        }while(--cnt && r1==0);
        send_cmd(CMD12,0,0X01);   //����ֹͣ����
    }
    dis_select();//ȡ��Ƭѡ
    return r1;//
}

//========================================================================
// ����: дSD��.
// ����: buf:���ݻ�����
//       sector:��ʼ����
//       cnt:������
// ����: 0,ok;����,ʧ��.
//========================================================================
uint8_t SDCARD::write_disk(uint8_t *buf,uint32_t sector,uint8_t cnt)
{
    uint8_t r1;
    if(SD_Type!=SD_TYPE_V2HC)sector *= 512;//ת��Ϊ�ֽڵ�ַ
    if(cnt == 1)
    {
        r1 = send_cmd(CMD24,sector,0X01);//д����
        if(r1 == 0)//ָ��ͳɹ�
        {
            r1 = send_block(buf,0xFE);//д512���ֽڣ�0xFE��ʾ��ʼд
        }
    }else
    {
        if(SD_Type!=SD_TYPE_MMC)
        {
            send_cmd(CMD55,0,0X01);
            send_cmd(CMD23,cnt,0X01);//����ָ��
        }
        r1 = send_cmd(CMD25,sector,0X01);//����д����
        if(r1==0)
        {
            do
            {
                r1 = send_block(buf,0xFC);//0xFC��ʾ�������д�뿪ʼ
                buf+=512;
            }while(--cnt && r1==0);
            r1 = send_block(0,0xFD);//0xFD��ʾ�������д�����
        }
    }
    dis_select();//ȡ��Ƭѡ
    return r1;
}

SDCARD sd(PC12);

#endif
