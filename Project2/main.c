

/**
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "SNES_Controller/controller.h"

int main(void)
{

    extern int32_t read_value;
    enable_peripherals();

    while(1)
    {
        send_signal();
    }

}
