/**
 * Filename: main.cpp
 * Author: John Danison
 * Date: 1/24/2025
 * 
 * ECET 32900 Lab 2 - Interrupts Review
 * Description:
 *  This lab simplifies and simulates a traffic light sequence. The program will:
 *    1) Turn on a Red LED
 *    2) Wait 4 Seconds
 *    3) Turn off the Red LED and Turn on a Green LED
 *    4) Wait 4 Seconds
 *    5) Turn off the Green LED and Turn on a Yellow LED
 *    6) Wait 4 Seconds 
 *    7) Turn off the Yellow LED and Repeat back to step 1
 * 
 *    ISR)
 *     1) When a button is pressed and INT0 is triggered, the current stoplight LED will clear and a 4th White LED will turn on. 
 *     2) Wait 4 seconds
 *     3) Continue with Stoplight sequence   
 */

/* Libraries */
#include <Arduino.h>         // Standard Arduino Library
#include <avr/io.h>			     // General Definition
#include <avr/interrupt.h>	 // General Interrupt

/* Global Variables */
volatile bool walkSignISR = false;


/* Global Constants */
int redStopLightLED = PORTC0;
int yellowStopLightLED = PORTC1;
int greenStopLightLED = PORTC2;
int whiteWalkSignLED = PORTC3;

/* Function Prototypes */
void init_io(void);
void init_interrupt(void);
void walkSignInterrupt(void);

/* Setup Function */
void setup() {
  // Initializations
  init_io();			  // IO Initialization
  init_interrupt();

  Serial.begin(9600);
  Serial.println("Setup Done");

	// Enable global interrupt
	sei();
}

/* Main Loop */
void loop() {
  /* Check for walk sign ISR */
	if (walkSignISR) {
    PORTC = 0x00;
    PORTC ^= (1 << whiteWalkSignLED);
    Serial.println("White Light");
    delay(4000);
    walkSignISR = false;
  }

  /* Normal Stoplight Sequence */
  // Red LED
  PORTC = 0x00;
  PORTC ^= (1 << redStopLightLED);
  Serial.println("Red Light");
  delay(4000);

  // Green LED
  PORTC = 0x00;
  PORTC ^= (1 << greenStopLightLED);
  Serial.println("Green Light");
  delay(4000);

  // Yellow LED
  PORTC = 0x00;
  PORTC ^= (1 << yellowStopLightLED);
  Serial.println("Yellow Light");
  delay(4000);
}

/* Custom Functions */
// Function for IO initialization
void init_io(void)
{
  // PORTC is the output for the Stop Lights LEDs
  DDRC = 0xFF;
  PORTC = 0x00;
	
	// PORTD.0 as INT0 Input
	DDRD = 0x00;
	PORTD = 0x00;
}

// Function for Interrupt initialization
void init_interrupt(void) {
  EICRA = (1 << ISC10) | (1 << ISC11);
  EIMSK = (1 << INT1);
}

/* Interrupt */
ISR(INT1_vect) {
  walkSignISR = true;
}