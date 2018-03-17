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





#define MAX_MQ_NAME     20
#define MAX_LOG_NAME    16
#define MAX_STR_LEN     64
#define LOG_MAX_MSGS    20




struct temp_thread_info
{
    uint8_t log_mq_name[MAX_MQ_NAME];
    uint8_t main_mq_name[MAX_MQ_NAME];
    uint8_t temp_mq_name[MAX_MQ_NAME];
    uint8_t server_mq_name[MAX_MQ_NAME];
};



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

typedef enum
{                       // These are the only two threads that should be messaging Temp
    T_MAIN,
    T_REMOTE
} temp_source_t;

typedef enum
{
    TEMP_DATAREQ,       // ID of message from Remote to Temp asking for a temperature data message
    TEMP_HEARTBEATREQ,  // ID of message from Main to Temp asking for a heartbeat
    TEMP_EXITCMD        // ID of message from Main to Temp commanding a graceful exit
} temp_id_t;

typedef struct temp_msg
{
    temp_id_t id;
    temp_source_t source;
} temp_msg_t;


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

temperature_e_t write_conf_reg (uint8_t MSB,uint8_t LSB);


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

temperature_e_t write_tlow_reg (uint8_t MSB,uint8_t LSB);


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

temperature_e_t write_thi_reg (uint8_t MSB,uint8_t LSB);

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

temperature_e_t read_temperature_reg (char data[]);


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

temperature_e_t read_config_reg (char data[]);


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

temperature_e_t read_tlow_reg (char data[]);

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

temperature_e_t read_thi_reg (char data[]);

/**
* @function read_thi_reg
*
* @brief reads from the thi reg
*
* @param  uint8_t data:pointer to store data
*
* @return temperature_e_t
*/

temperature_e_t read_thi_reg (char data[]);

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

float convert_temperature(void);


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

void *temp(void *args);





#endif /*__TEMP_H__ */
