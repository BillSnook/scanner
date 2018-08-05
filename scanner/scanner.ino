//
// vmdev
//
// Test usage of VMA100 328 development board
// Test usage of VMA101 2560 development board
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		William Snook
// 				billsnook
// Date			6/2/18 8:01 AM
//
// Copyright	© William Snook, 2018
//

#include <Arduino.h>

#include "LEDs.h"
#include "Sweep.h"
#include "Ultrasonic.h"
#include "WireComm.h"


LEDs			leds;
Ultrasonic		ultrasonic;
Sweep			sweep;
WireComm		wireComm;


String			inputString;		// a String to hold incoming data
boolean			stringComplete;		// whether the string is complete


void setup() {

	Serial.begin(115200);
	while(!Serial) {}	// Wait for it to be ready

	inputString = "";
	inputString.reserve(200);	// reserve 200 bytes for the inputString
	Serial.println( "" );

	leds = LEDs();
	leds.setupForLEDs();

	sweep = Sweep();
	sweep.setupForSweep();

	ultrasonic = Ultrasonic();
	ultrasonic.setupForUltrasonic();

	wireComm = WireComm();
	wireComm.setupForWireComm( false );	// true for master, false for slave

	stringComplete = false;

	Serial.println( "Setup complete" );

}

void loop() {
	//	Serial.print( "+" );
	if ( stringComplete ) {
		inputString = "New: " + inputString;
		Serial.println( inputString );
		// clear the string:
		inputString = "";
		stringComplete = false;
	}

	leds.toggle();
	
	ultrasonic.measure();

	sweep.checkSweep();
	
	wireComm.runWireComm();
}

// SerialEvent occurs whenever a new data comes in the hardware serial RX. This
// routine is run between each time loop() runs, so using delay inside loop can
// delay response. Multiple bytes of data may be available.
void serialEvent() {
	while ( Serial.available() ) {
		char inChar = (char)Serial.read();
		if ( inputString.length() == 0 ) {	// If first char of new string
			Serial.print( inChar );
			switch ( inChar ) {
				case 'l':
					leds.isRunning = !leds.isRunning;
					continue;

					
				case 'p':
					ultrasonic.isRunning = true;
					ultrasonic.measure();
					continue;

					
				case 'c':
					sweep.isRunning = false;
					sweep.centerSweep();
					continue;
					
				case 's':
					sweep.startSweep();
					continue;
					
				case 'u':
					sweep.startScan();
					continue;
					
					
//				case 'm':
//					wireComm.setupForWireComm( true );
//					continue;
//					
//				case 'v':
//					wireComm.setupForWireComm( false );
//					continue;
//					
				case 'r':								// buffer size is 32
					wireComm.readWireComm( 20 );		// We never get more than requested, we can get less
					continue;
					
				case 'w':
					wireComm.writeWireComm( "Data" );
					continue;
					
				default:
					break;
			}
		}
		inputString += inChar;
		if ( ( inChar == '\n' ) || ( inChar == '\r' ) ) {
			stringComplete = true;
		}
	}
}

