/*

 * @name :controller.c

 *

 * @Created on: Apr 14, 2018

 *

 * @Author: Diptarshi Chakraborty and Connor Shapiro

 *

 * Declaration : We have used Tivaware driver libraries and tivaware hardware includes in our project.

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



#include <stdint.h>

#include <stdbool.h>
#include <stdio.h>


#define BUTTONS_MASK    (0x0FFF)

//#define	 val		0x01










uint16_t usReadController(val)

{

    uint16_t usReadValue = 0;



    /*Put that data latch pin to high*/

   // MAP_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, (1<<5));



    /*Add a delay for the 12 us pulse*/

   // MAP_SysCtlDelay(US_12_DELAY);



    /*Put that data latch pin to low*/

   // MAP_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);



    /*add a 6 us pulse*/

  //  MAP_SysCtlDelay(US_6_DELAY);



    /*Now we shall put alternate routines of highs and lows*/

    /***********************************************************************************************************/



    //0 -- Button B



    /*Make serial clock low*/

 //   MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val) >> 3) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

   // MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

   // MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

   // MAP_SysCtlDelay(US_6_DELAY);



     /***********************************************************************************************************/





    /***********************************************************************************************************/



    //1 -- Button Y



    /*Make serial clock low*/

   // MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val) >> 2) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/ 

  //  MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

  //  MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

 //   MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //2 -- Button Select



    /*Make serial clock low*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val) >> 1) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

  //  MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

    //MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //3 -- Button Start



    /*Make serial clock low*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = ~(0xFFF7 | val)& BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //4 -- Button Up



    /*Make serial clock low*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val)<< 1) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //5 -- Button Down



    /*Make serial clock low*/

 //   MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val) << 2) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

 //   MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

 //   MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //6 -- Button Left



    /*Make serial clock low*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val)  << 3) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //7 -- Button Right



    /*Make serial clock low*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val) << 4) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //8 -- Button A



    /*Make serial clock low*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val)<< 5) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

 //   MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //9 -- Button X



    /*Make serial clock low*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val) << 6) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //10 -- Button L



    /*Make serial clock low*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val) << 7) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);

    /***********************************************************************************************************/





    /***********************************************************************************************************/



    //11 -- Button R



    /*Make serial clock low*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    usReadValue = (~(0xFFF7 | val) << 8) & BUTTONS_MASK;

                                                       //since i am using pin 3

    /*add a 6 us pulse*/

//    MAP_SysCtlDelay(US_6_DELAY);



    /*Make serial clock high*/

//    MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);



    /*add a 6 us pulse*/

   /* MAP_SysCtlDelay(US_6_DELAY);*/

    /***********************************************************************************************************/



    // Finished after R, only 12 buttons on controller

    return usReadValue;

}

//void main()
//{
//	printf("The value returned by read controller is %d\n",usReadController(0x0001));
//}







