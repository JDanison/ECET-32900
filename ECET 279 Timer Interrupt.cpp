/**
 * Filename: main.cpp
 * Author: John Danison
 * Date: 1/24/2025
 * 
 * ECET 32900 Lab 2 - Interrupts Review
 */

/* Libraries */
#include <Arduino.h>         // Standard Arduino Library
#include <avr/io.h>			     // General Definition
#include <avr/interrupt.h>	 // General Interrupt

/* Global Variables */
volatile uint16_t tick;   // For Timer 0

/* Global Constants */


/* Function Prototypes */
void init_io(void);
void initTimer1Int(void);

/* Setup Function */
void setup() {
  // Initializations
  init_io();			  // IO Initialization
	initTimer1Int();	// Timer 0 Initialization

	// Enable global interrupt
	sei();
}

/* Main Loop */
void loop() {
  // See if 500ms (500 Ticks) have passed
	if (tick >= 500)
	{
		// Toggle Arudino LED On and off
		PORTC ^= (1 << PC7);
				
		// Reset Tick Counter
		tick = 0;
	}
}

/* Custom Functions */
// Function for IO initialization
void init_io(void)
{
  // PORTC is the output for the LED Lights
  DDRC = 0xFF;
  PORTC = 0x00;

  // PORTF is for Analog Input
  DDRF = 0x00;
  PORTF = 0x00;
	
	// PORTE.4 as in Input
	DDRE = 0x00;
	PORTE = 0xFF;
}

// Timer Interrupt
void initTimer1Int(void) {
  TCCR1A = 0;                // Normal mode
  TCCR1B = (1 << CS11) | (1 << CS10);  // clk / 64 prescaler
  TCNT1 = 34286;             // Set counter value for 1 ms (16 MHz clock)
  TIMSK1 = (1 << TOIE1);     // Enable Timer 1 overflow interrupt
}

// Timer 1 Overflow Interrupt
ISR(TIMER1_OVF_vect) {
  TCNT1 = 34286;             // Reset Timer Value
  tick++;
}