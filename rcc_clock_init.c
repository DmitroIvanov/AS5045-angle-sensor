//rcc_clock_init.c
//настройка тактирования МК от HSI, HSЕ и использование PLL

#include <rcc_clock_init.h>

void hsi_init (void)
{
//------тут вкл HSION и сбросили все остальное------
	/* Set HSION bit */
	RCC->CR |= (uint32_t)0x00000001;
  
	if (STM32F030C8)//!!!только для этого МК!!!
		{
			/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCO[2:0] bits */
			RCC->CFGR &= (uint32_t)0xF8FFB80C;
		}
	else//!!!для F030F4P6, F030R6T6 !!!
		{
			/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE, MCO[2:0], MCOPRE[2:0] and PLLNODIV bits */
			RCC->CFGR &= (uint32_t)0x08FFB80C;
		}
	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= (uint32_t)0xFEF6FFFF;

	/* Reset HSEBYP bit */
	RCC->CR &= (uint32_t)0xFFFBFFFF;

	/* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
	RCC->CFGR &= (uint32_t)0xFFC0FFFF;

	/* Reset PREDIV1[3:0] bits */
	RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;

	/* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
	RCC->CFGR3 &= (uint32_t)0xFFFFFE6C;

	/* Reset HSI14 bit */
	RCC->CR2 &= (uint32_t)0xFFFFFFFE;

	/* Disable all interrupts */
	RCC->CIR = 0x00000000;
//---тут настройки частоты шин AHB/APB и PLL---------------------
	/* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
	/* Enable Prefetch Buffer and set Flash Latency */
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

	/* HCLK = SYSCLK * настройка пределителя частоты шины AHB */
	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

	/* PCLK = HCLK * настройка пределителя частоты шины APB */
	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

	/* если работаем через PLL, то настраиваем его коэфф умножения */
	/* здесь сбросили настройки:
	* RCC_CFGR_PLLSRC = 0 - HSI/2 входной сигнал PLL;
	* RCC_CFGR_PLLXTPRE = 0 - предделитель HSЕ выкл
	* RCC_CFGR_PLLMULL = 000 - коэфф умножения входн сигнала = х2	*/
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
	/* PLL configuration = HSI/2(4MHz) * 12 = 48 MHz */	
	RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLMULL12);

	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;

	/* Wait till PLL is ready */
	while((RCC->CR & RCC_CR_PLLRDY) == 0)
	{
	}
	/* Select PLL as system clock source */
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

	/* Wait till PLL is used as system clock source */
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
	{
	}
}
//**********************************************************************************
//**********************************************************************************
void hse_init (void)
{
//------тут вкл HSION и сбросили все остальное------
	/* Set HSION bit */
	RCC->CR |= (uint32_t)0x00000001;
  
	if (STM32F030C8)//!!!только для этого МК!!!
		{
			/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCO[2:0] bits */
			RCC->CFGR &= (uint32_t)0xF8FFB80C;
		}
	else//!!!для F030F4P6, F030R6T6 !!!
		{
			/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE, MCO[2:0], MCOPRE[2:0] and PLLNODIV bits */
			RCC->CFGR &= (uint32_t)0x08FFB80C;
		}
	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= (uint32_t)0xFEF6FFFF;

	/* Reset HSEBYP bit */
	RCC->CR &= (uint32_t)0xFFFBFFFF;

	/* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
	RCC->CFGR &= (uint32_t)0xFFC0FFFF;

	/* Reset PREDIV1[3:0] bits */
	RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;

	/* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
	RCC->CFGR3 &= (uint32_t)0xFFFFFE6C;

	/* Reset HSI14 bit */
	RCC->CR2 &= (uint32_t)0xFFFFFFFE;

	/* Disable all interrupts */
	RCC->CIR = 0x00000000;
	
//---тут настройки HSE, частоты шин AHB/APB и PLL---------------------

  uint32_t StartUpCounter = 0, HSEStatus = 0;

   /* SYSCLK, HCLK, PCLK configuration -----------------------------*/
   /* Enable HSE */
   RCC->CR |= ((uint32_t)RCC_CR_HSEON);

   /* Wait till HSE is ready and if Time out is reached exit */
   do
	{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while((HSEStatus == 0) && (StartUpCounter != STARTUP_TIMEOUT));

   if ((RCC->CR & RCC_CR_HSERDY) != RESET)
	{
		HSEStatus = (uint32_t)0x01;
	}
   else
	{
		HSEStatus = (uint32_t)0x00;
	}

   if (HSEStatus == (uint32_t)0x01)//если HSE вкл, то настраиваем все остальное
	{
		/* Enable Prefetch Buffer and set Flash Latency */
		FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

		/* HCLK = SYSCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

		/* PCLK = HCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

		/* если работаем через PLL, то настраиваем его коэфф умножения */
		/* здесь сбросили настройки:
		* RCC_CFGR_PLLSRC = 0 - HSI/2 входной сигнал PLL;
		* RCC_CFGR_PLLXTPRE = 0 - предделитель HSЕ выкл
		* RCC_CFGR_PLLMULL = 000 - коэфф умножения входн сигнала = х2	*/   
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
		/* PLL configuration:
		* RCC_CFGR_PLLSRC_PREDIV1 = 1 - HSE/PREDIV входной сигнал PLL
		* RCC_CFGR_PLLXTPRE_PREDIV1 = 0 - предделитель HSЕ выкл, коэф выставл в RCC_CFGR3 (по умолчанию=1)
		* RCC_CFGR_PLLMULL6 = HSE * 6 = 48 MHz */
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL6);

		/* Enable PLL */
		RCC->CR |= RCC_CR_PLLON;

		/* Wait till PLL is ready */
		while((RCC->CR & RCC_CR_PLLRDY) == 0)
			{
			}
		/* Select PLL as system clock source */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

		/* Wait till PLL is used as system clock source */
		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
			{
			}
	}
   else
	{ 	/* тут оказались если HSE не запустился и приходится работать от HSI и
		* необходимо его соответственно настроить;
		* выключаем HSE и настраиваем тактирование от HSI	*/
	   
		/* Disable HSE */
		RCC->CR &= (uint32_t)((uint32_t)~(RCC_CR_HSEON));
		
		/* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
		/* Enable Prefetch Buffer and set Flash Latency */
		FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

		/* HCLK = SYSCLK * настройка пределителя частоты шины AHB */
		RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

		/* PCLK = HCLK * настройка пределителя частоты шины APB */
		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

		/* если работаем через PLL, то настраиваем его коэфф умножения */
		/* здесь сбросили настройки:
			* RCC_CFGR_PLLSRC = 0 - HSI/2 входной сигнал PLL;
		* RCC_CFGR_PLLXTPRE = 0 - предделитель HSЕ выкл
		* RCC_CFGR_PLLMULL = 000 - коэфф умножения входн сигнала = х2	*/
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
		/* PLL configuration = HSI/2(4MHz) * 6 = 24 MHz */	
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLMULL6);

		/* Enable PLL */
		RCC->CR |= RCC_CR_PLLON;

		/* Wait till PLL is ready */
		while((RCC->CR & RCC_CR_PLLRDY) == 0)
			{
			}
		/* Select PLL as system clock source */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

		/* Wait till PLL is used as system clock source */
		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
			{
			}	
	}
}
//**********************************************************************************
//**********************************************************************************
void pll_config()
{	//--------PLL_Init---------------------------------
		if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL)//проверяем вкл ли PLL
			{
				//если вкл то:
				RCC->CFGR &= (~RCC_CFGR_SW);//выбираем HSI источником тактирования
				while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) { } //ждем пока запустится HSI
			}
		else
			{
				//даже если не вкл (тогда МК может работать от HSЕ)
				RCC->CFGR &= (~RCC_CFGR_SW);//выбираем HSI источником тактирования
				while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) { } //ждем пока запустится HSI
			}
			
		RCC->CR &= (~RCC_CR_PLLON);//выкл PLL
		while((RCC->CR & RCC_CR_PLLRDY) != 0) { } //ждем пока PLL выкл
		
		/*после этого шага МК работает от HSI и можно совершать разные настройки:
		* - изменить входной сигнал PLL;
		* - изменить частоты шин AHB/APBx;
		* - изменить коэф умножения PLLMULL */
		
		//выкл HSE, если дальше будем работать от HSI
		//RCC->CR &= (~RCC_CR_HSEON);//выкл HSE
		//while((RCC->CR & RCC_CR_HSERDY) != 0) { } //ждем пока HSE выкл
		
		//источник тактирования PLL - HSE
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC));
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1);
		
		//коэф деления частоты HSE на входе PLL
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLXTPRE));
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLXTPRE_PREDIV1);
		
		//коэф умножения частоты PLL
		RCC->CFGR &= (~RCC_CFGR_PLLMULL); //сбрасываем коэф умнож PLL=000 (это х2)
		RCC->CFGR |= RCC_CFGR_PLLMULL4; //устанавливаем коэф умножения PLL x4 (SYSCLK=16 MHz)
		
		//частоты работы шин AHB/APBx
		RCC->CFGR &= (~RCC_CFGR_HPRE); //шина AHB работает без делит на SYSCLK		
		RCC->CFGR &= (~RCC_CFGR_PPRE); //шина APB работает без делит на SYSCLK
		
		//тактирования различной периферии (наприм UART)
		//RCC->CFGR3 &= (~RCC_CFGR3_USART1SW); //тактирование UART от SYSCLK
		
		RCC->CR |= RCC_CR_PLLON; //вкл PLL
		while((RCC->CR & RCC_CR_PLLRDY) == 0) { } //ждем готовности PLL
		
		RCC->CFGR |= RCC_CFGR_SW_PLL; //выбираем PLL как источник тактирования
		while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { } //ждем пока PLL вкл
}
//**********************************************************************************
//**********************************************************************************
