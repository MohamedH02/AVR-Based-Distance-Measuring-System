 /******************************************************************************
 *
 * File Name: main.c
 *
 * Description: Source file for application
 *
 * Author: Mohamed Hisham
 *
 *******************************************************************************/


#include <avr/io.h>
#include "lcd.h"
#include "ultrasonic.h"
#include <util/delay.h>

int main(void){

	uint16 distance = 0; /* variable to store distance in */

	SREG |= (1<<7);  /* enable general interrupts */

	LCD_init();   /* initialize and enablr LCD */
	ULTRASONIC_init();    /* initialize and enablr Ultrasonic */
	LCD_displayString("Distance=    cm");
	while(1){
		distance = ULTRASONIC_readDistance();   /* get the distance and store it in variable */
		LCD_moveCursor(0, 10);
		/* display the distance on the LCD */
		if(distance >= 100)
		{
			LCD_intgerToString(distance);
		}
		else
		{
			LCD_intgerToString(distance);
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_displayCharacter(' ');
		}
		_delay_ms(50);
	}

}




