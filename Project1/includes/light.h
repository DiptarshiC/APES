/**
*@file: light.h
*
*@description: This header file provides an abstraction
*	 	for reading and writing values in the i2c
*	 	light sensor.
*
*@Date:2nd March 2017 
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/


#ifndef __LIGHT_H__
#define __LIGHT_H__



#define	SLAVE_ADDRESS		0x39
#define COMMAND_REG		0x80
#define COMMAND_WORD		0xA0
#define	CONTROL			0x00
#define	TIMING			0x01
#define THRESH_LOW_LOW		0x02
#define	THRESH_LOW_HIGH		0x03
#define	THRESH_HIGH_LOW		0x04
#define	THRESH_HIGH_HIGH	0x05
#define INTERRUPT		0x06
#define	ID			0x0A
#define DATA0LOW		0x0C
#define DATA0HIGH		0x0D
#define DATA1LOW		0x0E
#define DATA1HIGH		0x0F


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

	LIGHT_NO_ERR,
	LIGHT_BAD_PARAM


}light_e_t;


typedef struct light_msg
{
    time_t timestamp;
    log_level_t level;
    log_source_t source;
    uint8_t str[MAX_STR_LEN];
} light_msg_t;


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

light_e_t write_ctrl_reg (uint8_t data);

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

light_e_t write_timing_reg (uint8_t data);


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

light_e_t write_irq_thresh_reg (uint8_t channel, uint16_t data);


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

light_e_t write_irq_ctrl_reg (uint8_t data);



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

light_e_t read_ctrl_reg (char data[]);

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

light_e_t read_timing_reg (char data[]);


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

light_e_t read_irq_ctrl_reg (char data[]);

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

light_e_t read_irq_ctrl_reg (char data[]);

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

light_e_t read_id_reg (char data[]);

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

light_e_t read_adc_reg (uint8_t channel, char data[]);


/**
* @function convert_light
*
* @brief reads data from the adc reg
*
* @param void
*                 
* @return float
*/
float convert_light(uint8_t ADC1H,uint8_t ADC1L,uint8_t ADC0H,uint8_t ADC0L);

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





























#endif /*__TEMP_H__ */












