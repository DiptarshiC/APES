/*
 * @name :controller.h
 *
 * @Created on: April 10, 2018
 *
 * @Author: Diptarshi Chakraborty and Connor Shapiro
 *
 * In my application, below are the pin connections
 *
 * 5v          (white)           ->          5v
 *
 * data clock  (yellow)          ->          PE4 as output
 *
 * Serial data (red)             ->          PL3 as input
 *
 * data latch  (orange)          ->          PC5 as output
 *
 * ground      (brown)           ->          GND
 *
 */

#ifndef SNES_CONTROLLER_CONTROLLER_H_
#define SNES_CONTROLLER_CONTROLLER_H_


/**************************************************************************************************/
#define US_12_DELAY     (480-20)

#define US_6_DELAY      (240-20)

#define MS_16_466_DELAY (658640 - 20)


/**************************************************************************************************/
/*variable to read the input*/



/**************************************************************************************************/


/**
 * This function below will enable all the gpio
 * peripherals used for the clocking, data latching and
 * input of the game console.
 */
void enable_peripherals();

/**
 * This function shall actually latch all the signals from
 * the SNES controller onto the TIVA board
 *
 */
void send_signal();



#endif /* SNES_CONTROLLER_CONTROLLER_H_ */
