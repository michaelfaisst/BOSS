/*
 * scheduler.c
 *
 *  Created on: 26.02.2014
 *      Author: Michael
 */

#include <stdio.h>
#include <string.h>

#include "scheduler.h"
#include "../common/common.h"
#include "../timer/timer.h"
#include "../task/task.h"
#include "../task/taskTable.h"

// prototypes of this module
uint32_t getNextReady();
int32_t createTask( task_func entryPoint );

static uint32_t runningPID = 0;

uint32_t
initScheduler()
{
	// want IRQ from timer every 1000ms
	timerInit( TIMER2_ID, 1000 );

	// NOTE: need to waste some time, otherwise IRQ won't hit
	volatile uint32_t i = 100000;
	while ( i > 0 )
		--i;

	return 0;
}

uint32_t
schedule( UserContext* ctx )
{
	uint32_t ret = 0;

	Task* runningTask = getTask( runningPID );
	if ( runningTask->state == RUNNING )
	{
		runningTask->state = READY;
		runningTask->pc = ctx->pc;
		runningTask->cpsr = ctx->cpsr;
		memcpy( runningTask->reg, ctx->regs, sizeof( runningTask->reg ) );
	}

	uint32_t nextPID = getNextReady();
	Task* task = getTask( nextPID );
	if ( task->state == READY )
	{
		task->state = RUNNING;
		ctx->pc = task->pc;
		ctx->cpsr = task->cpsr;
		memcpy( ctx->regs, task->reg, sizeof( task->reg ) );

		runningPID = nextPID;

		// signal: a task was scheduled
		ret = 1;
	}

	return ret;
}

uint32_t
getNextReady()
{
	uint32_t i = 0;
	uint32_t pid = 0;

	for ( i = 1; i <= MAX_TASKS; i++ )
	{
		pid = ( runningPID + i ) % MAX_TASKS ;

		if( READY == getTask( pid )->state )
		{
			return pid;
		}
	}

	return runningPID;
}

int32_t
createTask( task_func entryPoint )
{
	Task newTask;
	newTask.state = READY;
	newTask.pid = getNextFreePID();
	newTask.pc = ( uint32_t* ) entryPoint;
	newTask.pc++; // TODO: move to IRQ handler in future
	newTask.cpsr = 0x60000110; // user-mode and IRQs enabled

	// TODO: need a valid stack-pointer
	void* stackPtr = ( void*) malloc( 1024 );
	memset( stackPtr, 'a', 1024 );

	newTask.reg[ 13 ] = ( uint32_t ) stackPtr;

	addTask( &newTask );

	return 0;
}

int32_t
fork()
{
	return 0;
}

int32_t
sleep( uint32_t millis )
{
	return 0;
}
