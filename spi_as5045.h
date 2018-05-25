//spi_as5045.h
//********************************************************
//настройки SPI для опроса датчика угла и выдачу на индикатор + GPIO сигналов ошибок
//********************************************************
#ifndef __SPI_AS5045_H__
#define __SPI_AS5045_H__

#include <stm32f0xx.h>
#include <system_stm32f0xx.h>
#include "string.h"
#include "stdlib.h"
//********************************************************
#define  OCF_GOOD    (GPIOA->BSRR |= GPIO_BSRR_BS_2)
#define  OCF_BAD     (GPIOA->BSRR |= GPIO_BSRR_BR_2)

#define  COF_GOOD    (GPIOA->BSRR |= GPIO_BSRR_BR_3)
#define  COF_BAD     (GPIOA->BSRR |= GPIO_BSRR_BS_3)

#define  LIN_GOOD    (GPIOB->BSRR |= GPIO_BSRR_BR_1)
#define  LIN_BAD     (GPIOB->BSRR |= GPIO_BSRR_BS_1)

#define  HSE_RDY     (GPIOA->BSRR |= GPIO_BSRR_BR_9)
#define  HSE_BAD     (GPIOA->BSRR |= GPIO_BSRR_BS_9)

#define  PLL_RDY     (GPIOA->BSRR |= GPIO_BSRR_BR_10)
#define  PLL_BAD     (GPIOA->BSRR |= GPIO_BSRR_BS_10)

//********************************************************
extern uint16_t razrad[6];//массив с текущими значениями разрядов индикатора
extern uint16_t as5045ReadData[6];//массив со значениями датчика
//********************************************************
void spi_init(void);

void hse_pll_analiz(void);

//*************************************************
//функции работы с датчиком AS5045
void display_and_readAS5045 (uint16_t *as5045ReadData, uint16_t *razrad);

void set_razrad (uint16_t *as5045ReadData, uint16_t *razrad);

//************************************************
#endif /* __SPI_AS5045__ */
