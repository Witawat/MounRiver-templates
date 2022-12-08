# SigmaStar trade secret
# Copyright (c) [2019~2020] SigmaStar Technology.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# SigmaStar and be kept in strict confidence
# (SigmaStar Confidential Information) by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of SigmaStar Confidential
# Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
#------------------------------------------------------------------------------
# Description of some variables owned by the library
#------------------------------------------------------------------------------
# Library module (lib) or Binary module (bin)
PROCESS = lib

PATH_C +=\
    $(PATH_gpio)/src

PATH_H +=\
    $(PATH_system)/sys/inc \
    $(PATH_gpio)/pub \
    $(PATH_gpio_hal)/inc \
    $(PATH_gpio_hal)/pub

#------------------------------------------------------------------------------
# List of source files of the library or executable to generate
#------------------------------------------------------------------------------
SRC_C_LIST = \
    mdrv_gpio.c \
    mdrv_gpio_io.c\
    gpio_test.c

#-------------------------------------------------------------------------------
# Include hal mak
#-------------------------------------------------------------------------------
include $(PATH_sysdriver)/gpio/hal/$(BB_CHIP_ID)/gpio_hal.mak