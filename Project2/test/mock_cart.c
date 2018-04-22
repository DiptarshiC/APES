#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../inc/cartridge.h"

#define static

uint8_t __wrap_ucRead_rom_byte(uint32_t ulAddress, uint8_t ucSpeed)
{
    check_expected(ulAddress);
    check_expected(ucSpeed);

    return mock_type(uint8_t);
}

