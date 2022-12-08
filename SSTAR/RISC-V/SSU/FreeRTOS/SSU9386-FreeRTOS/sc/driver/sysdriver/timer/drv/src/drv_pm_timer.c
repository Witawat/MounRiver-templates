/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/

/*=============================================================*/
// Include files
/*=============================================================*/

#include "vm_types.ht"
#include "kernel.h"
#include "kernel_systimer.h"
#include "drv_pm_timer.h"
#include "drv_int_ctrl_pub_api.h"
#include "sys_sys_isw_uart.h"

/*=============================================================*/
// Variable definition
/*=============================================================*/

void DrvTimerRegisterIsr(u32 u32TimerId, PfnTimerIntcISR pfnTimerISR)
{
    IntInitParam_u uInitParam = {{0}};
    u32 u32IntId = 0;

    switch (u32TimerId)
    {
        case DRV_TIMER0:
            u32IntId = INT_FIQ_TIMER_0;
            break;
        case DRV_TIMER1:
            u32IntId = INT_FIQ_TIMER_1;
            break;
        case DRV_TIMER2:
            u32IntId = INT_FIQ_TIMER_2;
            break;
        default:
            UartSendTrace("[%s] Wrong timer ID [%d]\r\n", __FUNCTION__, u32TimerId);
            return;
            break;
    }

    uInitParam.intc.eMap = INTC_MAP_FIQ;
    uInitParam.intc.ePriority = INTC_PRIORITY_6;
    uInitParam.intc.pfnIsr = (PfnIntcISR)pfnTimerISR;
    uInitParam.intc.pDevId = NULL;
    DrvInitInterrupt(&uInitParam, u32IntId);
    DrvUnmaskInterrupt(u32IntId);
}

void DrvTimerReleaseIsr(u32 u32TimerId)
{
    u32 u32IntId = 0;

    switch (u32TimerId)
    {
        case DRV_TIMER0:
            u32IntId = INT_FIQ_TIMER_0;
            break;
        case DRV_TIMER1:
            u32IntId = INT_FIQ_TIMER_1;
            break;
        case DRV_TIMER2:
            u32IntId = INT_FIQ_TIMER_2;
            break;
        default:
            UartSendTrace("[%s] Wrong timer ID [%d]\r\n", __FUNCTION__, u32TimerId);
            return;
            break;
    }

    DrvMaskInterrupt(u32IntId);
    DrvIntcFreeIsr(u32IntId, NULL);
}

/////////////////////////////////////////////////////////

static KeInfinteTimer_t* _DrvTimerGetObj(u32 u32TimerId)
{
    KeInfinteTimer_t* ptTimerObj = NULL;

    switch (u32TimerId)
    {
        case DRV_TIMER0:
            ptTimerObj = (KeInfinteTimer_t *)g_ptKeInfinteTimer0;
            break;
        case DRV_TIMER1:
            ptTimerObj = (KeInfinteTimer_t *)g_ptKeInfinteTimer1;
            break;
        case DRV_TIMER2:
            ptTimerObj = (KeInfinteTimer_t *)g_ptKeInfinteTimer2;
            break;
        default:
            UartSendTrace("[%s] Wrong timer ID [%d]\r\n", __FUNCTION__, u32TimerId);
            break;
    }

    return ptTimerObj;
}

void DrvTimerDisableIntr(u32 u32TimerId)
{
    volatile KeInfinteTimer_t* ptTimerObj = NULL;

    ptTimerObj = (volatile KeInfinteTimer_t*)_DrvTimerGetObj(u32TimerId);

    if (ptTimerObj)
        ptTimerObj->reg_timer_int_en = 0x0;
}

void DrvTimerEnableIntr(u32 u32TimerId)
{
    volatile KeInfinteTimer_t* ptTimerObj = NULL;

    ptTimerObj = (volatile KeInfinteTimer_t*)_DrvTimerGetObj(u32TimerId);

    if (ptTimerObj)
        ptTimerObj->reg_timer_int_en = 0x1;
}

void DrvTimerClearIntr(u32 u32TimerId)
{
    volatile KeInfinteTimer_t* ptTimerObj = NULL;

    ptTimerObj = (volatile KeInfinteTimer_t*)_DrvTimerGetObj(u32TimerId);

    if (ptTimerObj)
        ptTimerObj->reg_timer_hit = 1;
}

u32 DrvTimerGetIntrStatus(u32 u32TimerId)
{
    volatile KeInfinteTimer_t* ptTimerObj = NULL;
    u32 u32IntrStatus = 0;

    ptTimerObj = (volatile KeInfinteTimer_t*)_DrvTimerGetObj(u32TimerId);

    if (ptTimerObj)
        u32IntrStatus = ptTimerObj->reg_timer_hit;
    return u32IntrStatus;
}

void DrvTimerStop(u32 u32TimerId)
{
    volatile KeInfinteTimer_t* ptTimerObj = NULL;

    ptTimerObj = (volatile KeInfinteTimer_t*)_DrvTimerGetObj(u32TimerId);

    if (ptTimerObj)
        ptTimerObj->reg_timer_en = 0x00;
}

void DrvTimerStart(u32 u32TimerId, u32 u32MaxTick)
{
    volatile KeInfinteTimer_t* ptTimerObj = NULL;

    ptTimerObj = (volatile KeInfinteTimer_t*)_DrvTimerGetObj(u32TimerId);

    if (ptTimerObj)
    {
        ptTimerObj->reg_timer_int_en = (u32MaxTick == DRV_TIMER_TICK_MAX_COUNT) ? 0x00 : 0x01;
        ptTimerObj->reg_timer_max_low = ((u32MaxTick) & 0xFFFF);
        ptTimerObj->reg_timer_max_high = ((u32MaxTick>>16) & 0xFFFF);
        ptTimerObj->reg_timer_en = 0x01;
    }
}

u32 DrvTimerGetTimeoutLength(u32 u32TimerId)
{
    volatile KeInfinteTimer_t* ptTimerObj = NULL;
    u32 u32TimeoutLength = 0;

    ptTimerObj = (volatile KeInfinteTimer_t*)_DrvTimerGetObj(u32TimerId);

    if (ptTimerObj)
        u32TimeoutLength = (ptTimerObj->reg_timer_max_low + (ptTimerObj->reg_timer_max_high<<16));
    return u32TimeoutLength;
}

u32 DrvTimerGetCount(u32 u32TimerId)
{
    volatile KeInfinteTimer_t* ptTimerObj = NULL;
    u32 u32TimeCount = 0;

    ptTimerObj = (volatile KeInfinteTimer_t*)_DrvTimerGetObj(u32TimerId);

    if (ptTimerObj)
        u32TimeCount = (ptTimerObj->reg_timer_cap_low + (ptTimerObj->reg_timer_cap_high<<16));
    return u32TimeCount;
}
