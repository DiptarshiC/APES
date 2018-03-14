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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
#include <error.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>
#include "../includes/i2c.h"

 
/**
* @function i2c_read
*
* @brief reads from an i2c register
*
* Given an address it will read from the 
*       address
*
* @param uint8_t address : address of the register to read from
*              	uint8_t buf[] to store values that are read 
*
* @return void
*/



pthread_mutex_t i2c_locker =PTHREAD_MUTEX_INITIALIZER;



void  i2c_read(uint8_t address,uint8_t * buf)
{
/*	In both i2c read and i2c write operations
	
	the first step is the initialization 
	
	of the i2c bus for read and write.
	
	The initialization of the i2c bus is done

	with the code below
	
Credit:https://elixir.bootlin.com/linux/v4.9.78/source/Documentation/i2c/dev-interface
*/

	int file;
	int adapter_nr = 2; /* probably dynamically determined */
	char filename[20];
	
  
	snprintf(filename, 19, "/dev/i2c-2", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) 
	{
		perror("Error in opening the file\n");
    		exit(1);
  	}
/*
	After we have opened the device, 
	
	we must specify with what device

	address we want to communicate:
		
*/
	

	if (ioctl(file, I2C_SLAVE, address) < 0) 
	{
 	perror("Could not open the file\n");
	exit(1);
  	}
	pthread_mutex_lock(&i2c_locker);
	/* Using I2C Read*/
	if (read(file,buf,2) != 2) 
	{
        /*ERROR HANDLING: i2c transaction failed */
	perror("Failed to read from the i2c bus.\n");
	}
	pthread_mutex_unlock(&i2c_locker);
	if(close(file)<0)
	{
	perror("Error in closing file.\n");	
	}
	
	
}

/**
* @function i2c_read_word
*
* @brief reads from an i2c register
*
* Given an address it will read from the 
*       address
*
* @param uint8_t address : address of the register to read from
*              	uint8_t buf[] to store values that are read 
*
* @return void
*/

void  i2c_read_word(uint8_t address,uint8_t * buf)
{
/*	In both i2c read and i2c write operations
	
	the first step is the initialization 
	
	of the i2c bus for read and write.
	
	The initialization of the i2c bus is done
	with the code below
	
Credit:https://elixir.bootlin.com/linux/v4.9.78/source/Documentation/i2c/dev-interface
*/

	int file;
	int adapter_nr = 2; /* probably dynamically determined */
	char filename[20];
	
  
	snprintf(filename, 19, "/dev/i2c-2", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) 
	{
		perror("Error in opening the file\n");
    		exit(1);
  	}
/*
	After we have opened the device, 
	
	we must specify with what device
	address we want to communicate:
		
*/
	

	if (ioctl(file, I2C_SLAVE, address) < 0) 
	{
 	perror("Could not open the file\n");
	exit(1);
  	}
	pthread_mutex_lock(&i2c_locker);
	/* Using I2C Read*/
	if (read(file,buf,2) != 2) 
	{
        /*ERROR HANDLING: i2c transaction failed */
	perror("Failed to read from the i2c bus.\n");
	}
	pthread_mutex_unlock(&i2c_locker);
	if(close(file)<0)
	{
	perror("Error in closing file.\n");	
	}
	
	
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

/*	In both i2c read and i2c write operations
	
	the first step is the initialization 
	
	of the i2c bus for read and write.
	
	The initialization of the i2c bus is done
	with the code below
	
Credit:https://elixir.bootlin.com/linux/v4.9.78/source/Documentation/i2c/dev-interface
*/

	int file;
	int adapter_nr = 2; /* probably dynamically determined */
	char filename[20];
	
  
	snprintf(filename, 19, "/dev/i2c-2", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) 
	{
		perror("Error in opening the file\n");
    		exit(1);
  	}
/*
	After we have opened the device, 
	
	we must specify with what device
	address we want to communicate:
		
*/
	

	if (ioctl(file, I2C_SLAVE, address) < 0) 
	{
 	perror("Could not open the file\n");
	exit(1);
  	}
	pthread_mutex_lock(&i2c_locker);
	if (write(file, data, 2) != 2) 
	{
        /*ERROR HANDLING: i2c transaction failed */
        perror("write failed\n");
 
	}
	pthread_mutex_unlock(&i2c_locker);
	 if(close(file)<0)
        {
        perror("Error in closing file.\n");
        }



}

/**
* @function i2c_write_word
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

void i2c_write_word(uint8_t address, uint8_t *data)
{

/*	In both i2c read and i2c write operations
	
	the first step is the initialization 
	
	of the i2c bus for read and write.
	
	The initialization of the i2c bus is done
	with the code below
	
Credit:https://elixir.bootlin.com/linux/v4.9.78/source/Documentation/i2c/dev-interface
*/

	int file;
	int adapter_nr = 2; /* probably dynamically determined */
	char filename[20];
	
  
	snprintf(filename, 19, "/dev/i2c-2", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) 
	{
		perror("Error in opening the file\n");
    		exit(1);
  	}
/*
	After we have opened the device, 
	
	we must specify with what device
	address we want to communicate:
		
*/
	

	if (ioctl(file, I2C_SLAVE, address) < 0) 
	{
 	perror("Could not open the file\n");
	exit(1);
  	}
	 pthread_mutex_lock(&i2c_locker);
	if (write(file, data, 3) != 3) 
	{
        /*ERROR HANDLING: i2c transaction failed */
        perror("write failed\n");
 
	}
	pthread_mutex_unlock(&i2c_locker);
	if(close(file)<0)
        {
        perror("Error in closing file.\n");
        }

}


/*
void main()
{
	char buf[2]={0};
	while(1)
	{
      
	uint8_t *buf=(uint8_t*)malloc(2*sizeof(uint8_t));
	i2c_read(0x48,buf);
	int temp;
	uint8_t MSB=0;
	uint8_t LSB=0;
	
	MSB=*(buf);
	LSB=*(buf+1);
	
	
	float f, c;

       /* Convert 12bit int using two's compliment 
       /* Credit: http://bildr.org/2011/01/tmp102-arduino/ 
       temp = ((MSB << 8) | LSB) >> 4;

       c = temp*0.0625;
       f = (1.8 * c) + 32;

       printf("Temp Fahrenheit: %f Celsius: %f\n", f, c);

	sleep(1);
	}
	
}
*/	
	





