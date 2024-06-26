#include "App_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "App_Car.h"


void Start_Task(void * parm);
void Task1(void * parm);
void Task2(void * parm);
void Task3(void * parm);

#define START_TASK_DEPTH 256
#define TASK_PRIORITY 1
TaskHandle_t start_task_handler;

/* 获取角度、编码值任务配置 */
#define START1_TASK_DEPTH 256
#define TASK1_PRIORITY 4
TaskHandle_t start1_task_handler;

/* oled显示屏 */
#define START2_TASK_DEPTH 256
#define TASK2_PRIORITY 2
TaskHandle_t start2_task_handler;

/* PID控制任务 */
#define START3_TASK_DEPTH 256
#define TASK3_PRIORITY 4
TaskHandle_t start3_task_handler;


void FreeRTOS_Start(void) {

	
	// 1.创建启动任务
	xTaskCreate( (TaskFunction_t) Start_Task,
                            (char *) "start_task",
                            (configSTACK_DEPTH_TYPE) START_TASK_DEPTH,
                            (void *) NULL,
                            (UBaseType_t) TASK_PRIORITY,
                            (TaskHandle_t *) &start_task_handler );
                            
	// 2.启动任务调度器
	vTaskStartScheduler();
	
}



/**
 * @brief 启动任务，创建其他任务
 * 
 */	
void Start_Task(void * parm) {
    // 临界区，加锁，不会被中断打断
    vPortEnterCritical();


    // 创建任务1
    xTaskCreate( (TaskFunction_t) Task1,
                            (char *) "task1", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                            (configSTACK_DEPTH_TYPE) START1_TASK_DEPTH,
                            (void *) NULL,
                            (UBaseType_t) TASK1_PRIORITY,
                            (TaskHandle_t *) &start1_task_handler );

    // 创建任务2
    xTaskCreate( (TaskFunction_t) Task2,
                            (char *) "task2", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                            (configSTACK_DEPTH_TYPE) START2_TASK_DEPTH,
                            (void *) NULL,
                            (UBaseType_t) TASK2_PRIORITY,
                            (TaskHandle_t *) &start2_task_handler );

    // 创建任务3
    xTaskCreate( (TaskFunction_t) Task3,
                            (char *) "task3", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                            (configSTACK_DEPTH_TYPE) START3_TASK_DEPTH,
                            (void *) NULL,
                            (UBaseType_t) TASK3_PRIORITY,
                            (TaskHandle_t *) &start3_task_handler );

    // 启动任务只需要一次，用完删除
    vTaskDelete(NULL);

    // 退出临界区
    vPortExitCritical();

}

/**
 * @brief 任务1： 倾斜角度、编码器值
 * 
 */
void Task1(void * parm) {
    TickType_t pxPreviousWakeTime = xTaskGetTickCount() ;
    while(1) {
        App_Car_GetAngle_Encoder();
        xTaskNotifyGive(start3_task_handler);
        vTaskDelayUntil(&pxPreviousWakeTime, 10);
        printf("task1 is running...\r\n");
    }
    
}

/**
 * @brief 任务2： oled显示
 * 
 */
void Task2(void * parm) {
    TickType_t pxPreviousWakeTime = xTaskGetTickCount() ;
    while(1) {
        App_Car_OLED();
        vTaskDelayUntil(&pxPreviousWakeTime, 50);
        printf("task2 is running...\r\n");
    }
}

/**
 * @brief 任务3: 小车速度控制
 * 
 */
void Task3(void * parm) {
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        App_Car_Control();
        printf("task3 is running...\r\n");
    }
    
}



extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
	// printf("test systick...\r\n");
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
     xPortSysTickHandler();
   }
}




