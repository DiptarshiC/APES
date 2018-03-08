#ifndef _LIGHT_H_
#define _LIGHT_H_

/****************************************************************
 * light.h - Function declarations and error enumeration
 *		 for APDS-9301 I2C light sensor on BeagleBone B/G
 * ECEN5013 Project 1 - Chakraborty/Shapiro
 ***************************************************************/

typedef enum light_e    // Error enum for our light sensor module
{
    LIGHT_NO_ERROR
} light_e_t;

/*-----------------------------------------------------------------------------
 * write_cmd_reg()
 *    description:
 *				Write to COMMAND "register" (to specify operand register)
 *    input:
 *				uint8_t data = Byte to write to "register"
 *    output:
 *              light_e_t ret = Light sensor error code enum
 *---------------------------------------------------------------------------*/
light_e_t write_cmd_reg(uint8_t data);

/*-----------------------------------------------------------------------------
 * write_ctl_reg()
 *    description:
 *				Write to CONTROL register (0h)
 *    input:
 *				uint8_t data = Byte to write to register
 *    output:
 *              light_e_t ret = Light sensor error code enum
 *---------------------------------------------------------------------------*/
light_e_t write_ctl_reg(uint8_t data);

/*-----------------------------------------------------------------------------
 * write_timing_reg()
 *    description:
 *				Write to TIMING register (1h)
 *    input:
 *				uint8_t data = Byte to write to register
 *    output:
 *              light_e_t ret = Light sensor error code enum
 *---------------------------------------------------------------------------*/
light_e_t write_timing_reg(uint8_t data);


#endif // _LIGHT_H_