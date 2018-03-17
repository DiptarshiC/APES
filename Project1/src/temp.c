
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


uint8_t LSB;
uint8_t MSB;

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

temperature_e_t write_conf_reg (uint8_t MSB, uint8_t LSB)
{

	char DATA[3];
	DATA[0]=(POINTER_REG| CONF_REG_ADDR);
	DATA[1]=MSB;
	DATA[2]=LSB;
        i2c_write_word( DEV_ADDR,DATA);

}
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

temperature_e_t write_tlow_reg (uint8_t MSB, uint8_t LSB)
{
	char DATA[3];
        DATA[0]=(POINTER_REG| TLOW_REG_ADDR);
        DATA[1]=MSB;
	DATA[2]=LSB;
        i2c_write_word( DEV_ADDR,DATA);

}
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

temperature_e_t  write_thi_reg (uint8_t MSB,uint8_t LSB)
{
	char DATA[3];
        DATA[0]=(POINTER_REG|THIGH_REG_ADDR);
        DATA[1]=MSB;
        DATA[2]=LSB;
        i2c_write_word( DEV_ADDR,DATA);

}

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

temperature_e_t read_temperature_reg (char data[])
{
	
	data[0]=(POINTER_REG| TEMP_REG_ADDR);
	i2c_write( DEV_ADDR,data);
	data[0]=(POINTER_REG| TEMP_REG_ADDR);
        i2c_read( DEV_ADDR,data);

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

temperature_e_t read_config_reg (char data[])
{
	data[0]=(POINTER_REG| CONF_REG_ADDR);
        i2c_write( DEV_ADDR,data);
	 	
        data[0]=(POINTER_REG| CONF_REG_ADDR);
        i2c_read( DEV_ADDR,data);
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

temperature_e_t read_tlow_reg (char data[])
{
	data[0]=(POINTER_REG|TLOW_REG_ADDR) ;
        i2c_write( DEV_ADDR,data);
	data[0]=(POINTER_REG|TLOW_REG_ADDR) ;
        i2c_read( DEV_ADDR,data);
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

temperature_e_t read_thi_reg (char data[])
{
	data[0]=(POINTER_REG| THIGH_REG_ADDR) ;
        i2c_write( DEV_ADDR,data);
	data[0]=(POINTER_REG| THIGH_REG_ADDR) ;
        i2c_read( DEV_ADDR,data);	
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


/**
* @function convert_temperature
*
* @brief
*
*
*
*
* @param  void
*
* @return temperature_e_t
*/

float convert_temperature(void)
{
	  char data[2]={0};
	  uint8_t LSB1;
	  uint8_t MSB1;

	  read_temperature_reg(data);
	  float f=0;
	  MSB1=data[0];
          LSB1=data[1];
	  int temp=0;
	  temp = ((MSB << 8) | LSB) >> 4;
	  c = temp*0.0625;
       	  f = (1.8 * c) + 32;
	  return f;
}
void main()
{

	char data[2]={0};

	MSB=0,
	LSB=0;
	int temp=0;
	float f=0;
	float c=0;

//	while(1)
//	{
      
	
	read_temperature_reg(data);
	//printf("data reg=%d \n",*(data));
	//printf("data+1 reg=%d \n",*(data+1));
	
	MSB=data[0];
	LSB=data[1];

       /* Convert 12bit int using two's compliment */
       /* Credit: http://bildr.org/2011/01/tmp102-arduino */
       temp = ((MSB << 8) | LSB) >> 4;

       c = temp*0.0625;
       f = (1.8 * c) + 32;
	char data1[1];
       	printf("Temp Fahrenheit: %f Celsius: %f\n", f, c);

	printf("Now to check read/write operations on the thigh register\n");
	write_thi_reg(0xDA,0xBC);
	read_thi_reg(data);
	printf("data[0]:%x\n",data[0]);
	printf("data[1]:%x\n",data[1]);

 	printf("Now to check read/write operations on the tlow register\n");
        write_tlow_reg(0xDA,0xBC);
        read_tlow_reg(data);
        printf("data[0]:%x\n",data[0]);
        printf("data[1]:%x\n",data[1]);

	printf("Now to check read/write operations on the config register\n");
        write_conf_reg (0xBC,0xAD);
        read_config_reg(data);
        printf("data[0]:%x\n",data[0]);
 	printf("data[1]:%x\n",data[1]);



	
//	sleep(1);
//	}


}
