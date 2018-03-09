/**
*@file: i2c.c
*
*@description: this file contains the defintions of
*		i2c read and write functions
*
*@Date:2nd March 2017
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/



#include <stdint.h>
#include <../includes/i2c.h>
#include <<linux/i2c-dev.h>

/**
* @function i2c_read
*
* @brief reads from an i2c register
*
* Given an address it will read from the 
*       address
*
* @param uint8_t address : address of the register to read from
*              
*
* @return uint8_t *
*/
uint8_t * i2c_read(uint8_t address)
{

}

/**
* @function i2c_write
*
* @brief writes into an i2c register
*
* Given an address and a pointer to that address,
* 	it writes to the address
*
* @param __user data Pointer to a data item fro
*		user space
* @param value value to write to the location
*
* @return void.
*/

void i2c_write(uint8_t address, uint8_t *data)
{




}



	
	





