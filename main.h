//main.h

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stm32f0xx.h>
#include <system_stm32f0xx.h>
#include "string.h" //Подключение функций Си
#include "stdlib.h"

#include <rcc_clock_init.h>
#include <spi_as5045.h>
#include <rtos.h>
//**********************************************************************************
void display_and_read(void);//PA0 вкл
void set_razrad_and_error(void);//PA0 выкл
//**********************************************************************************

#endif /* __MAIN_H_ */
