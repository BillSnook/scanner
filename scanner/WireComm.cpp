//
// WireComm.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		scanner
//
// Created by 	William Snook, 7/22/18 5:02 PM
// 				billsnook
//
// Copyright 	(c) William Snook, 2018
// Licence		<#licence#>
//
// See 			WireComm.h and ReadMe.txt for references
//


// Library header
#include "Arduino.h"
#include "WireComm.h"


#define I2C_SLAVE_ADDRESS	8

WireComm::WireComm() {
	
	isSetup = false;		// to be sure
}

void WireComm::setupForWireComm( bool beMaster ) {
	
	if ( isSetup ) {
		Serial.println( "WireComm is already setup." );
		return;
	}
	isSetup = true;									// only done once
	isMaster = beMaster;
	if ( isMaster ) {
		Serial.println(" - setupForWireComm as master");		// print debug info
		Wire.begin();								// join i2c bus as master
	} else {
		Serial.println(" - setupForWireComm as slave at address 8");		// print debug info
		Wire.begin( I2C_SLAVE_ADDRESS );			// join i2c bus as slave with address #8
		Wire.onReceive(receiveEvent);				// register receive event for writes from master
		Wire.onRequest(requestEvent);				// register request event for read to master
	}

}

void WireComm::resetForWireComm() {
	
	
}

// MARK: These routines are effectively only for the master, to send to and initiate receiving data from a slave

void WireComm::readWireComm( int getBytes ) {
	
	if ( isSetup && isMaster ) {
		Serial.print( " - readWireComm get " );				// print debug info
		Serial.print( getBytes );							// print debug info
		Serial.println( " bytes from slave" );				// print debug info
		Wire.requestFrom( I2C_SLAVE_ADDRESS, getBytes );    // request getBytes bytes from slave device
	}
}

void WireComm::writeWireComm( char *data ) {
	
	if ( isSetup && isMaster ) {
		Serial.print( " - writeWireComm send " );			// print debug info
		Serial.print( strlen( data ) );						// print debug info
		Serial.print( " bytes to slave: " );					// print debug info
		Serial.println( data );		// print debug info
		Wire.beginTransmission( I2C_SLAVE_ADDRESS );		// transmit to device #8
		Wire.write( data, strlen( data ) );					// sends data
		Wire.endTransmission();								// stop transmitting
	}
}

void WireComm::runWireComm() {

	if ( !isSetup ) {
		return;
	}
	if ( isMaster ) {
		// This checks for data returned from a slave in response to a requestFrom command to that slave
		// The slave can send more than we request and we ignore it, but if it sends less, we get 0x255 fillers at the end
		if ( Wire.available() > 0 ) {	// however many slave sends, we do not seem to get more than we requested
			Serial.println("runWireComm with data available");		// print debug info
			while (Wire.available()) {	// slave may send less than requested
										//  but we seem to not accept more than we requested
				char c = Wire.read();	// receive a byte as character
				uint8_t n = (uint8_t)c;
				if ( c != 0x255 ) {		// Skip characters not sent - positions requested but not sent are prefilled
					Serial.print(c);    // print the valid characters
				}
//				Serial.print(" ");        // print the character
//				Serial.println(n);        // print the character
			}
			Serial.println("");			// print an end of line
		}
//	} else {
		
	}
}

// MARK: These slave callback routines are to handle writes and reads from the master

// function that executes whenever data is requested by a master when it requests data
// this function is registered as an event, see setup()
static void WireComm::requestEvent() {
	Serial.println("requestEvent, sending 'hello world....', 15 characters");	// debug info
	Wire.write("hello world....");			// respond with message with many bytes
	
	// Check mode then respond with appropriate data
}

// function that executes whenever data is received from master when it writes
// this function is registered as an event, see setup()
static void WireComm::receiveEvent( int howMany ) {
	Serial.print("receiveEvent, howMany: ");	// debug info
	Serial.println(howMany);	// debug info
	while (0 < Wire.available()) {	// loop through all but the last
		char c = Wire.read();		// receive byte as a character
		Serial.print(c);			// print the character
		
		// Accumulate bytes into command, execute it
	}
//	int x = Wire.read();			// receive byte as an integer
//	Serial.println(x);				// print the last number and then end of line
	Serial.println();				// print the last number and then end of line
}

