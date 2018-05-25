//rtos.h
/******************************************************************************************
 * За основу взят планировщик задач Шибанова                                 
 * http://chipenable.ru/index.php/embedded-programming/item/219-planirovschik-dlya-mikrokontrollera.html
 * Автор - Шибанов Владимир aka KontAr
 * Что может: 
 * - однократный вызов задачи сразу или с задержкой "taskDelay" 
 * - при повторном добавлении задачи обновляются ее переменные 
 * - Возможность циклического запуска задачи с заданным периодом "taskPeriod"
 * - удаление задачи по имени                                     
 ******************************************************************************************/
#ifndef __RTOS_H__
#define __RTOS_H__

#include <stm32f0xx.h>
#include <system_stm32f0xx.h>
#include "string.h"
#include "stdlib.h"

#define MAX_TASKS      9               
#define TimerTick_1ms	(48000-1)      

/******************************************************************************************
 * Структура задачи
 */
typedef unsigned char      u08;
typedef unsigned int       u16;
typedef struct task
{   
   void (*pFunc) (void);               
   u16 delay;                          
   u16 period;                         
   u08 run;                            
}task;
/******************************************************************************************
 * Прототипы фукнций
 */
void RTOS_Init (void);
void RTOS_SetTask (void (*taskFunc)(void), u16 taskDelay, u16 taskPeriod);
void RTOS_DeleteTask (void (*taskFunc)(void));
void RTOS_DispatchTask (void);
void RTOS_SchedulerTask (void);

#endif /* __RTOS_H__ */
