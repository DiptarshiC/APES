
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
#include <pthread.h>
#include <mqueue.h>
#include <stdbool.h>
#include <time.h>

#include "../includes/i2c.h"
#include "../includes/temp.h"
#include "../includes/main.h"
#include "../includes/logger.h"







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
	  c = temp*0.0625;
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


	/* Take in arguments from Main thread */
	struct temp_thread_info * p_targs = (struct temp_thread_info *) arg;

	 /* Open message queue and logfile */
	mqd_t temp_mq = mq_open(p_targs->temp_mq_name, O_RDONLY);

	if (temp_mq == FAILURE)
	{
        perror("Failed to open temp message queue.\n");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}





	/* Let Main know that Temperature startup went well */
	mqd_t main_mq = mq_open(p_targs->main_mq_name, O_WRONLY);
	if (main_mq == FAILURE)
	{
        log_str("(Temperature) [ERROR]: Failed to open Main message queue.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}
	main_msg_t * p_main_msg = (main_msg_t *) malloc(sizeof(main_msg_t));
	if (!p_main_msg)
	{
        log_str("(Temperature) [ERROR]: Failed to malloc for main_msg.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}
	p_main_msg->id = START_OK;
	p_main_msg->source = M_TEMPERATURE;
	if (mq_send(main_mq, p_main_msg, sizeof(main_msg_t), PRIORITY_TWO))
	{
        log_str("(Temperature) [ERROR]: Failed to send START_OK to Main.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}
	/* Allocate temp_msg */
	temp_msg_t * p_log_msg = (temp_msg_t *) malloc(sizeof(temp_msg_t));


	/* Main Loop */
    bool b_exit = false;
    while (!b_exit) // Do Logger things until Main orders a graceful exit.
    {
        mq_receive(temp_mq, p_log_msg, sizeof(temp_msg_t), NULL); // Block empty

        /* Check for COMMAND Messages, Handle them */
        if (p_log_msg->level == COMMAND)
        {
            if (p_log_msg->source == MAIN)
            {                       // Only allow Main thread to issue Commands
                if (strcmp(p_log_msg->str, "heartbeat"))
                {
                    p_main_msg->id = HEARTBEAT;
                    p_main_msg->source = M_TEMPERATURE;
                    if (mq_send(main_mq, p_main_msg, sizeof(main_msg_t),
                                                                PRIORITY_TWO) )
                    {
                log_str("(Temperature) [ERROR]: Failed to send "
                                                        "HEARTBEAT to Main.");
                        int8_t retvalue = FAILURE;
                        pthread_exit(&retvalue);
                    }
                }

		else if (strcmp(p_log_msg->str, "exit"))
                {
                    b_exit = true;
                    log_str("(Temperature) [INFO]: Received exit command from"
                                                                    " Main.");
                }
                else
                {
                    log_str("(Temperature) [WARNING]: Received unexpected"
                                                        " command from Main.");
                }
            }



        }

	/* If not COMMAND Message, Log the Message */
        else
        {   /* Prepare str_time */
            time_t thetime = p_log_msg->timestamp;
            uint8_t str_time[TIME_STR_SIZE];
            if (time(&thetime) == FAILURE)
            {
                perror("Failed to get time.\n");  /* Use perror() instead of
                                                   * some sort weird recursive
                                                   * logging.
                                                   */
                *str_time = NULL; // Null terminate first character
                return FAILURE;
            }
            strftime(str_time, TIME_STR_SIZE, '%T', localtime(&thetime));

	/* Prepare str_source */
            uint8_t str_source[MAX_SOURCE_LEN];
            switch (p_log_msg->source)
            {
                case MAIN:
                    strcopy(str_source, "Main");
                break;

                case REMOTE:
                    strcopy(str_source, "Remote");
                break;

                default:
                    log_str("(Temperature) [WARNING]: Received log from unknown "
                                                                    "thread.");
                    *str_source = NULL; // Null terminate first character
                    break;
            }

		/* Prepare str_level */
            uint8_t str_level[MAX_LEVEL_LEN];
            switch (p_log_msg->level)
            {
                case INFO:
                    strcpy(str_level, "INFO");
                break;

                case WARNING:
                    strcpy(str_level, "WARNING");
                break;

                case ERROR:
                    strcpy(str_level, "ERROR");
                break;

                default:
                    log_str("(Temperature) [WARNING]: Received log of unknown "
                                                                    "level.");
                    *str_level = NULL;
                break;
            }


		/* Log */
            if (fprintf(gp_log_file, "%s - (%s) [%s]: %s\n", str_time,
                            str_source, str_level, p_log_msg->str) < SUCCESS);
            {
                perror("Failed to write to logfile.\n");
            }
        }
    }

	/* Attempt to Graceful Exit */
    if (mq_close(temp_mq))
    {
        log_str("(Temperature) [ERROR]: Failed to close Log message queue.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    if (mq_unlink(temp_mq))
    {

        log_str("(Temperature) [ERROR]: Failed to destroy temperature message queue.");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    if (fclose(gp_log_file))
    {
        perror("(Temperature) [ERROR]: Failed to close logfile.\n");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }
    free(p_main_msg);
    free(p_log_msg);
    int8_t retvalue = SUCCESS;
    pthread_exit(&retvalue);




}






