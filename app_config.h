/********************************************************************************************************
 * @file     app_config.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     May. 12, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#define GPIO_LED		GPIO_PD2
#define GPIO_SW			GPIO_PD3
#define GPIO_RTD_ON		GPIO_PD7
#define GPIO_SENSOR_ON	GPIO_PB1

#define GPIO_I2C_SCL	GPIO_PC1
#define GPIO_I2C_SDA	GPIO_PC0

#define GPIO_PGAP		GPIO_PC2
#define GPIO_PGAN		GPIO_PC3

#define GPIO_ADC1		GPIO_PB7
#define GPIO_ADC2		GPIO_PB6
#define GPIO_ADC3		GPIO_PB5



/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  	24000000

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};

#define UART_PRINT_DEBUG_ENABLE 1

#if (UART_PRINT_DEBUG_ENABLE)
	#define PRINT_BAUD_RATE             					115200
	#define BIT_INTERVAL	             					(CLOCK_SYS_CLOCK_HZ/PRINT_BAUD_RATE)
	#define DEBUG_INFO_TX_PIN           					GPIO_PD7
	#define PD7_OUTPUT_ENABLE         						1
	#define PD7_DATA_OUT                                    1 //must
	#include "application/print/u_printf.h"
#endif




#include "vendor/common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
