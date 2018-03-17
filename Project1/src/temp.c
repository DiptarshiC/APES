
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


#include <stdbool.h>
#include <mqueue.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <stdbool.h>
#include <time.h>

#include "../includes/i2c.h"
#include "../includes/temp.h"
#include "../includes/main.h"
#include "../includes/logger.h"

#define	FAILURE		-1
#define SUCCESS		0




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
	  temp = ((MSB1 << 8) | LSB1) >> 4;
	  float c = temp*0.0625;
       	  f = (1.8 * c) + 32;
	  return f;
}


/**
* @function *temp
*
* @brief temperature task
*
*
*
*
* @param  void *args
*
* @return void *
*/

void *temp(void *args)
{


    struct temp_thread_info* p_targs = (struct temp_thread_info *) args;

	/* Let Main know that Temperature startup went well */
	mqd_t main_mq = mq_open(p_targs->main_mq_name, O_WRONLY);
	if (main_mq == FAILURE)
	{
        // Log something here using perror
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}
	main_msg_t * p_main_msg = (main_msg_t *) malloc(sizeof(main_msg_t));
	if (!p_main_msg)
	{
        // Log something here using perror
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}
	p_main_msg->id = START_OK;
	p_main_msg->source = M_TEMPERATURE;
	if (mq_send(main_mq, p_main_msg, sizeof(main_msg_t), PRIORITY_TWO))
	{
        // Log something here using perror
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}
	/* Allocate temp_msg */
	temp_msg_t * p_temp_msg = (temp_msg_t *) malloc(sizeof(temp_msg_t));

    //Open temp_mq from p_targs
    //Open log_mq

	/* Main Loop */
	bool b_exit = false;
	while (!b_exit) // Do Temperature things until Main orders a graceful exit.
	{
	mq_receive(temp_mq, p_temp_msg, sizeof(temp_msg_t), NULL); // Block empty
	if (p_temp_msg->source == T_MAIN)
	{

		if (p_temp_msg->id == TEMP_HEARTBEATREQ)
		{
			p_main_msg->id = HEARTBEAT;
			p_main_msg->source = M_TEMPERATURE;
			if (mq_send(main_mq, p_main_msg, sizeof(main_msg_t), PRIORITY_TWO) )//sends main heartbeat
			{
				int8_t retvalue = FAILURE;
				pthread_exit(&retvalue);
			}

		}
		else if (p_temp_msg->id == TEMP_EXITCMD)
		{
			if (mq_close(temp_mq))
			{
                // Log something to Logger
				int8_t retvalue = FAILURE;
        			pthread_exit(&retvalue);
    			}
    			if (mq_unlink(temp_mq))
    			{
                    // Log something to Logger
        			int8_t retvalue = FAILURE;
        			pthread_exit(&retvalue);
    			}

    			free(p_main_msg);
    			free(p_temp_msg);
    			int8_t retvalue = SUCCESS;
                // Log something with printf
			 pthread_exit(&retvalue);
		}
	}

	else if (p_temp_msg->source == T_REMOTE)
                { 


                    p_remote_msg->value =convert_temperature();

                    if (mq_send(remote_mqd, p_remote_msg, sizeof(remote_msg_t),
                                                                PRIORITY_TWO) )//sends main heartbeat
                    {
                        int8_t retvalue = FAILURE;
                        pthread_exit(&retvalue);
                    }

                }
	}
}







