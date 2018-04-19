/*
 * rom.c
 *
 *  Created on: Apr 12, 2018
 *      Author: Connor Shapiro
 */


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "inc/cartridge.h"

#define SUCCESS         (0)
#define MHZ_120         (120000000)
#define DELAY_125_NS    (5)
#define CSn_HI          GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5
#define CSn_LOW         GPIO_PORTM_BASE, GPIO_PIN_5, 0
#define RDn_HI          GPIO_PORTM_BASE, GPIO_PIN_7, GPIO_PIN_7
#define RDn_LOW         GPIO_PORTM_BASE, GPIO_PIN_7, 0
#define WRn_HI          GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3
#define WRn_LOW         GPIO_PORTB_BASE, GPIO_PIN_3, 0
#define RESETn_HI       GPIO_PORTK_BASE, GPIO_PIN_4, GPIO_PIN_4
#define RESETn_LOW      GPIO_PORTK_BASE, GPIO_PIN_4, 0
#define ALL_GPIO_PINS   (0xFF)
#define ROM_DATA_BUS    GPIO_PORTA_BASE, ALL_GPIO_PINS
#define A_MASK          (0x0000FFFF)
#define BA_SHIFT        (16)
#define BA7_MASK        (0x80)
#define BA6_MASK        (0x40)
#define BA5_MASK        (0x20)
#define BA4_MASK        (0x10)
#define BA3_MASK        (0x08)
#define BA2_MASK        (0x04)
#define BA1_MASK        (0x02)
#define BA0_MASK        (0x01)
#define BA7_SHIFT       (5)
#define BA6_SHIFT       (6)
#define BA5_SHIFT       (1)
#define BA4_SHIFT       (2)
#define BA3_SHIFT       (2)
#define BA2_SHIFT       (2)
#define A15_MASK        (0x8000)
#define A14_MASK        (0x4000)
#define A13_MASK        (0x2000)
#define A12_MASK        (0x1000)
#define A11_MASK        (0x0800)
#define A10_MASK        (0x0400)
#define A9_MASK         (0x0200)
#define A8_MASK         (0x0100)
#define A7_MASK         (0x0080)
#define A6_MASK         (0x0040)
#define A5_MASK         (0x0020)
#define A4_MASK         (0x0010)
#define A3_MASK         (0x0008)
#define A2_MASK         (0x0004)
#define A1_MASK         (0x0002)
#define A0_MASK         (0x0001)
#define A15_SHIFT       (13)
#define A14_SHIFT       (10)
#define A13_SHIFT       (10)
#define A12_SHIFT       (8)
#define A11_SHIFT       (5)
#define A10_SHIFT       (3)
#define A9_SHIFT        (8)
#define A8_SHIFT        (2)
#define A7_SHIFT        (4)
#define A6_SHIFT        (5)
#define A5_SHIFT        (5)
#define A4_SHIFT        (2)
#define A3_SHIFT        (2)
#define A2_SHIFT        (2)
#define A1_SHIFT        (4)
#define A0_SHIFT        (5)
#define PORTB_ENC       (((ba_bus & BA5_MASK) >> BA5_SHIFT) | ((ba_bus & BA3_MASK) << BA3_SHIFT))
#define PORTD_ENC       ((ba_bus & BA7_MASK) >> BA7_SHIFT)
#define PORTH_ENC       (((a_bus & A5_MASK) >> A5_SHIFT) | ((a_bus & A6_MASK) >> A6_SHIFT))
#define PORTK_ENC       (((ba_bus & BA2_MASK) >> BA2_SHIFT) | (ba_bus & BA1_MASK) | ((a_bus & A15_MASK) >> A15_SHIFT) | ((a_bus & A13_MASK) >> A13_SHIFT) | ((a_bus & A0_MASK) << A0_SHIFT) | ((a_bus & A8_MASK) >> A8_SHIFT) | ((a_bus & A10_MASK) >> A10_SHIFT))
#define PORTM_ENC       (((a_bus & A2_MASK) >> A2_SHIFT) | ((a_bus & A3_MASK) >> A3_SHIFT) | ((a_bus & A4_MASK) >> A4_SHIFT) | ((a_bus & A11_MASK) >> A11_SHIFT))
#define PORTN_ENC       ((a_bus & A12_MASK) >> A12_SHIFT)
#define PORTP_ENC       (((ba_bus & BA6_MASK) >> BA6_SHIFT) | ((ba_bus & BA4_MASK) >> BA4_SHIFT) | ((a_bus & A7_MASK) >> A7_SHIFT) | ((a_bus & A14_MASK) >> A14_SHIFT) | ((a_bus & A1_MASK) << A1_SHIFT))
#define PORTQ_ENC       ((ba_bus & BA0_MASK) | ((a_bus & A9_MASK) >> A9_SHIFT))

uint8_t read_rom_byte(uint32_t address)
{
    uint16_t a_bus = (uint16_t)(address & A_MASK);
    uint8_t ba_bus = (uint8_t)(address >> BA_SHIFT);

    MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5, PORTB_ENC);
    MAP_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, PORTD_ENC);
    MAP_GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1, PORTH_ENC);
    MAP_GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2
             | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, PORTK_ENC);
    MAP_GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2
                                                     | GPIO_PIN_6, PORTM_ENC);
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_4, PORTN_ENC);
    MAP_GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3
                                         | GPIO_PIN_4 | GPIO_PIN_5, PORTP_ENC);
    MAP_GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_1, PORTQ_ENC);
    MAP_GPIOPinWrite(RDn_LOW);
    MAP_SysCtlDelay(DELAY_125_NS);        // Supposed to provide at least 120ns
    return MAP_GPIOPinRead(ROM_DATA_BUS); // delay for FastROM cart
}

cart_e_t setup_cartridge(void)
{
    /* Configure sysclock to max (120MHz) */
    MAP_SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN
                              | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, MHZ_120);

    /* First off, hold cartridge in reset */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_4);
    MAP_GPIOPinWrite(RESETn_LOW);

    /* Configure GPIO for SNES MROM Reading */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, ALL_GPIO_PINS);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3 | GPIO_PIN_4 |
                                                                  GPIO_PIN_5);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, ALL_GPIO_PINS);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                          GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_4);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTP_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                       GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    MAP_GPIOPinWrite(WRn_HI);
    MAP_GPIOPinWrite(RDn_HI);
    MAP_GPIOPinWrite(CSn_LOW);

    /* Release reset */
    MAP_GPIOPinWrite(RESETn_HI);

    return CART_E_OK;
}

cart_e_t dump_rom(uint8_t * buf)
{
    return CART_E_OK;
}
