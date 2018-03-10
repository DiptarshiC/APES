/**
*@file: temp.h
*
*@description: This header file provides an abstraction
*	 	for reading and writing values in the i2c
*	 	temp sensor.
*
*@Date:2nd March 2017 
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/

#ifndef __TEMP_H__
#define __TEMP_H__

#include <stdint.h>


/**
*@brief temperature_e_t is a specialized
*	enum that is used to return
*	error codes for the 
*	temperature functions that 
*	are declared in this file.
*
*/

typedef enum
{

	TEMP_NO_ERR;
	TEMP_BAD_PARAM;


}temperature_e_t;


/**
* @function i2c_read
*
* @brief reads from an i2c register
*
* Given an address it will read from the 
*       address
*
* @param  uint8_t data:data to write to the pointer
*                 register
*
* @return temperature_e_t
*/

temperature_e_t write_ptr_reg (uint8_t data);


#endif /*__TEMP_H__ */
