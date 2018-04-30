#include <stdlib.h>



#include <stdarg.h>

#include <stddef.h>

#include <setjmp.h>

#include "cmocka.h"

//#include "quadratic.h"
#include "controller.h"

#include "cartridge.h"


void test_controller(void **state)

{



  uint32_t status = usReadController(0x0001);

  assert_int_equal(status, 2048);

  




}

void test_cartridge(void **state)

{



  uint8_t status = ucRead_rom_byte(0x0001);

  assert_int_equal(status, 0);

}

void test_cartridge1(void **state)

{

  uint16_t status = ucRead_rom_byte(0x0001);

  assert_int_equal(status, 0);


}


int main(int argc, char **argv)

{

  const struct CMUnitTest tests[] = {

    cmocka_unit_test(test_controller),
    cmocka_unit_test(test_cartridge),
    cmocka_unit_test(test_cartridge1),


};



  return cmocka_run_group_tests(tests, NULL, NULL);

}
 

