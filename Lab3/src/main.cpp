/**
 * Filename: main.cpp
 * Author: John Danison
 * Date: 1/31/2025
 * 
 * ECET 32900 Lab 3 - Thermister Analog Reading
 * Description:
 *  
 * 
 * Reference:
 * https://learn.adafruit.com/thermistor/using-a-thermistor 
 * 
 * https://cplusplus.com/reference/cmath/log/ 
 */

/* Libraries */
#include <Arduino.h>         // Standard Arduino Library

/* Global Constants */
#define DELAY_TIME 15000
#define NUM_SAMPLES 10
#define SERIES_RESISTOR 10000
#define MAX_ADC_VALUE 1023
#define ROOM_TEMP 25
#define THERMISTOR_B_VAL 3950

/* Global Variables */
float adc_read_sum = 0;
float avg_adc_reading = 0;
float adc_ratio = 0;
float thermistor_resistance = 0;
float room_temp_kelvin = 0;
float thermistor_temp_kelvin = 0;
float thermistor_temp_celsius = 0;


/* Setup Function */
void setup() {  
  // Initialize Serial
  Serial.begin(9600);

  Serial.println("Initialization Finished");
  Serial.println("--------------------------------------------------------------");
}

/* Main Loop */
void loop() {
  // Delay 15 Seconds
  delay(DELAY_TIME);

  // Reset ADC Reading variable
  adc_read_sum = 0;

  // Take 10 Samples with a slight delay between for improved accuracy
  for (int i = 0; i < NUM_SAMPLES; i++) {
    adc_read_sum += analogRead(0);
    delay(10);
  }

  // Average ADC Reading
  avg_adc_reading = (adc_read_sum * 1.0 / NUM_SAMPLES);

  // Solve for the Thermister Resistance
  adc_ratio = MAX_ADC_VALUE / avg_adc_reading - 1;
  thermistor_resistance = (SERIES_RESISTOR / adc_ratio);

  // Convert Room Temp from C to K
  room_temp_kelvin = (ROOM_TEMP + 273.15);

  // Solve for Kelvin Thermistor Temperature
  thermistor_temp_kelvin = 1.0 / ((1.0 / room_temp_kelvin) + (log(thermistor_resistance / SERIES_RESISTOR) / THERMISTOR_B_VAL));

  // Convert Temp to C from Kelvin
  thermistor_temp_celsius = thermistor_temp_kelvin - 273.15;

  // Print out the results
  Serial.println("\nTemp Measured: ");
  Serial.println(thermistor_temp_celsius);
}