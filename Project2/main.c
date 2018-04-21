/**
 * main.c
 *  ECEN 5013 (S18) Project 2 - SNES Streamer
 *  Authors: Diptarshi Chakraborty and Connor Shapiro
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "SNES_Controller/controller.h"
#include "inc/cartridge.h"
#include "FreeRTOS.h"
#include "portable.h"
#include "queue.h"
#include "portmacro.h"
#include "task.h"

#define MHZ_120 (120000000)

static void vSystem_init (void);

int main(void)
{
    /* Startup config */
    vSystem_init();

    /* Create tasks */
    xTaskCreate(vCartridgeTask, "Cartridge I/O Task", CART_STACK_DEPTH, NULL,
                                                            CART_PRIO, NULL);
    vTaskStartScheduler();

    while(1)
    {
        // Do nothing
    }

}

static void vSystem_init(void)
{
    /* Configure sysclock to max (120MHz) */
    MAP_SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN
                              | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, MHZ_120);
}
