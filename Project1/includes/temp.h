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



#define POINTER_REG 	0x00
#define DEV_ADDR 	0x48
#define TEMP_REG_ADDR 	0x00
#define CONF_REG_ADDR 	0x01
#define TLOW_REG_ADDR 	0x02
#define THIGH_REG_ADDR 	0x03




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

	TEMP_NO_ERR,
	TEMP_BAD_PARAM


}temperature_e_t;


/**
* @function write_ptr_reg
*
* @brief writes to temp sensor ptr reg
*
*  
*      
*
* @param  uint8_t data:data to write to the pointer
*                 register
*
* @return temperature_e_t
*/

temperature_e_t write_ptr_reg (uint8_t data);

/**
* @function write_conf_reg
*
* @brief writes into the config reg
*
* 
*       
*
* @param  uint8_t data:data to write to the config
*                 register
*
* @return temperature_e_t
*/

temperature_e_t write_conf_reg (uint16_t data);


/**
* @function write_tlow_reg
*
* @brief writes into the tlow reg
*
* @param  uint8_t data:data to write to the tlow
*                 register
*
* @return temperature_e_t
*/

temperature_e_t write_tlow_reg (uint16_t data);


/**
* @function write_thi_reg
*
* @brief writes into the thi reg
*
*
* @param  uint8_t data:data to write to the thi
*                 register
*
* @return temperature_e_t
*/

temperature_e_t write_thi_reg (uint16_t data);

/**
* @function read_temperature_reg
*
* @brief reads from the temp reg
*
*
* @param  uint8_t *data:ptr to store data
*                 
* @return temperature_e_t
*/

temperature_e_t read_temperature_reg (uint8_t data);


/**
* @function read_config_reg
*
* @brief reads from the config reg
*
*
* @param  uint8_t data:pointer to store data
*                 
*
* @return temperature_e_t
*/

temperature_e_t read_config_reg (uint8_t data);


/**
* @function tlow_config_reg
*
* @brief reads from the tlow reg
*
*
* @param  uint8_t data:pointer to store data
*
*
* @return temperature_e_t
*/

temperature_e_t read_tlow_reg (uint8_t data);

/**
* @function read_thi_reg
*
* @brief reads from the thi reg
*
*
* @param  uint8_t data:pointer to store data
*
*
* @return temperature_e_t
*/

temperature_e_t read_thi_reg (uint8_t data);

/**
* @function read_thi_reg
*
* @brief reads from the thi reg
*
* @param  uint8_t data:pointer to store data
*
* @return temperature_e_t
*/

temperature_e_t read_thi_reg (uint8_t data);

/**
* @function timeout
*
* @brief reads from Regularly gathers new
*	 temperature data after a set
*	 period of time without new
*	data.
*
* @param  void
*
* @return temperature_e_t
*/

temperature_e_t timeout (void);


#endif /*__TEMP_H__ */
