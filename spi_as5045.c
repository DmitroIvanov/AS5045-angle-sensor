//spi_as5045.c

#include <spi_as5045.h>

uint16_t razrad[6];
uint16_t as5045ReadData[6];
//********************************************************
void spi_init(void)
{
//--------GPIO_Init---------------------------------
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
	//PA4 - SPI1_NSS (AFO - Alternate function)
	GPIOA->MODER |= GPIO_MODER_MODER4_1;
	GPIOA->AFR[0] |= (GPIO_AFRL_AFRL4 & 0x00000000);//AF0 - AFR4[3:0]=0b0000
	//PA5 - SPI1_SCK (AF0 - Alternate function)
	GPIOA->MODER |= GPIO_MODER_MODER5_1;
	GPIOA->AFR[0] |= (GPIO_AFRL_AFRL5 & 0x00000000);
	//PA6 - SPI1_MISO (AFO - Alternate function)
	GPIOA->MODER |= GPIO_MODER_MODER6_1;
	GPIOA->AFR[0] |= (GPIO_AFRL_AFRL6 & 0x00000000);
	//PA7 - SPI1_MOSI (AF0 - Alternate function)
	GPIOA->MODER |= GPIO_MODER_MODER7_1;
	GPIOA->AFR[0] |= (GPIO_AFRL_AFRL7 & 0x00000000);
//----------------------
	//PA2 - OCF (output)
	GPIOA->MODER |= GPIO_MODER_MODER2_0;
	//PA3 - COF (output)
	GPIOA->MODER |= GPIO_MODER_MODER3_0;
	//PB1 - LIN (Output)
	GPIOB->MODER |= GPIO_MODER_MODER1_0;
	//PA9 - HSE (Output)
	GPIOA->MODER |= GPIO_MODER_MODER9_0;
	//PA10 - PLL(Output)
	GPIOA->MODER |= GPIO_MODER_MODER10_0;
//--------SPI_Init---------------------------------
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR; //MSTR-Master, BR=fpclk/256 (скорость передачи)
	SPI1->CR2 |= SPI_CR2_SSOE;//SSOE=1 -откл реж мульмастер;
	SPI1->CR2 |= SPI_CR2_NSSP;//NSSP- для защелкивания 74HC595	
	SPI1->CR2 |= SPI_CR2_DS;//DS=1111 - Data size = 16 bits	
	SPI1->CR2 &= ~(SPI_CR2_FRF);//Res FRF - SPI Motorola mode
	SPI1->CR1 |= SPI_CR1_SPE; //enable SPI
}
//********************************************************
//--------send and receive data SPI---------------------
//Функция отправляет и приним 2 байта одновременно
uint16_t spi_com(uint16_t data)
{
		while(!(SPI1->SR & SPI_SR_TXE));
		SPI1->DR = data; 
	    
		while(!(SPI1->SR & SPI_SR_RXNE));
		data = SPI1->DR; 

	    return data; 
}
//**********************************************************
/*************************************************************************
 * функции void set_segment (uint8_t razrad_simvol) - сопоставляет числу uint8_t razrad_simvol
 * сегменты индикатора
 *************************************************************************/
uint16_t set_segment (uint8_t razrad_simvol)
{
	uint16_t segmentsTakeOn;//то, что выводится на 1 разряд индикатора
	switch (razrad_simvol)
	{
			case 0: //цифра 0
				{
					segmentsTakeOn = 0x003F;//цифра 0
					break;
				}
			case 1: //цифра 1
				{
					segmentsTakeOn = 0x0006;//цифра 1
					break;
				}
			case 2: //цифра 2
				{
					segmentsTakeOn = 0x005B;//цифра 2
					break;
				}
			case 3: //цифра 3
				{
					segmentsTakeOn = 0x004F;//цифра 3
					break;
				}
			case 4: //цифра 4
				{
					segmentsTakeOn = 0x0066;//цифра 4
					break;
				}
			case 5: //цифра 5
				{
					segmentsTakeOn = 0x006D;//цифра 5
					break;
				}
			case 6: //цифра 6
				{
					segmentsTakeOn = 0x007D;//цифра 6
					break;
				}
			case 7: //цифра 7
				{
					segmentsTakeOn = 0x0007;//цифра 7
					break;
				}
			case 8: //цифра 8
				{
					segmentsTakeOn = 0x007F;//цифра 8
					break;
				}
			case 9: //цифра 9
				{
					segmentsTakeOn = 0x006F;// 9
					break;
				}
			default:
				{
					segmentsTakeOn = 0x0040;//минус
				}
	}
	return segmentsTakeOn;
}
//*************************************************************************
//Каждый вызов ф-ции выводит один разряд на индикацию + читает показания датчика
//uint16_t as5045ReadData[6] - считанные с датчика данные - выходной параметр
//uint16_t razrad[6] - зачения индикатора поразрядно - входной параметр
void display_and_readAS5045 (uint16_t *as5045ReadData, uint16_t *razrad)
{
	static uint8_t Nrazr = 0;
	uint16_t a;
	switch (Nrazr)
	{
		case 0://младший разряд
		{
			a=razrad[0];
			as5045ReadData[Nrazr] = spi_com(a);//засветили 1-й разряд
			break;
		}
		case 1:
		{
			a=razrad[1];
			as5045ReadData[Nrazr] = spi_com(a);//засветили 2-й разряд
			break;
		}
		case 2:
		{
			a=razrad[2];
			as5045ReadData[Nrazr] = spi_com(a);
			break;
		}
		case 3:
		{
			a=razrad[3];
			as5045ReadData[Nrazr] = spi_com(a);
			break;
		}
		case 4:
		{
			a=razrad[4];
			as5045ReadData[Nrazr] = spi_com(a);
			break;
		}
		case 5:
		{
			a=razrad[5];
			as5045ReadData[Nrazr] = spi_com(a);
			break;
		}
	}
//здесь считаем какой следуюший разряд будем высвечивать
	if (Nrazr == 5)	{Nrazr = 0;}
	else			{Nrazr++;}
}
//*************************************************************************
//сюда передали считанный массив, а получили только одно значение(только последнее и правильное)
// + анализ ошибок и вывод их на сд/диоды.
uint16_t data_error_analiz (uint16_t *as5045ReadData)
{	
	uint16_t AS5045_display;
	uint16_t error[6];
	uint8_t i; uint8_t ocf = 0; uint8_t cof = 0; uint8_t lin = 0;
	for (i = 0; i < 6; ++i)
	{
		error[i] = (as5045ReadData[i] & 0x0007);//выделили служебную часть числа
		if (error[i] & 0x0004) //проверка разряда OCF
		{//здесь данные от AS5045 верны, поэтому передаем их на индикацию
			ocf++;
			AS5045_display = as5045ReadData[i];
		}
		if (error[i] & 0x0002) //проверка разряда COF
		{
			cof++;
		}
		if (error[i] & 0x0001) //проверка разряда LIN
		{
			lin++;
		}
	}
	//отображаем ошибки датчика
	if (ocf >= 3) {OCF_GOOD;}//1 - даные корректны
	else {OCF_BAD;}//если 0 - даные некорректны

	if (cof >= 3) {	COF_BAD;}//1 - даные некорректны
	else {COF_GOOD;} //если 0 - даные корректны

	if (lin >= 3) {LIN_BAD;}//1 - даные некорректны
	else {LIN_GOOD;} //если 0 - даные корректны

	return AS5045_display;
}

/*************************************************************************
 * функция анализирут принятые данные от AS5045 на ошибки, пересчитывает в градусы, разбивает на разряды.
 *   uint16_t as5045ReadData[6] - 6 значений, считанных с датчика угла - входной параметр.
 *   uint16_t AS5045_num = 0...4095 - 12 разрядов значения датчика.
 *   uint32_t AS5045_ugol = (AS5045_num >> 3) * 879121 (MAX = 3 600 000 495=0xd693 a5ef) - перевод в градусы * 10^7.
 *   uint16_t razrad[6] - значения сегментов индикатора поразрядно - выходной параметр
 *************************************************************************/
void set_razrad (uint16_t *as5045ReadData, uint16_t *razrad)
{
	//проанализировали ошибки и вывели значение AS5045 для разбиения на разряды
	uint16_t AS5045_num =  data_error_analiz (as5045ReadData);
	//пересчитали в градусы
	uint32_t AS5045_ugol = 0xD693A5EF - (((uint32_t)AS5045_num >> 3) * 879121);
	//--- разбили на разряды значение угла в формате индикатора ----
	uint16_t data = 0;
	uint8_t razrad_simvol = 0;
	while(AS5045_ugol >= 1000000000)
	{
		AS5045_ugol -= 1000000000;
		razrad_simvol++;//вычислии цифру 6 разряда
	}
	data = set_segment (razrad_simvol);//сопоставили сегменты
	data |= 0x2000;//добавили в каком разряде засветить
	razrad[5] = data;//получили что высветить в 6 разряде
	razrad_simvol = 0;
	
	while(AS5045_ugol >= 100000000)
	{
		AS5045_ugol -= 100000000;
		razrad_simvol++;//вычислии цифру 5 разряда
	}
	data = set_segment (razrad_simvol);
	data |= 0x1000;
	razrad[4] = data;
	razrad_simvol = 0;

	while(AS5045_ugol >= 10000000)
	{
		AS5045_ugol -= 10000000;
		razrad_simvol++;//вычислии цифру 4 разряда
	}
	data = set_segment (razrad_simvol);
	data |= 0x0880;//+ добавили точку
	razrad[3]= data;
	razrad_simvol = 0;

	while(AS5045_ugol >= 1000000)
	{
		AS5045_ugol -= 1000000;
		razrad_simvol++;//вычислии цифру 3 разряда
	}
	data = set_segment (razrad_simvol);
	data |= 0x0400;
	razrad[2] = data;
	razrad_simvol = 0;

	while(AS5045_ugol >= 100000)
	{
		AS5045_ugol -= 100000;
		razrad_simvol++;//вычислии цифру 2 разряда
	}
	data = set_segment (razrad_simvol);
	data |= 0x0200;
	razrad[1] = data;
	razrad_simvol = 0;

	while(AS5045_ugol >= 10000)
	{
		AS5045_ugol -= 10000;
		razrad_simvol++;//вычислии цифру 1 разряда
	}
	data = set_segment (razrad_simvol);
	data |= 0x0100;
	razrad[0] = data;
	razrad_simvol = 0;
}
//*************************************************************************
//анализирует работу схемы тактирования МК
void hse_pll_analiz(void)
{
	//периодически проверяем состояние генератора - HSE
	if ((RCC->CR & RCC_CR_HSERDY) == RCC_CR_HSERDY)
	{
		HSE_RDY;//св.д - не горит
	}else
	{
		HSE_BAD;//горит красн св.д
	}//периодически проверяем System clock switch status - SWS
	if (((RCC->CFGR & RCC_CFGR_SWS_HSI)==RCC_CFGR_SWS_HSI) || ((RCC->CFGR & RCC_CFGR_SWS_HSE)==RCC_CFGR_SWS_HSE))
	{
		PLL_RDY;//должны работать от PLL; св.д - не горит
	}else
	{
		PLL_BAD;//горит красный св.д
	}
}
//*************************************************************************
