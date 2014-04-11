/*
 * task.h
 *
 *  Created on: 05.03.2014
 *      Author: Michael
 */

#ifndef TASK_H_
#define TASK_H_

#include <boss.h>

typedef enum {
	INVALID = 0,
	READY,
	RUNNING,
	BLOCKED,
	SLEEPING
} TaskState;

typedef struct {
	// process identification
	uint32_t pid;

	// processor state information
	uint32_t* pc;
	uint32_t reg[15];
	uint32_t cpsr;

	// process control information
	TaskState state;

	// time to sleep
	uint32_t sleepUntil;
} Task;

#endif /* TASK_H_ */
