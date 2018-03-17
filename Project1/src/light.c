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
#include <linux/i2c-dev.h>
#include <error.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
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
	DATA[0]=(COMMAND_REG|CONTROL);
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
        DATA[0]=(COMMAND_REG|TIMING);
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
        DATA[0]=(COMMAND_REG|INTERRUPT);
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


	data[0]=(COMMAND_REG|CONTROL);
        i2c_write(SLAVE_ADDRESS,data);
	data[0]=(COMMAND_REG|CONTROL);
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

	data[0]=(COMMAND_REG|TIMING);
        i2c_write(SLAVE_ADDRESS,data);
        data[0]=(COMMAND_REG|TIMING);
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

	data[0]=(COMMAND_REG|INTERRUPT);
        i2c_write(SLAVE_ADDRESS,data);
	data[0]=(COMMAND_REG|INTERRUPT);
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
	data[0]=(COMMAND_REG|ID);
        i2c_write(SLAVE_ADDRESS,data);
	data[0]=(COMMAND_REG|ID);
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

light_e_t read_adc_reg (uint8_t channel, char data[])
{


	char DATA[2];

	if(channel==0)

	{
		DATA[0]=(COMMAND_WORD|DATA0LOW);
		i2c_write(SLAVE_ADDRESS,DATA);
		DATA[0]=(COMMAND_WORD|DATA0LOW);
		i2c_read(SLAVE_ADDRESS,DATA);
		data[0]=DATA[0];
		data[1]=DATA[1];

	}
	else if(channel==1)
	{
		DATA[0]=(COMMAND_WORD|DATA1LOW);
                i2c_write(SLAVE_ADDRESS,DATA);
                DATA[0]=(COMMAND_WORD|DATA1LOW);
                i2c_read(SLAVE_ADDRESS,DATA);
		data[0]=DATA[0];
                data[1]=DATA[1];

	}

}
/**
* @function convert_light
*
* @brief reads data from the adc reg
*
* @param void
*
* @return float
*/
float convert_light (void)
{

write_ctrl_reg(0x03);

char data1[2];
char data2[2];

read_adc_reg(0,data1);
read_adc_reg(1,data2);

uint8_t ADC1H;
uint8_t ADC1L;
uint8_t ADC0H;
uint8_t ADC0L;

ADC1H=data2[1];
ADC1L=data2[0];
ADC0H=data1[1];
ADC0L=data1[0];


uint16_t CH1; 
uint16_t CH0;

CH1=(uint16_t)ADC1H<<8;
CH1=CH1|ADC1L;

CH0=(uint16_t)ADC0H<<8;
CH0=CH0|ADC0L;

float ch1=0.0;
float ch0=0.0;

ch1=CH1;
ch0=CH0;
float sensorlux;

	if(((ch1/ch0)<=0.50)&&((ch1/ch0)>0.0))
	{
	sensorlux=(0.0304*(ch0))-(0.062*(ch0)*(pow((ch1/ch0),1.4)));
	}
	else if(((ch1/ch0)<=0.61)&&((ch1/ch0)>0.50))
	{

	sensorlux=(0.0224*(ch1))-(0.031*(ch0));

	}
	else if(((ch1/ch0)<=0.80)&&((ch1/ch0)>0.61))
        {

        sensorlux=(0.0128*(ch0))-(0.0153*(ch1));

        }
	 else if(((ch1/ch0)<=1.30)&&((ch1/ch0)>0.80))
        {

        sensorlux=(0.00146*(ch0))-(0.00112*(ch1));

        }
	else if((ch1/ch0)>1.30)
	{
	sensorlux=(0.0);
	}
	return sensorlux;

}

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


