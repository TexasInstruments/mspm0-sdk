/*
 * Copyright (c) 2022, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== TaskP_freertos.c ========
 */

#include <ti/drivers/dpl/TaskP.h>

#include <FreeRTOS.h>
#include <task.h>

/*
 *  ======== Array for conversion of FreeRTOS task state to DPL task state ========
 */
const TaskP_State taskState[] = {TaskP_State_RUNNING,  /*!< eRunning */
                                 TaskP_State_READY,    /*!< eReady */
                                 TaskP_State_BLOCKED,  /*!< eBlocked */
                                 TaskP_State_INACTIVE, /*!< eSuspended */
                                 TaskP_State_DELETED,  /*!< eDeleted */
                                 TaskP_State_INVALID}; /*!< eInvalid */

/*
 *  ======== Default TaskP_Params values ========
 */
static const TaskP_Params TaskP_defaultParams = {
    .name      = "NAME",
    .arg       = NULL,
    .priority  = 1,
    .stackSize = TaskP_DEFAULT_STACK_SIZE,
    .stack     = NULL,
};

/*
 *  ======== TaskP_Params_init ========
 */
void TaskP_Params_init(TaskP_Params *params)
{
    /* structure copy */
    *params = TaskP_defaultParams;
}

/*
 *  ======== TaskP_create ========
 */
TaskP_Handle TaskP_create(TaskP_Function fxn, const TaskP_Params *params)
{
    TaskHandle_t task = NULL;

    /* Create the task using dynamic memory allocation */
    xTaskCreate((TaskFunction_t)fxn,
                params->name,
                (uint32_t)(params->stackSize / 4),
                params->arg,
                (UBaseType_t)params->priority,
                &task);

    return ((TaskP_Handle)task);
}

/*
 *  ======== TaskP_delete ========
 */
void TaskP_delete(TaskP_Handle task)
{
    if (task != NULL)
    {
        /* Delete the task that was created by use of dynamic memory allocation */
        vTaskDelete((TaskHandle_t)task);
    }
}

/*
 *  ======== TaskP_construct ========
 */
TaskP_Handle TaskP_construct(TaskP_Struct *obj, TaskP_Function fxn, const TaskP_Params *params)
{
    TaskHandle_t task = NULL;

#if (configSUPPORT_STATIC_ALLOCATION == 1)
    if (params == NULL)
    {
        /* Set default parameter values */
        params = &TaskP_defaultParams;
    }

    /* Construct the task using static memory */
    task = xTaskCreateStatic((TaskFunction_t)fxn,
                             params->name,
                             (uint32_t)(params->stackSize / 4),
                             params->arg,
                             (UBaseType_t)params->priority,
                             (StackType_t *)params->stack,
                             (StaticTask_t *)obj);
#endif

    return ((TaskP_Handle)task);
}

/*
 *  ======== TaskP_destruct ========
 */
void TaskP_destruct(TaskP_Struct *obj)
{
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    if (obj != NULL)
    {
        /* Destruct the task that was constructed using static memory */
        vTaskDelete((TaskHandle_t)obj);
    }
#endif
}

/*
 *  ======== TaskP_getState ========
 */
TaskP_State TaskP_getState(TaskP_Handle task)
{
    eTaskState state;

    state = eTaskGetState((TaskHandle_t)task);

    /* Convert from FreeRTOS task state to DPL task state */
    return (taskState[state]);
}

/*
 *  ======== TaskP_getCurrentTask ========
 */
TaskP_Handle TaskP_getCurrentTask(void)
{
    return ((TaskP_Handle)xTaskGetCurrentTaskHandle());
}

/*
 *  ======== TaskP_disableScheduler ========
 */
uintptr_t TaskP_disableScheduler(void)
{
    vTaskSuspendAll();
    return (0);
}

/*
 *  ======== TaskP_restoreScheduler ========
 */
void TaskP_restoreScheduler(uintptr_t key)
{
    xTaskResumeAll();
}

/*
 *  ======== TaskP_yield ========
 */
void TaskP_yield(void)
{
    taskYIELD();
}

/*
 *  ======== TaskP_getTaskObjectSize ========
 */
uint32_t TaskP_getTaskObjectSize(void)
{
    return (sizeof(StaticTask_t));
}
