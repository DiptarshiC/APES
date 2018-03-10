/**
*@file: temp.c
*
*@description: This c source file provides definitions
*	 	for reading and writing values in the i2c
*	 	temp sensor.
*
*@Date:2nd March 2017 
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../includes/i2c.h"
#include "../includes/temp.h"



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

temperature_e_t read_temperature_reg (uint8_t data)
{

	 	
	data=(POINTER_REG|TEMP_REG_ADDR);
	i2c_read( DEV_ADDR,&data);
}


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

temperature_e_t read_config_reg (uint8_t data)
{
	 	
        data=(POINTER_REG| CONF_REG_ADDR);
        i2c_read( DEV_ADDR,&data);
}
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

temperature_e_t read_tlow_reg (uint8_t data)
{

	 
	data=(POINTER_REG|TLOW_REG_ADDR) ;
        i2c_read( DEV_ADDR,&data);

}

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

temperature_e_t read_thi_reg (uint8_t data)
{
	
	  
	data=(POINTER_REG| THIGH_REG_ADDR) ;
        i2c_read( DEV_ADDR,&data);	

}


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

void main()
{

	uint8_t MSB=0;
	uint8_t LSB=0;
	uint8_t *msb=NULL;
	uint8_t *lsb=NULL;
	uint8_t temp=0;
	float f=0;
	float c=0;
	uint8_t  buf;
	while(1)
	{
      
	
	read_temperature_reg(buf);
	
	msb=&(buf);
	lsb=msb+1;
	
	MSB=*msb;
	LSB=*lsb;

       /* Convert 12bit int using two's compliment */
       /* Credit: http://bildr.org/2011/01/tmp102-arduino */
       temp = ((MSB << 8) | LSB) >> 4;

       c = temp*0.0625;
       f = (1.8 * c) + 32;

       	printf("Temp Fahrenheit: %f Celsius: %f\n", f, c);
	sleep(1);
	}


}
