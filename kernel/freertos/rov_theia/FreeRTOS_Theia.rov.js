"use strict";
/*
 * Copyright (c) 2022-2024, Texas Instruments Incorporated
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
// source from simplelink_cc32xx_sdk_6_10_00_05/kernel/freertos/rov
// modified for CCS Theia
Object.defineProperty(exports, "__esModule", { value: true });
class SystemStack {
}
class Heap {
}
class TaskModule {
}
class TaskInstance {
}
class FreeRTOS
{
    constructor(ctx)
    {
        this.ctx = ctx;
        this.viewMap = [
            { name: 'ISR Stack', fxn: this.getSystemStack.bind(this), structName: SystemStack },
            { name: 'Heap Stats', fxn: this.getHeap.bind(this), structName: Heap },
            { name: 'Task Module', fxn: this.getTaskModule.bind(this), structName: TaskModule },
            { name: 'Task Instances', fxn: this.getTaskInstances.bind(this), structName: TaskInstance }
        ];
        this.Program = this.ctx.getProgram();
    }
    async getSystemStack()
    {
        const view = [];
        const stackInfo = new SystemStack();
        // __STACK_END is not present in the GCC compiler...just TI's
        const ccsCompiler = await this.Program.lookupSymbolValue('__STACK_END');
        stackInfo.Address = '0x' + (await this.Program.lookupSymbolValue('__stack')).toString(16);
        if (ccsCompiler == -1) {
            stackInfo.StackSize = await this.Program.lookupSymbolValue('STACKSIZE');
        }
        else {
            stackInfo.StackSize = await this.Program.lookupSymbolValue('__STACK_SIZE');
        }
        const stackData = await this.Program.fetchArray('8u', stackInfo.Address, stackInfo.StackSize);
        let index = 0;
        // Find the first non-0xa5.
        while (stackData[index] == 0xa5) {
            index++;
        }
        if (index < 4) {
            // Might want to add a symbol to figure out that stack init was
            // not initialized.
            stackInfo.StackPeak = 'Stack Overflow \
            (or configENABLE_ISR_STACK_INIT is 0 in FreeRTOSConfig.h)';
        }
        else {
            stackInfo.StackPeak = stackInfo.StackSize - index;
        }
        view.push(stackInfo);
        return (view);
    }
    async getHeap()
    {
        const view = [];
        const heapInfo = new Heap();
        try {
            let current = await this.Program.fetchVariable('xStart');
            const end = await this.Program.fetchVariable('pxEnd');
            const ucHeapType = await this.Program.lookupTypeByVariable('ucHeap');
            heapInfo.HeapAddr = '0x' + (await this.Program.lookupSymbolValue('ucHeap')).toString(16);
            heapInfo.HeapType = 'heap_4';
            if (end == 0) {
                heapInfo.TotalSize = 'Heap not initialized';
            }
            else {
                heapInfo.TotalSize = ucHeapType.elnum; // sizeof(BlockLink_t)
                heapInfo.FirstFreeBlock = current.pxNextFreeBlock;
                while (current.pxNextFreeBlock < end) {
                    current = await this.Program.fetchFromAddr(current.pxNextFreeBlock, 'BlockLink_t');
                    heapInfo.TotalFree = (heapInfo.TotalFree ?? 0) + current.xBlockSize;
                }
            }
            // theia - make FirstFreeBlock to hex
            if (typeof heapInfo.FirstFreeBlock === 'number') {
                heapInfo.FirstFreeBlock = '0x' + heapInfo.FirstFreeBlock.toString(16);
            }
        }
        catch (e) {
            heapInfo.HeapType = 'Heap implementation not supported in ROV (currently only heap_4 is)';
        }
        view.push(heapInfo);
        return (view);
    }
    async getTaskModule()
    {
        const view = [];
        const taskInfo = new TaskModule();
        const readyList = await this.Program.fetchVariable('pxReadyTasksLists');
        taskInfo.NumPriorities = readyList.length;
        taskInfo.NumTasks = await this.Program.fetchVariable('uxCurrentNumberOfTasks');
        view.push(taskInfo);
        return (view);
    }
    async getTaskInstances()
    {
        const table = [];
        const readyList = await this.Program.fetchVariable('pxReadyTasksLists');
        for (let i = 0; i < readyList.length; i++) {
            await this.fillInTaskInstance(table, readyList[i], 'Ready');
        }
        const suspendedList = await this.Program.fetchVariable('xSuspendedTaskList');
        await this.fillInTaskInstance(table, suspendedList, 'Blocked');
        const delay1List = await this.Program.fetchVariable('xDelayedTaskList1');
        await this.fillInTaskInstance(table, delay1List, 'Delayed');
        const delay2List = await this.Program.fetchVariable('xDelayedTaskList2');
        await this.fillInTaskInstance(table, delay2List, 'Delayed');
        const terminatedList = await this.Program.fetchVariable('xTasksWaitingTermination');
        await this.fillInTaskInstance(table, terminatedList, 'Terminated');
        table.sort(compareAddress);
        return (table);
    }
    async fillInTaskInstance(table, list, state)
    {
        const currentTask = await this.Program.fetchVariable('pxCurrentTCB');
        let tcbBase = list.xListEnd.pxNext - 4;
        for (let i = 0; i < list.uxNumberOfItems; i++) {
            const task = await this.Program.fetchFromAddr(tcbBase, 'TCB_t');
            const taskInfo = new TaskInstance();
            // Commenting out since currently the task function is stored on the
            // stack and generally is wiped out. Maybe when we get CallStack we
            // can use it to get the entry function.
            // var nameAddr = tcbBase - 24;
            // var functionAddr = Program.fetchFromAddr(nameAddr, "uintptr_t");
            taskInfo.Address = '0x' + tcbBase.toString(16);
            let name = '';
            for (let j = 0; j < 12; j++) {
                if (task.pcTaskName[j] == 0)
                    break;
                name = name + String.fromCharCode(task.pcTaskName[j]);
            }
            taskInfo.TaskName = name;
            // taskInfo.FxnName = String(Program.lookupFuncName(functionAddr));
            taskInfo.Priority = task.uxPriority;
            taskInfo.BasePriority = task.uxBasePriority;
            if (tcbBase == currentTask) {
                taskInfo.State = 'Running';
            }
            else {
                taskInfo.State = state;
            }
            taskInfo.StackBase = task.pxStack;
            taskInfo.CurrentTaskSP = task.pxTopOfStack;
            // We don't know the size of the task stack, so look every 4 bytes :(
            // The stack size is stored in the "malloc" header right before the
            // stack (for dynamically allocated stacks). Tmr Svc and IDLE tasks
            // stack size probably can be queried from the target. These can be
            // used to improve the performance (e.g. read the whole stack instead
            // a word at a time!).
            let stackData = await this.Program.fetchArray('8u', task.pxStack, 4);
            let index = task.pxStack;
            // Find the first non-0xa5.
            while ((stackData[0] == 0xa5) &&
                (stackData[1] == 0xa5) &&
                (stackData[2] == 0xa5) &&
                (stackData[3] == 0xa5)) {
                index += 4;
                stackData = await this.Program.fetchArray('8u', index, 4);
            }
            // The r4, r5, r6, r7, r8, r9, r10, r11, r14 registers could have
            // had 0xa5a5a5a5 in them and make the stack "shrink" when they
            // are written back onto the stack. This happens in the
            // xPortPendSVHandler function.
            // To error on the safe side, subtract 8 registers * 4 bytes
            // from the index.
            // TIRTOS-2091 covers this in more details.
            index -= 8 * 4;
            if ((taskInfo.StackBase >= index) ||
                (taskInfo.StackBase >= taskInfo.CurrentTaskSP)) {
                taskInfo.UnusedStackSize = 'Stack Overflow';
            }
            else {
                index -= 4;
                taskInfo.UnusedStackSize = index - taskInfo.StackBase;
            }
            // theia - make StackBase, CurrentTaskSP to hex
            if (typeof taskInfo.StackBase === 'number') {
                taskInfo.StackBase = '0x' + taskInfo.StackBase.toString(16);
            }
            if (typeof taskInfo.CurrentTaskSP === 'number') {
                taskInfo.CurrentTaskSP = '0x' + taskInfo.CurrentTaskSP.toString(16);
            }
            table.push(taskInfo);
            // Traverse the list
            tcbBase = task.xStateListItem.pxNext - 4;
        }
    }
    getModuleName() {
        return 'FreeRTOS';
    }
}
function compareAddress(a, b)
{
    return +(a.Address ?? 0) - +(b.Address ?? 0);
}
// exports.viewMap = [
//     {name: "ISR Stack", fxn: getSystemStack, structName: SystemStack},
//     {name: "Heap Stats", fxn: getHeap, structName: Heap},
//     {name: "Task Module", fxn: getTaskModule, structName: TaskModule},
//     {name: "Task Instances", fxn: getTaskInstances, structName: TaskInstance}
// ];
// exports.moduleName = moduleName;
exports.classCtor = FreeRTOS;
