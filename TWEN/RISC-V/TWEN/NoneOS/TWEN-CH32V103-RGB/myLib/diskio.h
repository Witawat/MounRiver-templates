/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2014          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

#define _USE_WRITE  1   /* 1: Enable disk_write function */
#define _USE_IOCTL  1   /* 1: Enable disk_ioctl fucntion */

#include "integer.h"
#include "CH32V_SDCARD.h"
#include "malloc.h"

/* Status of Disk Functions */
typedef UCHAR    DSTATUS;

/* Results of Disk Functions */
typedef enum {
    RES_OK = 0,     /* 0: Successful */
    RES_ERROR,      /* 1: R/W Error */
    RES_WRPRT,      /* 2: Write Protected */
    RES_NOTRDY,     /* 3: Not Ready */
    RES_PARERR      /* 4: Invalid Parameter */
} DRESULT;

/*---------------------------------------*/
/* Prototypes for disk control functions */

DSTATUS disk_initialize (UCHAR pdrv);
DSTATUS disk_status (UCHAR pdrv);
DRESULT disk_read (UCHAR pdrv, UCHAR* buff, DWORD sector, UINT count);
DRESULT disk_write (UCHAR pdrv, const UCHAR* buff, DWORD sector, UINT count);
DRESULT disk_ioctl (UCHAR pdrv, UCHAR cmd, void* buff);

/* Disk Status Bits (DSTATUS) */
#define STA_NOINIT      0x01    /* Drive not initialized */
#define STA_NODISK      0x02    /* No medium in the drive */
#define STA_PROTECT     0x04    /* Write protected */

/* Command code for disk_ioctrl fucntion */
/* Generic command (Used by FatFs) */
#define CTRL_SYNC           0   /* Complete pending write process (needed at _FS_READONLY == 0) */
#define GET_SECTOR_COUNT    1   /* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_SIZE     2   /* Get sector size (needed at _MAX_SS != _MIN_SS) */
#define GET_BLOCK_SIZE      3   /* Get erase block size (needed at _USE_MKFS == 1) */
#define CTRL_TRIM           4   /* Inform device that the data on the block of sectors is no longer used (needed at _USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_POWER          5   /* Get/Set power status */
#define CTRL_LOCK           6   /* Lock/Unlock media removal */
#define CTRL_EJECT          7   /* Eject media */
#define CTRL_FORMAT         8   /* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE        10  /* Get card type */
#define MMC_GET_CSD         11  /* Get CSD */
#define MMC_GET_CID         12  /* Get CID */
#define MMC_GET_OCR         13  /* Get OCR */
#define MMC_GET_SDSTAT      14  /* Get SD status */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV         20  /* Get F/W revision */
#define ATA_GET_MODEL       21  /* Get model name */
#define ATA_GET_SN          22  /* Get serial number */

#ifdef __cplusplus
}
#endif

#define SD_CARD  0  //SD��,���Ϊ0
#define EX_FLASH 1  //�ⲿflash,���Ϊ1

#define FLASH_SECTOR_SIZE   512

//========================================================================
// ����: ��ʼ������.
// ����: pdrv:��ʼ���ĸ�����.
// ����: 0,��ʼ���ɹ�.
//========================================================================
DSTATUS disk_initialize (
    UCHAR pdrv               /* Physical drive nmuber (0..) */
)
{
    uint8_t res=0;
    switch(pdrv)
    {
        case SD_CARD://SD��
            res = sd.begin();  //SD����ʼ��
            if(res)     //STM32 SPI��bug,��sd������ʧ�ܵ�ʱ�������ִ����������,���ܵ���SPI��д�쳣
            {
                hardspi.wr_data(0xff);//�ṩ�����8��ʱ��
            }
            break;
        case EX_FLASH://�ⲿflash
            break;
        default:
            res=1;
    }
    if(res)return  STA_NOINIT;
    else return 0; //��ʼ���ɹ�
}

//========================================================================
// ����: ��ô���״̬.
// ����: pdrv:�ĸ�����.
// ����: 0.
//========================================================================
DSTATUS disk_status (
    UCHAR pdrv       /* Physical drive nmuber (0..) */
)
{
    return 0;
}

//========================================================================
// ����: ������.
// ����: pdrv:���̱��0~9
//       buff:���ݽ��ջ����׵�ַ
//       sector:������ַ
//       count:��Ҫ��ȡ��������
// ����: �ɹ�����RES_OK.
//========================================================================
DRESULT disk_read (
    UCHAR pdrv,      /* Physical drive nmuber (0..) */
    UCHAR *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address (LBA) */
    UINT count      /* Number of sectors to read (1..128) */
)
{
    u8 res=0;
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������
    switch(pdrv)
    {
        case SD_CARD://SD��
            res = sd.read_disk(buff,sector,count);
            if(res)//STM32 SPI��bug,��sd������ʧ�ܵ�ʱ�������ִ����������,���ܵ���SPI��д�쳣
            {
                hardspi.wr_data(0xff);//�ṩ�����8��ʱ��
            }
            break;
        case EX_FLASH://�ⲿflash
            res=0;
            break;
        default:
            res=1;
    }
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res==0x00)return RES_OK;
    else return RES_ERROR;
}

//========================================================================
// ����: д����.
// ����: pdrv:���̱��0~9
//       buff:���ݷ��ͻ����׵�ַ
//       sector:������ַ
//       count:��Ҫд���������
// ����: �ɹ�����RES_OK.
//========================================================================
#if _USE_WRITE
DRESULT disk_write (
    UCHAR pdrv,          /* Physical drive nmuber (0..) */
    const UCHAR *buff,   /* Data to be written */
    DWORD sector,       /* Sector address (LBA) */
    UINT count          /* Number of sectors to write (1..128) */
)
{
    u8 res=0;
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������
    switch(pdrv)
    {
        case SD_CARD://SD��
            res = sd.write_disk((u8*)buff,sector,count);
            break;
        case EX_FLASH://�ⲿflash
            res=0;
            break;
        default:
            res=1;
    }
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res == 0x00)return RES_OK;
    else return RES_ERROR;
}
#endif

//========================================================================
// ����: ����������Ļ��.
// ����: pdrv:���̱��0~9
//       cmd:���ƴ���
//       buff:����/���տ������ݻ���
// ����: �ɹ�����RES_OK.
//========================================================================
#if _USE_IOCTL
DRESULT disk_ioctl (
    UCHAR pdrv,      /* Physical drive nmuber (0..) */
    UCHAR cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;
    if(pdrv==SD_CARD)//SD��
    {
        switch(cmd)
        {
            case CTRL_SYNC:
                sd.select();      //??????????????
                if( sd.wait_ready() ==0 )res = RES_OK;
                else res = RES_ERROR;
                sd.dis_select();      //??????????????
                break;
            case GET_SECTOR_SIZE:
                *(WORD*)buff = 512;
                res = RES_OK;
                break;
            case GET_BLOCK_SIZE:
                *(WORD*)buff = 8;
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:
                *(DWORD*)buff = sd.get_sector_count();
                res = RES_OK;
                break;
            default:
                res = RES_PARERR;
                break;
        }
    }
    else res=RES_ERROR;//�����Ĳ�֧��
    return res;
}
#endif

//========================================================================
// ����: ��ȡʱ�䣨û��ʹ�ã�.
// ����: none.
// ����: none.
//========================================================================
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
DWORD get_fattime (void)
{
    return 0;
}

#endif
