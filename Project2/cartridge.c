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
#include "inc/portable.h"
#include "queue.h"

#define FAIL            (-1)
#define CLEAR_ALL       (0xFFFFFFFF)
#define FASTROM_DELAY   (5)
#define SLOWROM_DELAY   (8)
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
#define PORTB_ENC       (((ucBA_bus & BA5_MASK) >> BA5_SHIFT) | ((ucBA_bus & BA3_MASK) << BA3_SHIFT))
#define PORTD_ENC       ((ucBA_bus & BA7_MASK) >> BA7_SHIFT)
#define PORTH_ENC       (((ulA_bus & A5_MASK) >> A5_SHIFT) | ((ulA_bus & A6_MASK) >> A6_SHIFT))
#define PORTK_ENC       (((ucBA_bus & BA2_MASK) >> BA2_SHIFT) | (ucBA_bus & BA1_MASK) | ((ulA_bus & A15_MASK) >> A15_SHIFT) | ((ulA_bus & A13_MASK) >> A13_SHIFT) | ((ulA_bus & A0_MASK) << A0_SHIFT) | ((ulA_bus & A8_MASK) >> A8_SHIFT) | ((ulA_bus & A10_MASK) >> A10_SHIFT))
#define PORTM_ENC       (((ulA_bus & A2_MASK) >> A2_SHIFT) | ((ulA_bus & A3_MASK) >> A3_SHIFT) | ((ulA_bus & A4_MASK) >> A4_SHIFT) | ((ulA_bus & A11_MASK) >> A11_SHIFT))
#define PORTN_ENC       ((ulA_bus & A12_MASK) >> A12_SHIFT)
#define PORTP_ENC       (((ucBA_bus & BA6_MASK) >> BA6_SHIFT) | ((ucBA_bus & BA4_MASK) >> BA4_SHIFT) | ((ulA_bus & A7_MASK) >> A7_SHIFT) | ((ulA_bus & A14_MASK) >> A14_SHIFT) | ((ulA_bus & A1_MASK) << A1_SHIFT))
#define PORTQ_ENC       ((ucBA_bus & BA0_MASK) | ((ulA_bus & A9_MASK) >> A9_SHIFT))
#define ROM_SCHEME_ADDR (0xFFD5)    // Contains ROM access speed and addressing scheme
#define ROM_TYPE_ADDR   (0xFFD6)    // Information about what components the cartridge has
#define ROM_SIZE_ADDR   (0xFFD7)
#define MEBIBIT_2_BYTE  (131072)
#define BANK_SHIFT      (15)
#define LO_PAGE_SIZE    (0x8000)
#define UPPER_NIBBLE    (0xF0)
#define LOWER_NIBBLE    (0x0F)
#define HIROM_VAL       (1)
#define LOWROM_VAL     (0)
#define FASTROM_VAL     (3)

typedef enum
{
    CART_E_OK,
    CART_E_FAIL
} cart_e_t;

extern QueueHandle_t xMROM_Queue;
static uint8_t ucRead_rom_byte (uint32_t ulAddress, uint8_t ucSpeed);
static void vCartridge_init (void);
static int8_t ucCart_addr_scheme (void);
static uint8_t ucCart_speed (void);
static uint8_t ucCart_size (void); // Returns cartridge MROM size (in Mebibits)
static uint8_t ucCart_SRAM_size (void); // Returns SRAM size (in Kibibits)

void vCartridgeTask(void *pvParameters)
{
    bool xTaskExit;
    uint32_t ulNotificationValue;
    uint8_t ucSpeed;
    uint8_t ucSize;
    uint32_t uli_rom;
    uint16_t usi_bank;
    uint8_t ucData;

    /* Get ready for operations and enter task loop */
    vCartridge_init();
    xTaskExit = false;

    /* Cartridge Task main loop */
    while(!xTaskExit)
    {
        /* Block until a notification is received */
        xTaskNotifyWait(CLEAR_ALL, CLEAR_ALL, &ulNotificationValue,
                                                                portMAX_DELAY);

        /* If notification was for graceful exit, do so ignoring other cmds */
        if (ulNotificationValue & EXIT_MASK)
        {
            xTaskExit = true;
        }

        else
        {
            if (ulNotificationValue & READ_MROM_MASK)
            {
                /* Get ROM speed & size */
                ucSpeed = ucCart_speed();
                ucSize = MEBIBIT_2_BYTE * ucCart_size();

                /* Dump HiROM, blocking until queue is emptied when full */
                if (ucCart_addr_scheme == HIROM_VAL)
                {
                    for (uli_rom = 0; uli_rom < ucSize; uli_rom++)
                    {
                        ucData = ucRead_rom_byte(uli_rom, ucSpeed);
                        xQueueSend(xMROM_Queue, &ucData, portMAX_DELAY);
                    }
                }

                else if (ucCart_addr_scheme == LOWROM_VAL)
                {   // For LowROM reads, read the upper half of each bank
                    for (usi_bank = 0; usi_bank < (ucSize >> BANK_SHIFT);
                                                                    usi_bank++)
                    {
                        for (uli_rom = LO_PAGE_SIZE; uli_rom < 
                                                (2 * LO_PAGE_SIZE); uli_rom++)
                        {
                            ucData = ucRead_rom_byte(((usi_bank * LO_PAGE_SIZE)
                                                         + uli_rom), ucSpeed);
                            xQueueSend(xMROM_Queue, &ucData, portMAX_DELAY);
                        }
                    }
                }

                else if (ucCart_addr_scheme == FAIL)
                {
                    // Tell user to cycle cartridge
                }
            }
        }
    }

    /* Exit Task */
    vTaskDelete(NULL);
}

static uint8_t ucRead_rom_byte(uint32_t ulAddress, uint8_t ucSpeed)
{
    uint16_t ulA_bus = (uint16_t)(ulAddress & A_MASK);   // Address
    uint8_t ucBA_bus = (uint8_t)(ulAddress >> BA_SHIFT); // Bank address

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

    /* MROM access time - 120ns for FastROM (I delay 125), 200ns for SlowROM */
    MAP_SysCtlDelay(ucSpeed);

    return MAP_GPIOPinRead(ROM_DATA_BUS);
}

static void vCartridge_init(void)
{
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
    // No need to enable GPIOK as this was already done above
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
}

static uint8_t ucCart_addr_scheme(void)
{
    /* Check if ROM is HiROM */
    if ((ucRead_rom_byte(ROM_SCHEME_ADDR, SLOWROM_DELAY) & LOWER_NIBBLE) ==
                                                                    HIROM_VAL)
    {
        return HIROM_VAL;
    }
    /* Check if ROM is LowROM */
    else if ((ucRead_rom_byte(ROM_SCHEME_ADDR, SLOWROM_DELAY) & LOWER_NIBBLE)
                                                                == LOWROM_VAL)
    {
        return LOWROM_VAL;
    }
    /* Otherwise cartridge contacts are likely dirty or some other issue */
    else
    {
        return FAIL;
    }
}

static uint8_t ucCart_speed(void)
{
    /* Check if ROM is FastROM */
    if ((ucRead_rom_byte(ROM_SCHEME_ADDR, SLOWROM_DELAY) & UPPER_NIBBLE) ==
                                                                FASTROM_VAL)
    {
        return FASTROM_DELAY;
    }
    /* Assume SlowROM if not */
    else
    {
        return SLOWROM_DELAY;
    }
}

static uint8_t ucCart_size(void)
{
    return (1 << (ucRead_rom_byte(ROM_SIZE_ADDR, SLOWROM_DELAY) - 7));
}

static uint8_t ucCart_SRAM_size(void)
{
    return (1 << (ucRead_rom_byte(ROM_SIZE_ADDR, SLOWROM_DELAY) + 3));
}