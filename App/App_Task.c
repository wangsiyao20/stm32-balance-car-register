#include "App_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"


void Start_Task(void * parm);
void Task1(void * parm);
void Task2(void * parm);
void Task3(void * parm);

#define START_TASK_DEPTH 128
#define TASK_PRIORITY 1
TaskHandle_t start_task_handler;

#define START1_TASK_DEPTH 128
#define TASK1_PRIORITY 2
TaskHandle_t start1_task_handler;

#define START2_TASK_DEPTH 128
#define TASK2_PRIORITY 3
TaskHandle_t start2_task_handler;

#define START3_TASK_DEPTH 128
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
 * @brief 任务1
 * 
 */
void Task1(void * parm) {
    while(1) {
        printf("task1 is running ... \r\n");
        vTaskDelay(500);
    }
    
}

/**
 * @brief 任务2
 * 
 */
void Task2(void * parm) {
    while(1) {

        printf("task2 is running ... \r\n");
        vTaskDelay(500);
    }
}

/**
 * @brief 任务3
 * 
 */
void Task3(void * parm) {
    while (1) {
        printf("task3 is running...\r\n");
        vTaskDelay(500);
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




