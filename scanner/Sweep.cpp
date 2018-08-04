//
// Sweep.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		vmdev
//
// Created by 	William Snook, 6/30/18 4:11 PM
// 				billsnook
//
// Copyright 	(c) William Snook, 2018

#include "Arduino.h"
#include "Ultrasonic.h"

#include "Sweep.h"

#define centerPosition		90

extern Ultrasonic	ultrasonic;


Sweep::Sweep() {
	
	sweepPin = 9;						// sweepPin is the default PWM pin for the servo
	sweepPosition = centerPosition;		// initial sweepPosition used to manage the sweep
	
	minPosition = 0;					// far right sweep angle
	maxPosition = 180;					// far left sweep angle
	sweepIncrement = 5;					// position step value
	interval = 100;						// pause interval (milliseconds)
	previousMillis = 0;					// will store last time sweep was updated
	up = true;
	
	isInitialized = false;
	isRunning = false;
	isMeasuring = false;
}

bool Sweep::setupForSweep() {
	
	myservo.attach( sweepPin );  		// attaches the servo device to control the servo object
	myservo.write(sweepPosition);		// tell servo to start at center position
	
	isInitialized = true;
	isRunning = false;
	isMeasuring = false;
	return true;
}

bool Sweep::resetForSweep() {
	
	myservo.write(centerPosition);			// tell servo to go to 'center position
	isRunning = false;
	isMeasuring = false;
	return true;
}

void Sweep::centerSweep() {
	
	sweepPosition = centerPosition;
	myservo.write(sweepPosition);		// tell servo to go to 'center position
}


void Sweep::checkSweep() {
	
	if ( !isInitialized || !isRunning ) {
		return;
	}
	
	if ( isMeasuring ) {
		runScan();
	} else {
		runSweep();
	}
}

void Sweep::startSweep() {
	
	isRunning = true;
	isMeasuring = false;
	sweepPosition = minPosition;
	sweepIncrement = 10;
	interval = 100;						// pause interval (milliseconds)
}

void Sweep::runSweep() {
	
	unsigned long currentMillis = millis();
	if ( ( currentMillis - previousMillis ) < interval ) {
		return;
	}
	previousMillis = currentMillis;	// save the last time we did a cycle

	if ( up ) {
		sweepPosition += sweepIncrement;
		if ( sweepPosition >= maxPosition ) {
			up = false;
			sweepPosition = maxPosition;
		}
	} else {
		sweepPosition -= sweepIncrement;
		if ( sweepPosition <= minPosition ) {
			up = true;
			sweepPosition = minPosition;
		}
	}
	myservo.write(sweepPosition);		// tell servo to go to 'sweepPosition'
}

void Sweep::startScan() {
	
	isRunning = true;
	isMeasuring = true;
	sweepPosition = minPosition;
	sweepIncrement = 10;
	interval = 10;						// pause interval (milliseconds)
}

void Sweep::runScan() {

	myservo.write(sweepPosition);		// tell servo to go to 'sweepPosition'
	delay( 10 );
	if ( isMeasuring ) {
		ultrasonic.makeMeasurement();
	}
	delay( interval );
	sweepPosition += sweepIncrement;
	if ( sweepPosition >= maxPosition ) {
		isMeasuring = false;
		isRunning = false;
		sweepPosition = centerPosition;
		myservo.write(sweepPosition);		// tell servo to go to 'sweepPosition'
		delay( 10 );
		Serial.println();
	}
}

//void Sweep::delay( int toDelay ) {
//
//	unsigned long currentMillis = millis();
//	while ( ( currentMillis - previousMillis ) < toDelay ) {
//		currentMillis = millis();
//	}
//	previousMillis = currentMillis;	// save the last time we blinked the LED
//
//}
