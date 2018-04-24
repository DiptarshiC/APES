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


/* Cartridge task parameters */
#define CONTROL_STACK_DEPTH    (1000)
#define CONTROL_PRIO           (2)

#define US_12_DELAY     (480 - 20)
#define US_6_DELAY      (240 - 20)
#define EXIT_MASK       (0x80)


void vControllerTask (void *pvParameters);



#endif /* SNES_CONTROLLER_CONTROLLER_H_ */
