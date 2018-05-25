//rtos.с
#include "rtos.h"

/******************************************************************************************
 * Переменные модуля
 */
volatile static task TaskArray[MAX_TASKS];      
volatile static u08 arrayTail;                  

/******************************************************************************************
 * Инициализация РТОС, время тика - 1 мс
 */
void RTOS_Init (void)
{
	SysTick_Config(TimerTick_1ms);
	arrayTail = 0;                               
}

/******************************************************************************************
 * Добавление задачи в список
 */
void RTOS_SetTask (void (*taskFunc)(void), u16 taskDelay, u16 taskPeriod)
{
	u08 i;   
	if(!taskFunc) return;
	for(i = 0; i < arrayTail; i++)                     
	{
		if(TaskArray[i].pFunc == taskFunc)             
		{
			NVIC_DisableIRQ(SysTick_IRQn);

			TaskArray[i].delay  = taskDelay;
			TaskArray[i].period = taskPeriod;
			TaskArray[i].run    = 0;   

			NVIC_EnableIRQ(SysTick_IRQn);
			return;                                  
		}
	}
	if (arrayTail < MAX_TASKS)                         
	{                                                  
		NVIC_DisableIRQ(SysTick_IRQn);
      
		TaskArray[arrayTail].pFunc  = taskFunc;
		TaskArray[arrayTail].delay  = taskDelay;
		TaskArray[arrayTail].period = taskPeriod;
		TaskArray[arrayTail].run    = 0;
		arrayTail++;									
		
		NVIC_EnableIRQ(SysTick_IRQn);
   }
}

/******************************************************************************************
 * Удаление задачи из списка
 */
void RTOS_DeleteTask (void (*taskFunc)(void))
{
	u08 i;   
	for (i=0; i<arrayTail; i++)                        
	{
		if(TaskArray[i].pFunc == taskFunc)              
		{         
			NVIC_DisableIRQ(SysTick_IRQn);
			if(i != (arrayTail - 1))                     
			{                                            
				TaskArray[i] = TaskArray[arrayTail - 1];
			}
			arrayTail--;                                 
			NVIC_EnableIRQ(SysTick_IRQn);
			return;
		}
	}
}

/******************************************************************************************
 * Диспетчер РТОС, вызывается в main
 */
void RTOS_DispatchTask()
{
	u08 i;
	void (*function) (void);
	for (i=0; i<arrayTail; i++)                        
	{
		if (TaskArray[i].run == 1)                      
		{                                               
			function = TaskArray[i].pFunc;              
			if(TaskArray[i].period == 0)                
			{                                            
				RTOS_DeleteTask(TaskArray[i].pFunc);               
			}
			else
			{
				TaskArray[i].run = 0;                    				
			}
			(*function)();                              
		}
	}
}

/******************************************************************************************
 * Таймерная служба РТОС (в прерывание аппаратного SysTick таймера)
 */
void RTOS_SchedulerTask (void)
{
	u08 i;
   	for (i=0; i<arrayTail; i++)                       
	{
		if  (TaskArray[i].delay == 0)                  
        {
			TaskArray[i].run = 1;                     
			TaskArray[i].delay = TaskArray[i].period;  
		}
		else
		{
			TaskArray[i].delay--;                     
		}
   }
}
//-----------------------------------------------------------------------------------------
