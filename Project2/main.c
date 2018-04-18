

/**
 * main.c
 */

#include "SNES_Controller/controller.h"

int main(void)
{

    void enable_peripherals();

    while(1)
    {
        send_signal();
    }

}
