#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../inc/cartridge.h"

#define MEBIBITS_2  (2)
#define MEBIBITS_4  (4)
#define MEBIBITS_8  (8)
#define MEBIBITS_16 (16)
#define MEBIBITS_32 (32)

#define KIBIBITS_16 (16)
#define KIBIBITS_32 (32)
#define KIBIBITS_64 (64)

static void check_scheme(void)
{
    expect_value(__wrap_ucRead_rom_byte(ulAddress, 0xFFD5));
    expect_value(__wrap_ucRead_rom_byte(ucSpeed, 8));
}

static void check_speed(void)
{
    expect_value(__wrap_ucRead_rom_byte(ulAddress, 0xFFD5));
    expect_value(__wrap_ucRead_rom_byte(ucSpeed, 8));
}

static void check_size(void)
{
    expect_value(__wrap_ucRead_rom_byte(ulAddress, 0xFFD7));
    expect_value(__wrap_ucRead_rom_byte(ucSpeed, 8));
}

static void check_sram_size(void)
{
    expect_value(__wrap_ucRead_rom_byte(ulAddress, 0xFFD8));
    expect_value(__wrap_ucRead_rom_byte(ucSpeed, 8));
}

static void test_HIROM(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 1);

    check_scheme();

    assert_true(HIROM_VAL == cCart_addr_scheme());
}

static void test_LOWROM(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 0);

    check_scheme();

    assert_true(LOWROM_VAL == cCart_addr_scheme());
}

static void test_bad_scheme(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 2)

    check_scheme();

    assert_true(FAIL == cCart_addr_scheme());
}

static void test_FASTROM(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 3);

    check_speed();

    assert_true(FASTROM_DELAY == ucCart_speed());
}

static void test_SLOWROM(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 0);

    check_speed();

    assert_true(SLOWROM_DELAY == ucCart_speed());
}

static void test_2Mi(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 8);

    check_size();

    assert_true(MEBIBITS_2 == ucCart_size());
}

static void test_4Mi(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 9);

    check_size();

    assert_true(MEBIBITS_4 == ucCart_size());
}

static void test_8Mi(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 10);

    check_size();

    assert_true(MEBIBITS_8 == ucCart_size());
}

static void test_16Mi(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 11);

    check_size();

    assert_true(MEBIBITS_16 == ucCart_size());
}

static void test_32Mi(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 12);

    check_size();

    assert_true(MEBIBITS_32 == ucCart_size());
}

static void test_RAM_16Ki(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 1);

    check_size();

    assert_true(KIBIBITS_16 == ucCart_SRAM_size());
}

static void test_RAM_32Ki(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 2);

    check_size();

    assert_true(KIBIBITS_32 == ucCart_SRAM_size());
}

static void test_RAM_64Ki(void **state)
{
    will_return(__wrap_ucRead_rom_byte, 3);

    check_size();

    assert_true(KIBIBITS_64 == ucCart_SRAM_size());
}

int main(void)
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test_HIROM),
        cmocka_unit_test(test_LOWROM),
        cmocka_unit_test(test_bad_scheme),
        cmocka_unit_test(test_FASTROM),
        cmocka_unit_test(test_SLOWROM),
        cmocka_unit_test(test_2Mi),
        cmocka_unit_test(test_4Mi),
        cmocka_unit_test(test_8Mi),
        cmocka_unit_test(test_16Mi),
        cmocka_unit_test(test_32Mi),
        cmocka_unit_test(test_RAM_16Ki),
        cmocka_unit_test(test_RAM_32Ki),
        cmocka_unit_test(test_RAM_64Ki)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}