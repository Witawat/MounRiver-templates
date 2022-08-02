/* 2014.09.09
*****************************************
**   Copyright  (C)  W.ch  1999-2019   **
**   Web:      http://wch.cn           **
*****************************************
**  USB-flash File Interface for CHRV3 **
**  KEIL423, gcc 8.20          **
*****************************************
*/
/* CHRV3 U�������ļ�ϵͳ�ӿ�, ֧��: FAT12/FAT16/FAT32 */

//#include "CHRV3BAS.H"

#ifndef __CHRV3UFI_H__
#define __CHRV3UFI_H__

#include "CH32V103.h"
#define CHRV3_LIB_VER		0x10

//#define DISK_BASE_BUF_LEN		512	/* Ĭ�ϵĴ������ݻ�������СΪ512�ֽ�(����ѡ��Ϊ2048����4096��֧��ĳЩ��������U��),Ϊ0���ֹ�ڱ��ļ��ж��建��������Ӧ�ó�����pDISK_BASE_BUF��ָ�� */
/* �����Ҫ���ô������ݻ������Խ�ԼRAM,��ô�ɽ�DISK_BASE_BUF_LEN����Ϊ0�Խ�ֹ�ڱ��ļ��ж��建����,����Ӧ�ó����ڵ���CHRV3LibInit֮ǰ��������������õĻ�������ʼ��ַ����pDISK_BASE_BUF���� */

//#define NO_DEFAULT_ACCESS_SECTOR	1		/* ��ֹĬ�ϵĴ���������д�ӳ���,���������б�д�ĳ�������� */
//#define NO_DEFAULT_DISK_CONNECT		1		/* ��ֹĬ�ϵļ����������ӳ���,���������б�д�ĳ�������� */
//#define NO_DEFAULT_FILE_ENUMER		1		/* ��ֹĬ�ϵ��ļ���ö�ٻص�����,���������б�д�ĳ�������� */

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************************************************************* */

/* FILE: CHRV3UF.H */

/* ������ */
#ifndef ERR_SUCCESS
#define ERR_SUCCESS				0x00	/* �����ɹ� */
#endif
#ifndef ERR_DISK_DISCON
#define ERR_CHRV3_ERROR			0x81	/* CHRV3Ӳ������,������Ҫ��λCHRV3 */
//#define ERR_DISK_DISCON			0x82	/* ������δ����,���ܴ����Ѿ��Ͽ� */
#define ERR_STATUS_ERR			0x83	/* ����״̬����,�����������ӻ��߶Ͽ����� */
#define ERR_HUB_PORT_FREE		0x84	/* USB-HUB�Ѿ����ӵ���HUB�˿���δ���Ӵ���,���ܴ����Ѿ��Ͽ� */
#define ERR_MBR_ERROR			0x91	/* ���̵���������¼��Ч,���ܴ�����δ����������δ��ʽ�� */
#define ERR_TYPE_ERROR			0x92	/* ���̷������Ͳ�֧��,ֻ֧��FAT12/FAT16/BigDOS/FAT32,��Ҫ�ɴ��̹��������·��� */
#define ERR_BPB_ERROR			0xA1	/* ������δ��ʽ��,���߲�������,��Ҫ��WINDOWS����Ĭ�ϲ������¸�ʽ�� */
#define ERR_TOO_LARGE			0xA2	/* ���̷�������ʽ��������������4GB,������������250GB,��Ҫ��WINDOWS����Ĭ�ϲ������¸�ʽ�� */
#define ERR_FAT_ERROR			0xA3	/* ���̵��ļ�ϵͳ��֧��,ֻ֧��FAT12/FAT16/FAT32,��Ҫ��WINDOWS����Ĭ�ϲ������¸�ʽ�� */
#define ERR_DISK_FULL			0xB1	/* �����ļ�̫��,ʣ��ռ�̫�ٻ����Ѿ�û��,��Ҫ�������� */
#define ERR_FDT_OVER			0xB2	/* Ŀ¼���ļ�̫��,û�п��е�Ŀ¼��,FAT12/FAT16��Ŀ¼�µ��ļ���Ӧ������500��,��Ҫ�������� */
#define ERR_MISS_DIR			0xB3	/* ָ��·����ĳ����Ŀ¼û���ҵ�,������Ŀ¼���ƴ��� */
#define ERR_FILE_CLOSE			0xB4	/* �ļ��Ѿ��ر�,�����Ҫʹ��,Ӧ�����´��ļ� */
#define ERR_OPEN_DIR			0x41	/* ָ��·����Ŀ¼���� */
#define ERR_MISS_FILE			0x42	/* ָ��·�����ļ�û���ҵ�,�������ļ����ƴ��� */
#define ERR_FOUND_NAME			0x43	/* ��������ͨ�����ƥ����ļ���,�ļ�����������·�������������,�����Ҫʹ��,Ӧ�ô򿪸��ļ� */
#endif
/* ����2XH-3XH����USB������ʽ��ͨѶʧ�ܴ���,��CHRV3�ӳ���ģ��CH375�ķ��� */
/* ����1XH����USB������ʽ�Ĳ���״̬����,��CHRV3�ӳ���ģ��CH375�ķ��� */
#ifndef ERR_USB_CONNECT
#define	ERR_USB_CONNECT_LS		0x13	/* ��⵽����USB�豸�����¼� */
#define	ERR_USB_CONNECT			0x15	/* ��⵽USB�豸�����¼�,�����Ѿ����� */
#define	ERR_USB_DISCON			0x16	/* ��⵽USB�豸�Ͽ��¼�,�����Ѿ��Ͽ� */
#define	ERR_USB_BUF_OVER		0x17	/* USB��������������������̫�໺������� */
#define	ERR_USB_DISK_ERR		0x1F	/* USB�洢������ʧ��,�ڳ�ʼ��ʱ������USB�洢����֧��,�ڶ�д�����п����Ǵ����𻵻����Ѿ��Ͽ� */
#define	ERR_USB_TRANSFER		0x20	/* NAK/STALL�ȸ����������0x20~0x2F */
#endif

/* ���̼��ļ�״̬ */
#define DISK_UNKNOWN			0x00	/* ��δ��ʼ��,δ֪״̬ */
#define DISK_DISCONNECT			0x01	/* ����û�����ӻ����Ѿ��Ͽ� */
#define DISK_CONNECT			0x02	/* �����Ѿ�����,������δ��ʼ�������޷�ʶ��ô��� */
#define DISK_USB_ADDR			0x04	/* �����Ѿ�����USB�豸��ַ,������δ����USB�ͳ�ʼ������ */
#define DISK_MOUNTED			0x05	/* �����Ѿ���ʼ���ɹ�,������δ�����ļ�ϵͳ�����ļ�ϵͳ��֧�� */
#define DISK_READY				0x10	/* �Ѿ��������̵��ļ�ϵͳ�����ܹ�֧�� */
#define DISK_OPEN_ROOT			0x12	/* �Ѿ��򿪸�Ŀ¼,����ģʽ,ֻ��������Ϊ��λ��дĿ¼������,ʹ�ú����ر�,ע��FAT12/FAT16��Ŀ¼�ǹ̶����� */
#define DISK_OPEN_DIR			0x13	/* �Ѿ�����Ŀ¼,����ģʽ,ֻ��������Ϊ��λ��дĿ¼������ */
#define DISK_OPEN_FILE			0x14	/* �Ѿ����ļ�,����ģʽ,����������Ϊ��λ�������ݶ�д */
#define DISK_OPEN_FILE_B		0x15	/* �Ѿ����ļ�,�ֽ�ģʽ,�������ֽ�Ϊ��λ�������ݶ�д */

/* FAT���ͱ�־ */
#ifndef DISK_FAT16
#define DISK_FS_UNKNOWN			0		/* δ֪���ļ�ϵͳ */
#define DISK_FAT12				1		/* FAT12�ļ�ϵͳ */
#define DISK_FAT16				2		/* FAT16�ļ�ϵͳ */
#define DISK_FAT32				3		/* FAT32�ļ�ϵͳ */
#endif
void Delay_Us( UINT16 n )
{
	delayMicroseconds(n);
}
void Delay_Ms( UINT16 n )
{
	delay(n);
}
/* FAT���������ļ�Ŀ¼��Ϣ */
typedef struct _FAT_DIR_INFO {
	UINT8	DIR_Name[11];				/* 00H,�ļ���,��11�ֽ�,���㴦��ո� */
	UINT8	DIR_Attr;					/* 0BH,�ļ�����,�ο������˵�� */
	UINT8	DIR_NTRes;					/* 0CH */
	UINT8	DIR_CrtTimeTenth;			/* 0DH,�ļ�������ʱ��,��0.1�뵥λ���� */
	UINT16	DIR_CrtTime;				/* 0EH,�ļ�������ʱ�� */
	UINT16	DIR_CrtDate;				/* 10H,�ļ����������� */
	UINT16	DIR_LstAccDate;				/* 12H,���һ�δ�ȡ���������� */
	UINT16	DIR_FstClusHI;				/* 14H */
	UINT16	DIR_WrtTime;				/* 16H,�ļ��޸�ʱ��,�ο���MAKE_FILE_TIME */
	UINT16	DIR_WrtDate;				/* 18H,�ļ��޸�����,�ο���MAKE_FILE_DATA */
	UINT16	DIR_FstClusLO;				/* 1AH */
	UINT32	DIR_FileSize;				/* 1CH,�ļ����� */
} FAT_DIR_INFO;							/* 20H */

typedef FAT_DIR_INFO *PX_FAT_DIR_INFO;

/* �ļ����� */
#define ATTR_READ_ONLY			0x01	/* �ļ�Ϊֻ������ */
#define ATTR_HIDDEN				0x02	/* �ļ�Ϊ�������� */
#define ATTR_SYSTEM				0x04	/* �ļ�Ϊϵͳ���� */
#define ATTR_VOLUME_ID			0x08	/* ��� */
#define ATTR_DIRECTORY			0x10	/* ��Ŀ¼ */
#define ATTR_ARCHIVE			0x20	/* �ļ�Ϊ�浵���� */
#define ATTR_LONG_NAME			( ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID )
/* �ļ����� UINT8 */
/* bit0 bit1 bit2 bit3 bit4 bit5 bit6 bit7 */
/*  ֻ   ��   ϵ   ��   Ŀ   ��   δ����   */
/*  ��   ��   ͳ   ��   ¼   ��            */
/* �ļ�ʱ�� UINT16 */
/* Time = (Hour<<11) + (Minute<<5) + (Second>>1) */
#define MAKE_FILE_TIME( h, m, s )	( (h<<11) + (m<<5) + (s>>1) )	/* ����ָ��ʱ������ļ�ʱ������ */
/* �ļ����� UINT16 */
/* Date = ((Year-1980)<<9) + (Month<<5) + Day */
#define MAKE_FILE_DATE( y, m, d )	( ((y-1980)<<9) + (m<<5) + d )	/* ����ָ�������յ��ļ��������� */

/* �ļ��� */
#define PATH_WILDCARD_CHAR		0x2A	/* ·������ͨ��� '*' */
#define PATH_SEPAR_CHAR1		0x5C	/* ·�����ķָ��� '\' */
#define PATH_SEPAR_CHAR2		0x2F	/* ·�����ķָ��� '/' */
#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN			64		/* ���·������,������б�ָܷ�����С���������Լ�·��������00H */
#endif

/* �ⲿ������� */
typedef union _CMD_PARAM {
	struct {
		UINT8	mBuffer[ MAX_PATH_LEN ];
	} Other;
	struct {
		UINT32	mTotalSector;			/* ����: ��ǰ�߼��̵��������� */
		UINT32	mFreeSector;			/* ����: ��ǰ�߼��̵�ʣ�������� */
		UINT32	mSaveValue;
	} Query;							/* CMD_DiskQuery, ��ѯ������Ϣ */
	struct {
		UINT8	mPathName[ MAX_PATH_LEN ];	/* �������: ·��: [�̷�,ð��,б��,Ŀ¼�������ļ�������չ��...,������00H], �����̷���ð�ſ���ʡ��, ����"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} Open;								/* CMD_FileOpen, ���ļ� */
//	struct {
//		UINT8	mPathName[ MAX_PATH_LEN ];	/* �������: ·��: [�̷�,ð��,б��,Ŀ¼�������ļ�������չ��(��ͨ���*)...,������00H], �����̷���ð�ſ���ʡ��, ����"C:\DIR1.EXT\DIR2\FILE*",00H */
//	} Open;								/* CMD_FileOpen, ö���ļ�, CHRV3vFileSize���λΪ1�������xFileNameEnumer,Ϊ0�򷵻�ָ����ŵ��ļ��� */
	struct {
		UINT8	mUpdateLen;				/* �������: �Ƿ�������³���: 0��ֹ,1���� */
	} Close;							/* CMD_FileClose, �رյ�ǰ�ļ� */
	struct {
		UINT8	mPathName[ MAX_PATH_LEN ];	/* �������: ·��: [�̷�,ð��,б��,Ŀ¼�������ļ�������չ��...,������00H], �����̷���ð�ſ���ʡ��, ����"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} Create;							/* CMD_FileCreate, �½��ļ�����,����ļ��Ѿ���������ɾ�������½� */
	struct {
		UINT8	mPathName[ MAX_PATH_LEN ];	/* �������: ·��: [�̷�,ð��,б��,Ŀ¼�������ļ�������չ��...,������00H], �����̷���ð�ſ���ʡ��, ����"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} Erase;							/* CMD_FileErase, ɾ���ļ����ر� */
	struct {
		UINT32	mFileSize;				/* �������: �µ��ļ�����,Ϊ0FFFFFFFFH���޸�, ����: ԭ���� */
		UINT16	mFileDate;				/* �������: �µ��ļ�����,Ϊ0FFFFH���޸�, ����: ԭ���� */
		UINT16	mFileTime;				/* �������: �µ��ļ�ʱ��,Ϊ0FFFFH���޸�, ����: ԭʱ�� */
		UINT8	mFileAttr;				/* �������: �µ��ļ�����,Ϊ0FFH���޸�, ����: ԭ���� */
	} Modify;							/* CMD_FileQuery, ��ѯ��ǰ�ļ�����Ϣ; CMD_FileModify, ��ѯ�����޸ĵ�ǰ�ļ�����Ϣ */
	struct {
		UINT32	mSaveCurrClus;
		UINT32	mSaveLastClus;
	} Alloc;							/* CMD_FileAlloc, �����ļ����ȵ���Ϊ�ļ�����Ĵ��̿ռ� */
	struct {
		UINT32	mSectorOffset;			/* �������: ����ƫ��,0���ƶ����ļ�ͷ,0FFFFFFFFH���ƶ����ļ�β, ����: ��ǰ�ļ�ָ���Ӧ�ľ�������������, 0FFFFFFFFH���ѵ��ļ�β */
		UINT32	mLastOffset;
	} Locate;							/* CMD_FileLocate, �ƶ���ǰ�ļ�ָ�� */
	struct {
		UINT8	mSectorCount;			/* �������: ��ȡ������, ����: ʵ�ʶ�ȡ������ */
		UINT8	mActCnt;
		UINT8	mLbaCount;
		UINT8	mRemainCnt;
		PUINT8	mDataBuffer;			/* �������: ��������ʼ��ַ, ����: ��������ǰ��ַ */
		UINT32	mLbaStart;
	} Read;								/* CMD_FileRead, �ӵ�ǰ�ļ���ȡ���� */
	struct {
		UINT8	mSectorCount;			/* �������: д��������, ����: ʵ��д�������� */
		UINT8	mActCnt;
		UINT8	mLbaCount;
		UINT8	mAllocCnt;
		PUINT8	mDataBuffer;			/* �������: ��������ʼ��ַ, ����: ��������ǰ��ַ */
		UINT32	mLbaStart;
		UINT32	mSaveValue;
	} Write;							/* CMD_FileWrite, ��ǰ�ļ�д������ */
	struct {
		UINT32	mDiskSizeSec;			/* ����: ����������̵���������, ���״ε���ʱ���� */
	} DiskReady;						/* CMD_DiskReady, ��ѯ���̾��� */
	struct {
		UINT32	mByteOffset;			/* �������: ���ֽ�Ϊ��λ��ƫ����, ���ֽ�Ϊ��λ���ļ�ָ��, ����: ��ǰ�ļ�ָ���Ӧ�ľ�������������, 0FFFFFFFFH���ѵ��ļ�β */
		UINT32	mLastOffset;
	} ByteLocate;						/* CMD_ByteLocate, ���ֽ�Ϊ��λ�ƶ���ǰ�ļ�ָ�� */
	struct {
		UINT16	mByteCount;				/* �������: ׼����ȡ���ֽ���, ����: ʵ�ʶ������ֽ��� */
		PUINT8	mByteBuffer;			/* �������: ָ���Ŷ������ݿ�Ļ����� */
		UINT16	mActCnt;
	} ByteRead;							/* CMD_ByteRead, ���ֽ�Ϊ��λ�ӵ�ǰ�ļ���ȡ���ݿ� */
	struct {
		UINT16	mByteCount;				/* �������: ׼��д����ֽ���, ����: ʵ��д����ֽ��� */
		PUINT8	mByteBuffer;			/* �������: ָ���Ŷ������ݿ�Ļ����� */
		UINT16	mActCnt;
	} ByteWrite;						/* CMD_ByteWrite, ���ֽ�Ϊ��λ��ǰ�ļ�д�����ݿ� */
	struct {
		UINT8	mSaveVariable;			/* �������: Ϊ0��ָ�����U�̵ı���,Ϊ0x80��ָ����U�̵ı���,����ֵ�򱸷�/������� */
		UINT8	mReserved[3];
		PUINT8	mBuffer;				/* �������: ָ���ӳ����ı����ı��ݻ�����,���Ȳ�С��80���ֽ� */
	} SaveVariable;						/* CMD_SaveVariable, ����/����/�ָ��ӳ����ı��� */
} CMD_PARAM;

typedef CMD_PARAM CMD_PARAM_I;
//typedef CMD_PARAM *P_CMD_PARAM;

/* SCSI������ */
#ifndef SPC_CMD_INQUIRY
#define SPC_CMD_INQUIRY			0x12
#define SPC_CMD_READ_CAPACITY	0x25
#define SPC_CMD_READ10			0x28
#define SPC_CMD_WRITE10			0x2A
#define SPC_CMD_TEST_READY		0x00
#define SPC_CMD_REQUEST_SENSE	0x03
#define SPC_CMD_MODESENSE6		0x1A
#define SPC_CMD_MODESENSE10		0x5A
#define SPC_CMD_START_STOP		0x1B
#endif

/* FILE: CHRV3UFI.C */

#define EN_DISK_WRITE			1

#ifndef DISK_BASE_BUF_LEN
#define DISK_BASE_BUF_LEN		512		/* Ĭ�ϵĴ������ݻ�������СΪ512�ֽ�,����ѡ��Ϊ2048����4096��֧��ĳЩ��������U��,Ϊ0���ֹ��.H�ļ��ж��建��������Ӧ�ó�����pDISK_BASE_BUF��ָ�� */
#endif

/* �ӳ�������ṩ�ı��� */
extern	UINT8V	CHRV3IntStatus;				/* CHRV3�������ж�״̬ */
extern	UINT8V	CHRV3DiskStatus;			/* ���̼��ļ�״̬ */
extern	UINT8	CHRV3vDiskFat;				/* �߼��̵�FAT��־:1=FAT12,2=FAT16,3=FAT32 */
extern	UINT8	CHRV3vSecPerClus;			/* �߼��̵�ÿ�������� */
extern	UINT8	CHRV3vSectorSizeB;			/* log2(CHRV3vSectorSize) */
extern	UINT32	CHRV3vStartLba;				/* �߼��̵���ʼ����������LBA */
extern	UINT32	CHRV3vDiskRoot;				/* ����FAT16��Ϊ��Ŀ¼ռ��������,����FAT32��Ϊ��Ŀ¼��ʼ�غ� */
extern	UINT32	CHRV3vDataStart;			/* �߼��̵������������ʼLBA */
extern	UINT32	CHRV3vStartCluster;			/* ��ǰ�ļ�����Ŀ¼����ʼ�غ� */
extern	UINT32	CHRV3vFileSize;				/* ��ǰ�ļ��ĳ��� */
extern	UINT32	CHRV3vCurrentOffset;		/* ��ǰ�ļ�ָ��,��ǰ��дλ�õ��ֽ�ƫ�� */
extern	UINT32	CHRV3vFdtLba;				/* ��ǰFDT���ڵ�LBA��ַ */
extern	UINT32	CHRV3vLbaCurrent;			/* ��ǰ��д�Ĵ�����ʼLBA��ַ */
extern	UINT16	CHRV3vFdtOffset;			/* ��ǰFDT�������ڵ�ƫ�Ƶ�ַ */
extern	UINT16	CHRV3vSectorSize;			/* ���̵�������С */
extern	UINT8	CHRV3vCurrentLun;			/* ���̵�ǰ�����߼���Ԫ�� */
extern	BOOL	CHRV3vSubClassIs6;			/* USB�洢���豸������Ϊ6,0���6 */
extern	PUINT8	pDISK_BASE_BUF;		/* ָ���ⲿRAM�Ĵ������ݻ�����,���������Ȳ�С��CHRV3vSectorSize,��Ӧ�ó����ʼ�� */
extern	PUINT8	pDISK_FAT_BUF;		/* ָ���ⲿRAM�Ĵ���FAT���ݻ�����,���������Ȳ�С��CHRV3vSectorSize,��Ӧ�ó����ʼ�� */
extern	UINT16	CHRV3vPacketSize;			/* USB�洢���豸����������:64@FS,512@HS/SS,��Ӧ�ó����ʼ�� */
extern	PUINT32	pTX_DMA_A_REG;				/* ָ����DMA��ַ�Ĵ���,��Ӧ�ó����ʼ�� */
extern	PUINT32	pRX_DMA_A_REG;				/* ָ�����DMA��ַ�Ĵ���,��Ӧ�ó����ʼ�� */
extern	PUINT16	pTX_LEN_REG;				/* ָ���ͳ��ȼĴ���,��Ӧ�ó����ʼ�� */
extern	PUINT16	pRX_LEN_REG;				/* ָ����ճ��ȼĴ���,��Ӧ�ó����ʼ�� */

extern	CMD_PARAM_I	mCmdParam;				/* ������� */

extern	UINT8	RxBuffer[ MAX_PACKET_SIZE ];  // IN, must even address
extern	UINT8	TxBuffer[ MAX_PACKET_SIZE ];  // OUT, must even address

//#define		PXUDISK_BOC_CBW	PUDISK_BOC_CBW
//#define		PXUDISK_BOC_CSW	PUDISK_BOC_CSW

#ifndef	pSetupReq
#define	pSetupReq	((PUSB_SETUP_REQ)TxBuffer)
#endif
#ifndef	pCBW
#define	pCBW		((PXUDISK_BOC_CBW)TxBuffer)
#define	pCSW		((PXUDISK_BOC_CSW)RxBuffer)
#endif
#ifndef	pBOC_buf
#define	pBOC_buf	(TxBuffer+((USB_BO_CBW_SIZE+4)&0xFE))
#endif

#if		DISK_BASE_BUF_LEN > 0
extern	UINT8	DISK_BASE_BUF[ DISK_BASE_BUF_LEN ];	/* �ⲿRAM�Ĵ������ݻ�����,����������Ϊһ�������ĳ��� */
#endif
extern	UINT8	CHRV3ReadSector( UINT8 SectCount, PUINT8 DataBuf );	/* �Ӵ��̶�ȡ������������ݵ��������� */
#ifdef	EN_DISK_WRITE
extern	UINT8	CHRV3WriteSector( UINT8 SectCount, PUINT8 DataBuf );	/* ���������еĶ�����������ݿ�д����� */
#endif

extern	UINT8	CHRV3DiskConnect( void );	/* �������Ƿ����Ӳ����´���״̬ */
extern	void	xFileNameEnumer( void );	/* �����ⲿ������ӳ���,�ļ���ö�ٻص��ӳ��� */

extern	UINT8	CHRV3LibInit( void );		/* ��ʼ��CHRV3�����,�����ɹ�����0 */

/* �ӳ�������ṩ���ӳ��� */
/* �����ӳ�����, �ļ������ӳ���CHRV3File*�ʹ��̲�ѯ�ӳ���CHRV3DiskQuery�����ܻ��õ��������ݻ�����pDISK_BASE_BUF,
   �����п�����pDISK_BASE_BUF�б����˴�����Ϣ, ���Ա��뱣֤pDISK_BASE_BUF��������������;,
   ���RAM����, Ҫ��pDISK_BASE_BUF��ʱ����������;, ��ô����ʱ�����������CHRV3DirtyBuffer������̻����� */
extern	UINT8	CHRV3GetVer( void );		/* ��ȡ��ǰ�ӳ����İ汾�� */
extern	void	CHRV3DirtyBuffer( void );	/* ������̻����� */
extern	UINT8	CHRV3BulkOnlyCmd( PUINT8 DataBuf );	/* ִ�л���BulkOnlyЭ������� */
extern	UINT8	CHRV3DiskReady( void );		/* ��ѯ�����Ƿ�׼���� */
extern	UINT8	CHRV3AnalyzeError( UINT8 iMode );	/* USB����ʧ�ܷ���CHRV3IntStatus���ش���״̬ */
extern	UINT8	CHRV3FileOpen( void );		/* ���ļ�����ö���ļ� */
extern	UINT8	CHRV3FileClose( void );		/* �رյ�ǰ�ļ� */
#ifdef	EN_DISK_WRITE
extern	UINT8	CHRV3FileErase( void );		/* ɾ���ļ����ر� */
extern	UINT8	CHRV3FileCreate( void );	/* �½��ļ�����,����ļ��Ѿ���������ɾ�������½� */
extern	UINT8	CHRV3FileAlloc( void );		/* �����ļ����ȵ���Ϊ�ļ�����Ĵ��̿ռ� */
#endif
extern	UINT8	CHRV3FileModify( void );	/* ��ѯ�����޸ĵ�ǰ�ļ�����Ϣ */
extern	UINT8	CHRV3FileQuery( void );		/* ��ѯ��ǰ�ļ�����Ϣ */
extern	UINT8	CHRV3FileLocate( void );	/* �ƶ���ǰ�ļ�ָ�� */
extern	UINT8	CHRV3FileRead( void );		/* �ӵ�ǰ�ļ���ȡ���ݵ�ָ�������� */
#ifdef	EN_DISK_WRITE
extern	UINT8	CHRV3FileWrite( void );		/* ��ǰ�ļ�д��ָ�������������� */
#endif
extern	UINT8	CHRV3ByteLocate( void );	/* ���ֽ�Ϊ��λ�ƶ���ǰ�ļ�ָ�� */
extern	UINT8	CHRV3ByteRead( void );		/* ���ֽ�Ϊ��λ�ӵ�ǰλ�ö�ȡ���ݿ� */
#ifdef	EN_DISK_WRITE
extern	UINT8	CHRV3ByteWrite( void );		/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
#endif
extern	UINT8	CHRV3DiskQuery( void );		/* ��ѯ������Ϣ */
extern	void	CHRV3SaveVariable( void );	/* ����/����/�ָ��ӳ����ı���,�����ӳ�����ڶ��оƬ����U��֮������л� */

extern	void	mDelayuS( UINT16 n );		// ��uSΪ��λ��ʱ
extern	void	mDelaymS( UINT16 n );		// ��mSΪ��λ��ʱ
extern	UINT8	USBHostTransact( UINT8 endp_pid, UINT8 tog, UINT32 timeout );	// CHRV3��������,����Ŀ�Ķ˵��ַ/PID����,ͬ����־,NAK����ʱ��,����0�ɹ�,��ʱ/��������
extern	UINT8	HostCtrlTransfer( PUINT8 DataBuf, PUINT8 RetLen );	// ִ�п��ƴ���,8�ֽ���������pSetupReq��,DataBufΪ��ѡ���շ�������,ʵ���շ����ȷ�����ReqLenָ��ı�����
//extern	void	CopySetupReqPkg( PCCHAR pReqPkt );  // ���ƿ��ƴ���������
//extern	UINT8	CtrlGetDeviceDescrTB( void );  // ��ȡ�豸������,������TxBuffer��
extern	UINT8	CtrlGetConfigDescrTB( void );  // ��ȡ����������,������TxBuffer��
//extern	UINT8	CtrlSetUsbAddress( UINT8 addr );  // ����USB�豸��ַ
extern	UINT8	CtrlSetUsbConfig( UINT8 cfg );  // ����USB�豸����
extern	UINT8	CtrlClearEndpStall( UINT8 endp );  // ����˵�STALL
#ifndef	FOR_ROOT_UDISK_ONLY
//extern	UINT8	CtrlGetHubDescr( void );  // ��ȡHUB������,������TxBuffer��
extern	UINT8	HubGetPortStatus( UINT8 HubPortIndex );  // ��ѯHUB�˿�״̬,������TxBuffer��
//extern	UINT8	HubSetPortFeature( UINT8 HubPortIndex, UINT8 FeatureSelt );  // ����HUB�˿�����
extern	UINT8	HubClearPortFeature( UINT8 HubPortIndex, UINT8 FeatureSelt );  // ���HUB�˿�����
#endif





UINT8	CtrlGetConfigDescrTB( void )  // ��ȡ����������,������TxBuffer��
{
	return( CtrlGetConfigDescr( TxBuffer ) );
}

CMD_PARAM_I	mCmdParam;						/* ������� */
#if		DISK_BASE_BUF_LEN > 0
//UINT8	DISK_BASE_BUF[ DISK_BASE_BUF_LEN ] __attribute__((at(BA_RAM+SZ_RAM/2)));	/* �ⲿRAM�Ĵ������ݻ�����,����������Ϊһ�������ĳ��� */
UINT8	DISK_BASE_BUF[ DISK_BASE_BUF_LEN ] __attribute__((aligned (4)));	/* �ⲿRAM�Ĵ������ݻ�����,����������Ϊһ�������ĳ��� */
//UINT8	DISK_FAT_BUF[ DISK_BASE_BUF_LEN ] __attribute__((aligned (4)));	/* �ⲿRAM�Ĵ���FAT���ݻ�����,����������Ϊһ�������ĳ��� */
#endif

/* ���³�����Ը�����Ҫ�޸� */

#ifndef	NO_DEFAULT_ACCESS_SECTOR		/* ��Ӧ�ó����ж���NO_DEFAULT_ACCESS_SECTOR���Խ�ֹĬ�ϵĴ���������д�ӳ���,Ȼ�������б�д�ĳ�������� */
//if ( use_external_interface ) {  // �滻U�������ײ��д�ӳ���
//    CHRV3vSectorSize=512;  // ����ʵ�ʵ�������С,������512�ı���,��ֵ�Ǵ��̵�������С
//    CHRV3vSectorSizeB=9;   // ����ʵ�ʵ�������С��λ����,512���Ӧ9,1024��Ӧ10,2048��Ӧ11
//    CHRV3DiskStatus=DISK_MOUNTED;  // ǿ�ƿ��豸���ӳɹ�(ֻ������ļ�ϵͳ)
//}

UINT8	CHRV3ReadSector( UINT8 SectCount, PUINT8 DataBuf )  /* �Ӵ��̶�ȡ������������ݵ��������� */
{
	UINT8	retry;
//	if ( use_external_interface ) return( extReadSector( CHRV3vLbaCurrent, SectCount, DataBuf ) );  /* �ⲿ�ӿ� */
	for( retry = 0; retry < 3; retry ++ ) {  /* �������� */
		pCBW -> mCBW_DataLen = (UINT32)SectCount << CHRV3vSectorSizeB;  /* ���ݴ��䳤�� */
		pCBW -> mCBW_Flag = 0x80;
		pCBW -> mCBW_LUN = CHRV3vCurrentLun;
		pCBW -> mCBW_CB_Len = 10;
		pCBW -> mCBW_CB_Buf[ 0 ] = SPC_CMD_READ10;
		pCBW -> mCBW_CB_Buf[ 1 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 2 ] = (UINT8)( CHRV3vLbaCurrent >> 24 );
		pCBW -> mCBW_CB_Buf[ 3 ] = (UINT8)( CHRV3vLbaCurrent >> 16 );
		pCBW -> mCBW_CB_Buf[ 4 ] = (UINT8)( CHRV3vLbaCurrent >> 8 );
		pCBW -> mCBW_CB_Buf[ 5 ] = (UINT8)( CHRV3vLbaCurrent );
		pCBW -> mCBW_CB_Buf[ 6 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 7 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 8 ] = SectCount;
		pCBW -> mCBW_CB_Buf[ 9 ] = 0x00;
		CHRV3BulkOnlyCmd( DataBuf );  /* ִ�л���BulkOnlyЭ������� */
		if ( CHRV3IntStatus == ERR_SUCCESS ) {
			return( ERR_SUCCESS );
		}
		CHRV3IntStatus = CHRV3AnalyzeError( retry );
		if ( CHRV3IntStatus != ERR_SUCCESS ) {
			return( CHRV3IntStatus );
		}
	}
	return( CHRV3IntStatus = ERR_USB_DISK_ERR );  /* ���̲������� */
}

#ifdef	EN_DISK_WRITE
UINT8	CHRV3WriteSector( UINT8 SectCount, PUINT8 DataBuf )  /* ���������еĶ�����������ݿ�д����� */
{
	UINT8	retry;
//	if ( use_external_interface ) return( extWriteSector( CHRV3vLbaCurrent, SectCount, DataBuf ) );  /* �ⲿ�ӿ� */
	for( retry = 0; retry < 3; retry ++ ) {  /* �������� */
		pCBW -> mCBW_DataLen = (UINT32)SectCount << CHRV3vSectorSizeB;  /* ���ݴ��䳤�� */
		pCBW -> mCBW_Flag = 0x00;
		pCBW -> mCBW_LUN = CHRV3vCurrentLun;
		pCBW -> mCBW_CB_Len = 10;
		pCBW -> mCBW_CB_Buf[ 0 ] = SPC_CMD_WRITE10;
		pCBW -> mCBW_CB_Buf[ 1 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 2 ] = (UINT8)( CHRV3vLbaCurrent >> 24 );
		pCBW -> mCBW_CB_Buf[ 3 ] = (UINT8)( CHRV3vLbaCurrent >> 16 );
		pCBW -> mCBW_CB_Buf[ 4 ] = (UINT8)( CHRV3vLbaCurrent >> 8 );
		pCBW -> mCBW_CB_Buf[ 5 ] = (UINT8)( CHRV3vLbaCurrent );
		pCBW -> mCBW_CB_Buf[ 6 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 7 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 8 ] = SectCount;
		pCBW -> mCBW_CB_Buf[ 9 ] = 0x00;
		CHRV3BulkOnlyCmd( DataBuf );  /* ִ�л���BulkOnlyЭ������� */
		if ( CHRV3IntStatus == ERR_SUCCESS ) {
			Delay_Us( 200 );  /* д��������ʱ */
			return( ERR_SUCCESS );
		}
		CHRV3IntStatus = CHRV3AnalyzeError( retry );
		if ( CHRV3IntStatus != ERR_SUCCESS ) {
			return( CHRV3IntStatus );
		}
	}
	return( CHRV3IntStatus = ERR_USB_DISK_ERR );  /* ���̲������� */
}
#endif
#endif  // NO_DEFAULT_ACCESS_SECTOR

#ifndef	NO_DEFAULT_DISK_CONNECT			/* ��Ӧ�ó����ж���NO_DEFAULT_DISK_CONNECT���Խ�ֹĬ�ϵļ����������ӳ���,Ȼ�������б�д�ĳ�������� */

/*
Լ��: USB�豸��ַ�������(�ο�USB_DEVICE_ADDR)
��ֵַ  �豸λ��
0x02    ����Root-HUB0�µ�USB�豸���ⲿHUB
0x03    ����Root-HUB1�µ�USB�豸���ⲿHUB
0x1x    ����Root-HUB0�µ��ⲿHUB�Ķ˿�x�µ�USB�豸,xΪ1~n
0x2x    ����Root-HUB1�µ��ⲿHUB�Ķ˿�x�µ�USB�豸,xΪ1~n
*/

//#define		UHUB_DEV_ADDR	( CHRV3vRootPort ? R8_USB1_DEV_AD : R8_USB0_DEV_AD )
//#define		UHUB_MIS_STAT	( CHRV3vRootPort ? R8_USB1_MIS_ST : R8_USB0_MIS_ST )
//#define		UHUB_HOST_CTRL	( CHRV3vRootPort ? R8_UHOST1_CTRL : R8_UHOST0_CTRL )
//#define		UHUB_INT_FLAG	( CHRV3vRootPort ? R8_USB1_INT_FG : R8_USB0_INT_FG )
#define		UHUB_DEV_ADDR	R8_USB_DEV_AD
#define		UHUB_MIS_STAT	R8_USB_MIS_ST
#define		UHUB_HOST_CTRL	R8_UHOST_CTRL
#define		UHUB_INT_FLAG	R8_USB_INT_FG
#define		bUMS_ATTACH		RB_UMS_DEV_ATTACH
#define		bUMS_SUSPEND	RB_UMS_SUSPEND

/* �������Ƿ����� */
UINT8	CHRV3DiskConnect( void )
{
	UINT8	ums, devaddr;
	UHUB_DEV_ADDR = UHUB_DEV_ADDR & 0x7F;
	ums = UHUB_MIS_STAT;
	devaddr = UHUB_DEV_ADDR;
	if ( devaddr == USB_DEVICE_ADDR ) {  /* ����Root-HUB�µ�USB�豸 */
//		if ( UHUB_HOST_CTRL & RB_UH_PORT_EN ) {  /* ����Root-HUB�µ�USB�豸������δ��� */
		if ( ums & bUMS_ATTACH ) {  /* ����Root-HUB�µ�USB�豸���� */
//			if ( ( UHUB_INT_FLAG & UIF_DETECT ) == 0 ) {  /* ����Root-HUB�µ�USB�豸������δ��� */
			if ( ( ums & bUMS_SUSPEND ) == 0 ) {  /* ����Root-HUB�µ�USB�豸������δ��� */
				return( ERR_SUCCESS );  /* USB�豸�Ѿ�������δ��� */
			}
			else {  /* ����Root-HUB�µ�USB�豸���� */
mDiskConnect:
				CHRV3DiskStatus = DISK_CONNECT;  /* �����Ͽ��� */
				return( ERR_SUCCESS );  /* �ⲿHUB��USB�豸�Ѿ����ӻ��߶Ͽ����������� */
			}
		}
		else {  /* USB�豸�Ͽ� */
mDiskDisconn:
			CHRV3DiskStatus = DISK_DISCONNECT;
			return( ERR_USB_DISCON );
		}
	}
#ifndef	FOR_ROOT_UDISK_ONLY
	else if ( devaddr > 0x10 && devaddr <= 0x14 ) {  /* �ⲿHUB�Ķ˿��µ�USB�豸 */
//		if ( UHUB_HOST_CTRL & RB_UH_PORT_EN ) {  /* ����Root-HUB�µ��ⲿHUB������δ��� */
		if ( ums & bUMS_ATTACH ) {  /* ����Root-HUB�µ�USB�豸���� */
//			if ( ( UHUB_INT_FLAG & UIF_DETECT ) == 0 ) {  /* ����Root-HUB�µ�USB�豸������δ��� */
			if ( ( ums & bUMS_SUSPEND ) == 0 ) {  /* ����Root-HUB�µ�USB�豸������δ��� */
				TxBuffer[ MAX_PACKET_SIZE - 1 ] = devaddr;  /* ���� */
				UHUB_DEV_ADDR = USB_DEVICE_ADDR - 1 + ( UHUB_DEV_ADDR >> 4 );  /* ����USB�����˵�USB��ַָ��HUB */
				CHRV3IntStatus = HubGetPortStatus( TxBuffer[ MAX_PACKET_SIZE - 1 ] & 0x0F );  /* ��ѯHUB�˿�״̬,������TxBuffer�� */
				if ( CHRV3IntStatus == ERR_SUCCESS ) {
					if ( TxBuffer[2] & (1<<(HUB_C_PORT_CONNECTION-0x10)) ) {  /* ��⵽HUB�˿��ϵĲ���¼� */
						CHRV3DiskStatus = DISK_DISCONNECT;  /* �ٶ�ΪHUB�˿��ϵ�USB�豸�Ͽ� */
						HubClearPortFeature( TxBuffer[ MAX_PACKET_SIZE - 1 ] & 0x0F, HUB_C_PORT_CONNECTION );  /* ���HUB�˿������¼�״̬ */
					}
					UHUB_DEV_ADDR = TxBuffer[ MAX_PACKET_SIZE - 1 ];  /* ����USB�����˵�USB��ַָ��USB�豸 */
					if ( TxBuffer[0] & (1<<HUB_PORT_CONNECTION) ) {  /* ����״̬ */
						if ( CHRV3DiskStatus < DISK_CONNECT ) {
							CHRV3DiskStatus = DISK_CONNECT;  /* �����Ͽ��� */
						}
						return( ERR_SUCCESS );  /* USB�豸�Ѿ����ӻ��߶Ͽ����������� */
					}
					else {
//						CHRV3DiskStatus = DISK_DISCONNECT;
//						return( ERR_USB_DISCON );
						CHRV3DiskStatus = DISK_CONNECT;
						return( ERR_HUB_PORT_FREE );  /* HUB�Ѿ����ӵ���HUB�˿���δ���Ӵ��� */
					}
				}
				else {
					UHUB_DEV_ADDR = TxBuffer[ MAX_PACKET_SIZE - 1 ];  /* ����USB�����˵�USB��ַָ��USB�豸 */
					if ( CHRV3IntStatus == ERR_USB_DISCON ) {
//						CHRV3DiskStatus = DISK_DISCONNECT;
//						return( ERR_USB_DISCON );
						goto mDiskDisconn;
					}
					else {
						CHRV3DiskStatus = DISK_CONNECT;  /* HUB����ʧ�� */
						return( CHRV3IntStatus );
					}
				}
			}
			else {  /* ����Root-HUB�µ�USB�豸����,�ⲿHUB��USB�豸�Ѿ����ӻ��߶Ͽ����������� */
//				CHRV3DiskStatus = DISK_CONNECT;  /* �����Ͽ��� */
//				return( ERR_SUCCESS );  /* �ⲿHUB��USB�豸�Ѿ����ӻ��߶Ͽ����������� */
				goto mDiskConnect;
			}
		}
		else {  /* �ⲿHUB�Ͽ� */
			CHRV3DiskStatus = DISK_DISCONNECT;
		}
	}
#endif
	else {
//		CHRV3DiskStatus = DISK_DISCONNECT;
//		return( ERR_USB_DISCON );
		goto mDiskDisconn;
	}
}
#endif  // NO_DEFAULT_DISK_CONNECT

#ifndef	NO_DEFAULT_FILE_ENUMER			/* ��Ӧ�ó����ж���NO_DEFAULT_FILE_ENUMER���Խ�ֹĬ�ϵ��ļ���ö�ٻص�����,Ȼ�������б�д�ĳ�������� */
void xFileNameEnumer( void )			/* �ļ���ö�ٻص��ӳ��� */
{
/* ���ָ��ö�����CHRV3vFileSizeΪ0xFFFFFFFF�����FileOpen����ôÿ������һ���ļ�FileOpen������ñ��ص�����
   �ص�����xFileNameEnumer���غ�FileOpen�ݼ�CHRV3vFileSize������ö��ֱ�����������ļ�����Ŀ¼�����������ǣ�
   �ڵ���FileOpen֮ǰ����һ��ȫ�ֱ���Ϊ0����FileOpen�ص�������󣬱�������CHRV3vFdtOffset�õ��ṹFAT_DIR_INFO��
   �����ṹ�е�DIR_Attr�Լ�DIR_Name�ж��Ƿ�Ϊ�����ļ�������Ŀ¼������¼�����Ϣ������ȫ�ֱ�������������
   ��FileOpen���غ��жϷ���ֵ�����ERR_MISS_FILE��ERR_FOUND_NAME����Ϊ�����ɹ���ȫ�ֱ���Ϊ����������Ч�ļ�����
   ����ڱ��ص�����xFileNameEnumer�н�CHRV3vFileSize��Ϊ1����ô����֪ͨFileOpen��ǰ���������������ǻص��������� */
#if		0
	UINT8			i;
	UINT16			FileCount;
	PX_FAT_DIR_INFO	pFileDir;
	PUINT8			NameBuf;
	pFileDir = (PX_FAT_DIR_INFO)( pDISK_BASE_BUF + CHRV3vFdtOffset );  /* ��ǰFDT����ʼ��ַ */
	FileCount = (UINT16)( 0xFFFFFFFF - CHRV3vFileSize );  /* ��ǰ�ļ�����ö�����,CHRV3vFileSize��ֵ��0xFFFFFFFF,�ҵ��ļ�����ݼ� */
	if ( FileCount < sizeof( FILE_DATA_BUF ) / 12 ) {  /* ��黺�����Ƿ��㹻���,�ٶ�ÿ���ļ�����ռ��12���ֽڴ�� */
		NameBuf = & FILE_DATA_BUF[ FileCount * 12 ];  /* ���㱣�浱ǰ�ļ����Ļ�������ַ */
		for ( i = 0; i < 11; i ++ ) NameBuf[ i ] = pFileDir -> DIR_Name[ i ];  /* �����ļ���,����Ϊ11���ַ�,δ����ո� */
//		if ( pFileDir -> DIR_Attr & ATTR_DIRECTORY ) NameBuf[ i ] = 1;  /* �ж���Ŀ¼�� */
		NameBuf[ i ] = 0;  /* �ļ��������� */
	}
#endif
}
#endif  // NO_DEFAULT_FILE_ENUMER

UINT8	CHRV3LibInit( void )  /* ��ʼ��CHRV3�����,�����ɹ�����0 */
{
	if ( CHRV3GetVer( ) < CHRV3_LIB_VER ) return( 0xFF );  /* ��ȡ��ǰ�ӳ����İ汾��,�汾̫���򷵻ش��� */
#if		DISK_BASE_BUF_LEN > 0
	pDISK_BASE_BUF = & DISK_BASE_BUF[0];  /* ָ���ⲿRAM�Ĵ������ݻ����� */
	pDISK_FAT_BUF = & DISK_BASE_BUF[0];  /* ָ���ⲿRAM�Ĵ���FAT���ݻ�����,������pDISK_BASE_BUF�����Խ�ԼRAM */
//	pDISK_FAT_BUF = & DISK_FAT_BUF[0];  /* ָ���ⲿRAM�Ĵ���FAT���ݻ�����,������pDISK_BASE_BUF������ٶ� */
/* ���ϣ������ļ���ȡ�ٶ�,��ô�������������е���CHRV3LibInit֮��,��pDISK_FAT_BUF����ָ����һ�������������pDISK_BASE_BUFͬ����С�Ļ����� */
#endif
	CHRV3DiskStatus = DISK_UNKNOWN;  /* δ֪״̬ */
	CHRV3vSectorSizeB = 9;  /* Ĭ�ϵ�������̵�������512B */
	CHRV3vSectorSize = 512;  // Ĭ�ϵ�������̵�������512B,��ֵ�Ǵ��̵�������С
	CHRV3vStartLba = 0;  /* Ĭ��Ϊ�Զ�����FDD��HDD */
	CHRV3vPacketSize = 64;  /* USB�洢���豸����������:64@FS,512@HS/SS,��Ӧ�ó����ʼ��,ö��U�̺�����Ǹ��ٻ��߳�����ô��ʱ����Ϊ512 */
	pTX_DMA_A_REG = (PUINT32)&R16_UH_TX_DMA;  /* ָ����DMA��ַ�Ĵ���,��Ӧ�ó����ʼ�� */
	pRX_DMA_A_REG = (PUINT32)&R16_UH_RX_DMA;  /* ָ�����DMA��ַ�Ĵ���,��Ӧ�ó����ʼ�� */
	pTX_LEN_REG = (PUINT16)&R8_UH_TX_LEN;  /* ָ���ͳ��ȼĴ���,��Ӧ�ó����ʼ�� */
	pRX_LEN_REG = (PUINT16)&R8_USB_RX_LEN;  /* ָ����ճ��ȼĴ���,��Ӧ�ó����ʼ�� */

//CHRV3vRootPort = 0;  /* USB����ѡ��(����Root-hub��������ѡ�˿�) */
	return( ERR_SUCCESS );
}

void mDelaymS( UINT16 n )
{
	delay(n);
}




#ifdef __cplusplus
}
#endif

#endif
