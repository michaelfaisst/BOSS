#include "core/core.h"
#include "scheduler/scheduler.h"
#include "irq/irq.h"
#include "fs/fat32/fat32.h"
#include "mmu/mmu.h"
#include "task/taskLoader.h"

/**
 * Prototypes
 */
static int32_t initHardware( void );
static int32_t initOs( void );
static int32_t initDrivers( void );
static int32_t initSystem( void );

/**
 * Entry-Point of Kernel.
 * NOTE: the entry-point of each module in the kernel is placed on top of the corresponding module-file
 */
int32_t
main( void )
{
	if ( initHardware() )
	{
		// initializing hardware failed, exit OS
		return 1;
	}
	
	if ( initOs() )
	{
		// initializing OS failed, exit OS
		return 2;
	}

	if ( initDrivers() )
	{
		// initializing Drivers failed, exit OS
		return 3;
	}

	if ( initSystem() )
	{
		// initializing OS failed, exit OS
		return 4;
	}

	// the mother of all endless-loops...
	while ( 1 ) {}
}

/**
 * This function initializes the hardware of the beagleboard
 * - power management functionality
 * - interrupts
 */
int32_t
initHardware( void )
{
	if(mmu_init()) {
		return 1;
	}

	return 0;
}

/**
 * This function sets up and initializes all internal os data-structures
 * - tasks & scheduling
 * - address spaces
 * - paging
 * - sys-calls
 * - IPC
 * - ... ?
 *
 */
int32_t
initOs( void )
{
	if ( initCore() )
	{
		return 1;
	}

	if ( irqInit() )
	{
		return 1;
	}

	if( schedInit() )
	{
		return 1;
	}

	return 0;
}

/**
 * This function initializes all driver-servers
 */
int32_t
initDrivers( void )
{
	if( fat32Init() )
	{
		return 1;
	}

	loadTaskFromFile( "boot/hdmidrv.out" );
	loadTaskFromFile( "boot/rs232drv.out" );
	loadTaskFromFile( "boot/console.out" );

	return 0;
}

/**
 * This function initializes all servers necessary for system-functionality:
 * - minimal bash
 * - ...
 */
int32_t
initSystem( void )
{
	// NOTE: at this point we should be able to send through RS232

	irqEnable();

	schedStart();

	return 0;
}
