/******************************************************************************
 *
 * Module: ultrasonic
 *
 * File Name: ultrasonic.c
 *
 * Description: Source file for the ultrasonic driver
 *
 * Author: Mohamed Hisham
 *
 *******************************************************************************/

#include "ultrasonic.h"
#include "icu.h"
#include "gpio.h"
#include <util/delay.h>

ICU_ConfigType Config_Ptr = {F_CPU_8,RAISING};
static uint16 highTime = 0;
static uint8 edge_counter = 0;


/*******************************************************************************
 *                      Functions Definitions                                   *
 *******************************************************************************/

/*
 *  Description:
 * Initialize the ICU driver as required.
 * Setup the ICU call back function.
 * Setup the direction for the trigger pin as output pin through the GPIO driver.
 * Inputs: None
 * Return: None
 */

void ULTRASONIC_init(void){
	ICU_init(&Config_Ptr);
	ICU_setCallBack(ULTRASONIC_edgeProcessing);
	GPIO_setupPinDirection(TRIGGER_PORT,TRIGGER_PIN,PIN_OUTPUT);
}


/*
 * Description:
 * Send the Trigger pulse to the ultrasonic.
 * Inputs: None
 * Return: None
 */
void ULTRASONIC_Trigger(void){
	GPIO_writePin(TRIGGER_PORT,TRIGGER_PIN,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(TRIGGER_PORT,TRIGGER_PIN,LOGIC_LOW);
}


/*
 * Description
 * Send the trigger pulse by using Ultrasonic_Trigger function.
 * Start the measurements by the ICU from this moment.
 * Inputs: None
 * Return: The measured distance in Centimeter.
 */
uint16 ULTRASONIC_readDistance(void){
	static uint16 distance;

	ULTRASONIC_Trigger();
	distance = ((float32)highTime/58) + 1;
	return distance;
}

/*
 * Description
 * This is the call back function called by the ICU driver.
 * This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 * Inputs: None
 * Return: None
 */
void ULTRASONIC_edgeProcessing(void){
	edge_counter++;

	if(edge_counter == 1)
	{
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(FALLING);
	}
	else if(edge_counter == 2)
	{
		highTime = ICU_getInputCaptureValue();
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(RAISING);
		edge_counter = 0;
	}

}
