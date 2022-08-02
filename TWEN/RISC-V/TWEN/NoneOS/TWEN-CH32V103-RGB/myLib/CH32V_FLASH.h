/**********************************************************
 * �ÿ�ΪCH32V103���ڲ�flash��.
 * *******************************************************/
#ifndef _CH32V_FLASH_H
#define _CH32V_FLASH_H

#include "CH32V103.h"
#include "string.h"

/* Global define */
#define FLASH_SIZE             (128*512)
#define FLASH_PAGE_SIZE        128
#define FLASH_START_ADDR       ((uint32_t)0x08000000)
#define FLASH_END_ADDR         ((uint32_t)0x08010000)

#define FLASH_PAGES_TO_BE_PROTECTED  FLASH_WRProt_Pages60to63

class INTERNAL_FLASH {
public:
	INTERNAL_FLASH(void);
	~INTERNAL_FLASH(void);
		
	FLASH_Status erase_page(uint32_t page);
	FLASH_Status FLASH_erase_page_all(void);  //����flashȫ������

	int8_t read(uint32_t data_addr, uint8_t *buf, uint16_t num_bytes);
	int8_t readChar(uint32_t data_addr);
	uint8_t readUChar(uint32_t data_addr);
	int16_t readShort(uint32_t data_addr);
	uint16_t readUShort(uint32_t data_addr);
	int32_t readInt(uint32_t data_addr);
	uint32_t readUInt(uint32_t data_addr);
	int64_t readLong64(uint32_t data_addr);
	uint64_t readULong64(uint32_t data_addr);
	float readFloat(uint32_t data_addr);
	double readDouble(uint32_t data_addr);
	template <class T> T readALL (uint32_t address, T &);

	void write(uint32_t data_addr, uint8_t *buf, uint16_t num_bytes);
	FLASH_Status writeChar(uint32_t data_addr,char *Data,uint32_t count);
	FLASH_Status writeUChar(uint32_t data_addr,uint8_t *Data,uint32_t count);
	FLASH_Status writeShort(uint32_t data_addr,int16_t *Data,uint32_t count);
	FLASH_Status writeUShort(uint32_t data_addr,uint16_t *Data,uint32_t count);
	FLASH_Status writeInt(uint32_t data_addr,int32_t *Data,uint32_t count);
	FLASH_Status writeUInt(uint32_t data_addr,uint32_t *Data,uint32_t count);
	FLASH_Status writeLong64(uint32_t data_addr,int64_t *Data,uint32_t count);
	FLASH_Status writeULong64(uint32_t data_addr,uint64_t *Data,uint32_t count);
	FLASH_Status writeFloat(uint32_t data_addr,float *Data,uint32_t count);
	FLASH_Status writeDouble(uint32_t data_addr,double *Data,uint32_t count);

	FLASH_Status write_half_word(uint32_t data_addr, uint16_t buf);
	void write_128bytes(uint32_t data_addr, uint8_t *dat);
protected:		
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
};

INTERNAL_FLASH::INTERNAL_FLASH(void)
{
}

INTERNAL_FLASH::~INTERNAL_FLASH() 
{
}

//========================================================================
// ����: flash��Ƭ����.
// ����: none
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::FLASH_erase_page_all(void)
{
	uint32_t WRPR_Value;
    FLASH_Unlock();
    WRPR_Value = FLASH_GetWriteProtectionOptionByte();    //д����
    if ( (WRPR_Value & FLASH_PAGES_TO_BE_PROTECTED) != 0x00){
        FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP|FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);
        FLASHStatus = FLASH_EraseAllPages();  //��Ƭ����
    }
    FLASH_Lock();
    return FLASHStatus;
}

//========================================================================
// ����: flashҳ����.
// ����: page��������һҳ��CH32V103R8T6һ��512ҳ(0-511)��ÿҳ128byte��
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::erase_page(uint32_t page)
{
	if(page < 64)return FLASH_ERROR_PG;
    FLASH_Unlock_Fast();
    FLASH_ErasePage_Fast(FLASH_START_ADDR + (FLASH_PAGE_SIZE * page));  //����ҳ
    FLASH_Lock_Fast();
    return FLASH_COMPLETE;
}

//========================================================================
// ����: flash��ָ����ַ����ָ�����ȵ�����.
// ����: data_addr:��ַ; 
// ����: none.
//========================================================================
int8_t INTERNAL_FLASH::read(uint32_t data_addr, uint8_t *buf, uint16_t num_bytes)
{
	// memcpy((void*)buf,(const void*)(data_addr),(size_t)num_bytes);
	for(uint32_t i = 0; i < num_bytes;i++)
	{
		buf[i] = (*(__IO uint8_t*)(data_addr+i));
	}
	return 1;
}

//========================================================================
// ����: flash��ȡ�з���8λ����.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
int8_t INTERNAL_FLASH::readChar(uint32_t data_addr)
{
	int8_t value = 0;
	return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡ�޷���8λ����.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
uint8_t INTERNAL_FLASH::readUChar(uint32_t data_addr)
{
	uint8_t value = 0;
	return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡ�з���16λ����.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
int16_t INTERNAL_FLASH::readShort(uint32_t data_addr)
{
	int16_t value = 0;
	return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡ�޷���16λ����.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
uint16_t INTERNAL_FLASH::readUShort(uint32_t data_addr)
{
    uint16_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡ�з���32λ����.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
int32_t INTERNAL_FLASH::readInt(uint32_t data_addr)
{
    int32_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡ�޷���32λ����.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
uint32_t INTERNAL_FLASH::readUInt(uint32_t data_addr)
{
    uint32_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡ�з���64λ����.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
int64_t INTERNAL_FLASH::readLong64(uint32_t data_addr)
{
    int64_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡ�޷���64λ����.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
uint64_t INTERNAL_FLASH::readULong64(uint32_t data_addr)
{
    uint64_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡfloat����С��.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
float INTERNAL_FLASH::readFloat(uint32_t data_addr)
{
    float value=0;
    return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡdouble����С��.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
double INTERNAL_FLASH::readDouble(uint32_t data_addr)
{
    double value = 0;
    return readALL(data_addr,value);
}

//========================================================================
// ����: flash��ȡģ��.
// ����: data_addr:��ַ
// ����: none.
//========================================================================
template <class T> T INTERNAL_FLASH::readALL(uint32_t data_addr,T &value)
{
    if( (data_addr-FLASH_START_ADDR + sizeof(T)) > FLASH_SIZE )
        return value;
    memcpy((uint8_t*) &value,(uint8_t*)(data_addr),sizeof(T));
    return value;
}

//========================================================================
// ����: flashд��һ����������.
// ����: data_addr:��ַ
//       dat:��������
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::write_half_word(uint32_t data_addr, uint16_t buf)
{
    FLASH_Unlock();
    FLASHStatus = FLASH_ProgramHalfWord(data_addr, buf);
    FLASH_Lock();
    return FLASHStatus;
}

//========================================================================
// ����: flash����д��128�ֽ�����.
// ����: data_addr:��ַ
//       buf:����ָ�루128�ֽڣ�
// ����: none.
//========================================================================
void INTERNAL_FLASH::write_128bytes(uint32_t data_addr, uint8_t *buf)
{
	uint32_t page_buf[32];
	uint32_t data_32 = 0;
	for (int i = 0; i < 32; i++)
  	{
		data_32 = buf[i * 4 + 3];
		data_32 <<= 8;
		data_32 |= buf[i * 4 + 2];
		data_32 <<= 8;
		data_32 |= buf[i * 4 + 1];
		data_32 <<= 8;
		data_32 |= buf[i * 4];
		page_buf[i] = data_32;
  	}
    FLASH_Unlock_Fast();
    FLASH_BufReset();
	FLASH_BufLoad(data_addr, page_buf[0], page_buf[1], page_buf[2], page_buf[3]);
	FLASH_BufLoad(data_addr + 0x10, page_buf[4], page_buf[5], page_buf[6], page_buf[7]);
	FLASH_BufLoad(data_addr + 0x20, page_buf[8], page_buf[9], page_buf[10], page_buf[11]);
	FLASH_BufLoad(data_addr + 0x30, page_buf[12], page_buf[13], page_buf[14], page_buf[15]);
	FLASH_BufLoad(data_addr + 0x40, page_buf[16], page_buf[17], page_buf[18], page_buf[19]);
	FLASH_BufLoad(data_addr + 0x50, page_buf[20], page_buf[21], page_buf[22], page_buf[23]);
	FLASH_BufLoad(data_addr + 0x60, page_buf[24], page_buf[25], page_buf[26], page_buf[27]);
	FLASH_BufLoad(data_addr + 0x70, page_buf[28], page_buf[29], page_buf[30], page_buf[31]);
    FLASH_ProgramPage_Fast(data_addr);
    FLASH_Lock_Fast();
}

//========================================================================
// ����: flash��ָ����ַд��ָ�����ȵ�����.
// ����: data_addr:��ַ
//       buf:���ݻ�����
//		 num_bytes��Ҫд����ֽ���
// ����: none.
//========================================================================
void INTERNAL_FLASH::write(uint32_t data_addr, uint8_t *buf, uint16_t num_bytes)
{
	uint32_t secpos;	//ҳ��ַ
	uint16_t secoff;  //��ҳ��ַ��ƫ��
	uint16_t secremain;	   
 	uint16_t i;
	uint8_t flash_buf[FLASH_PAGE_SIZE];   
	data_addr =  data_addr - FLASH_START_ADDR;
	secpos = data_addr/FLASH_PAGE_SIZE;  //ҳ��ַ
	secoff = data_addr%FLASH_PAGE_SIZE;  //��ҳ��ַ�ڵ�ƫ��
	secremain = FLASH_PAGE_SIZE-secoff;//��ҳ��ʣ��ռ��С   
	if(num_bytes <= secremain)	
	{
		secremain = num_bytes;
	}
	while(1)
	{
		read(FLASH_START_ADDR+secpos*FLASH_PAGE_SIZE, flash_buf, FLASH_PAGE_SIZE);	//������������������
		for(i=0;i<secremain;i++)
		{
			if(flash_buf[secoff+i]!=0XFF){
				erase_page(secpos);	//������ҳ
				break;
			}
		}
		for(i=0;i<secremain;i++)	    //����
		{
			flash_buf[secoff+i] = buf[i];	  
		}
		write_128bytes(FLASH_START_ADDR+secpos*FLASH_PAGE_SIZE, flash_buf);
		if(num_bytes == secremain)break;	//ȫ��д�����
		else{
			secpos++;	//ҳ��ַ��1
			secoff = 0;	//ƫ������
		   	buf+=secremain;  //Ҫд������ݵ�ַƫ��
			data_addr+=secremain;//flash��ַƫ��	  
			num_bytes-=secremain;	//Ҫд����ֽ����ݼ�
			if(num_bytes>128)secremain = 128;	//��һҳ����д����
			else secremain = num_bytes;	  //��һ����������д����
		}				
	}
}

//========================================================================
// ����: flashд���з���8λ����.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeChar(uint32_t data_addr,char *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint8_t) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count);
	return FLASH_COMPLETE;
}

//========================================================================
// ����: flashд���޷���8λ����.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeUChar(uint32_t data_addr,uint8_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint8_t) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count);
	return FLASH_COMPLETE;
}

//========================================================================
// ����: flashд���з���16λ����.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeShort(uint32_t data_addr,int16_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(int16_t) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(int16_t));
	return FLASH_COMPLETE;
}

//========================================================================
// ����: flashд���޷���16λ����.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeUShort(uint32_t data_addr,uint16_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint16_t) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(uint16_t));
	return FLASH_COMPLETE;
}

//========================================================================
// ����: flashд���з���32λ����.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeInt(uint32_t data_addr,int32_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(int32_t) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(int32_t));
	return FLASH_COMPLETE;
}

//========================================================================
// ����: flashд���޷���32λ����.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeUInt(uint32_t data_addr,uint32_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint32_t) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(uint32_t));
	return FLASH_COMPLETE;
}

//========================================================================
// ����: flashд���з���64λ����.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeLong64(uint32_t data_addr,int64_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(int64_t) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(int64_t));
	return FLASH_COMPLETE;
}

//========================================================================
// ����: flashд���޷���64λ����.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeULong64(uint32_t data_addr,uint64_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint64_t) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(uint64_t));
	return FLASH_COMPLETE;
}

//========================================================================
// ����: flashд��float��������.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeFloat(uint32_t data_addr,float *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(float) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(float));
	return FLASH_COMPLETE;
}

//========================================================================
// ����: flashд��double��������.
// ����: data_addr:Ҫд���flash��ַ
//       Data:Ҫд�������ָ��
//       count:Ҫд������ݳ���  
// ����: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeDouble(uint32_t data_addr,double *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(double) > FLASH_END_ADDR) 	//����flash��Χ
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(double));
	return FLASH_COMPLETE;
}

#endif









