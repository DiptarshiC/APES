/**
*@file: light.c
*
*@description: This c source file provides definitions
*	 	for reading and writing values in the i2c
*	 	light sensor.
*
*@Date:2nd March 2017 
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../includes/i2c.h"
#include "../includes/light.h"

/**
* @function write_cmd_reg
*
* @brief writes to light sensor cmd reg
*
*  
* @param  uint8_t data:data to write to the cmd
*                 register
*
* @return light_e_t
*/

light_e_t write_cmd_reg (uint8_t data);

/**
* @function write_ctrl_reg
*
* @brief writes to light sensor cmd reg
*
*
* @param  uint8_t data:data to write to the ctrl
*                 register
*
* @return light_e_t
*/

light_e_t write_ctrl_reg (uint8_t data)
{

	char DATA[2];
	DATA[0]=(COMMAND|CONTROL);
	DATA[1]=data;
	i2c_write(SLAVE_ADDRESS,DATA);	

}

/**
* @function write_timing_reg
*
* @brief writes to light sensor timing reg
*
*
* @param  uint8_t data:data to write to the timing
*                 register
*
* @return light_e_t
*/

light_e_t write_timing_reg (uint8_t data)
{

	char DATA[2];
        DATA[0]=(COMMAND|TIMING);
        DATA[1]=data;
        i2c_write(SLAVE_ADDRESS,DATA);

}
/**
* @function write_irq_thresh_reg
*
* @brief writes to light sensor  reg
*
*
* @param  uint8_t data:data to write to the irq thresh register
*   
*
* @return light_e_t
*/

light_e_t write_irq_thresh_reg (uint8_t channel, uint16_t data)
{
  /*	char DATA[2];
        DATA[0]=(COMMAND|TIMING);
        DATA[1]=data;
        i2c_write(SLAVE_ADDRESS,DATA);*/

}
/**
* @function write_ctrl_reg
*
* @brief writes to light sensor irq ctrl reg
*
*
* @param  uint8_t data:data to write to the irq ctrl
*                 register
*
* @return light_e_t
*/

light_e_t write_irq_ctrl_reg (uint8_t data)
{

        char DATA[2];
        DATA[0]=(COMMAND|INTERRUPT);
        DATA[1]=data;
        i2c_write(SLAVE_ADDRESS,DATA);

}

/**
* @function read_ctrl_reg
*
* @brief writes to light sensor irq ctrl reg
*
*
* @param  uint8_t *data:read data from the ctrl
*                 register
*
* @return light_e_t
*/

light_e_t read_ctrl_reg (char  data[])
{
        data[0]=(COMMAND|CONTROL);
       
        i2c_read(SLAVE_ADDRESS,data);
}

/**
* @function read_timing_reg
*
* @brief reads data from the timing reg
*
*
* @param  uint8_t *data:read data from the timing
*                 register
*
* @return light_e_t
*/

light_e_t read_timing_reg (char  data[])
{
 	data[0]=(COMMAND|TIMING);
        i2c_read(SLAVE_ADDRESS,data);

}
/**
* @function read_irq_thresh_reg
*
* @brief reads data from the timing reg
*
*
* @param  uint8_t *data:read data from the irq thresh
*                 register
*
* @return light_e_t
*/
light_e_t read_irq_thresh_reg (uint8_t channel, uint16_t * data);

/**
* @function read_irq_ctrl_reg
*
* @brief reads data from the timing reg
*
*
* @param  uint8_t *data:read data from the irq ctrl reg
*                 register
*
* @return light_e_t
*/

light_e_t read_irq_ctrl_reg (char  data[])
{
      data[0]=(COMMAND|INTERRUPT);
      i2c_read(SLAVE_ADDRESS,data);

}


/**
* @function read_id__reg
*
* @brief reads data from the id reg
*
*
* @param  uint8_t *data:read data from the id reg
*                 register
*
* @return light_e_t
*/

light_e_t read_id_reg (char data[])
{
	data[0]=(COMMAND|ID);
      i2c_read(SLAVE_ADDRESS,data);
}

/**
* @function read_adc_reg
*
* @brief reads data from the adc reg
*
*
* @param  uint8_t *data:read data from the adc reg
*                 register
*
* @return light_e_t
*/

light_e_t read_adc_reg (uint8_t channel, uint16_t * data);


/**
* @function convert_light
*
* @brief reads data from the adc reg
*
* @param void
*                 
* @return float
*/
float convert_light (void);

/**
* @function heatbeat
*
* @brief sends messages to main
*
* @param void
*
* @return light_e_t
*/
light_e_t heartbeat (void);

void main()
{

	char data[2];
	printf("Now we shall check read/write functions\n");

	printf("First ill do a write/read check on control register\n");
	
	write_ctrl_reg(0xDA);
	read_ctrl_reg(data);

	printf("The value in the CONTROL register is %x \n",data[0]);


}
