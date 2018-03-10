/**
*@file: i2c.h
*
*@description: This header file provides an abstraction
*	 	for reading and writing values in the i2c
*	 	registers.
*
*@Date:2nd March 2017 
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/

#ifndef __I2C_H__
#define __I2C_H__


#include <stdint.h>

/**
* @function i2c_read
*
* @brief reads from an i2c register
*
* Given an address it will read from the 
*       address
*
* @param  uint8_t address : address of the register to read from
*               uint8_t buf[] to store values that are read   
*
* @return void
*/
void  i2c_read(uint8_t address,uint8_t* buf);


/**
* @function i2c_write
*
* @brief writes into an i2c register
*
* Given an address and a pointer to that address,
* 	it writes to the address
*
* @param __user data  to a data item fro
*		user space
* @param value value to write to the location
*
* @return void.
*/

void i2c_write(uint8_t address, uint8_t *data);


#endif /*__I2C_H__ */












