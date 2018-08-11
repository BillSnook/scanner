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

#define CenterPosition		90
#define SweepServoPin		9

extern Ultrasonic	ultrasonic;

enum ScanType {
	noScan = 0,
	sweepScan,
	ping1Scan,
	ping2Scan,
	ping3Scan
} ScanType;



Sweep::Sweep() {


	sweepPin = SweepServoPin;			// sweepPin is the default PWM pin for the servo
	sweepPosition = CenterPosition;		// initial sweepPosition used to manage the sweep
	
	minPosition = 0;					// far right sweep angle
	maxPosition = 180;					// far left sweep angle
	sweepIncrement = 5;					// position step value
	interval = 100;						// pause interval (milliseconds)
	previousMillis = 0;					// will store last time sweep was updated
	up = true;
	
	isInitialized = false;
	isRunning = false;
	sweepType = noScan;
}

bool Sweep::setupForSweep() {
	
	myservo.attach( sweepPin );  		// attaches the servo device to control the servo object
	myservo.write(sweepPosition);		// tell servo to start at center position
	
	isInitialized = true;
	isRunning = false;
	sweepType = sweepScan;
	return true;
}

bool Sweep::resetForSweep() {
	
	myservo.write(CenterPosition);			// tell servo to go to 'center position
	isRunning = false;
	return true;
}

void Sweep::centerSweep() {
	
	sweepType = noScan;
	sweepPosition = CenterPosition;
	myservo.write(sweepPosition);		// tell servo to go to 'center position
}


void Sweep::checkSweep() {
	
	if ( !isInitialized || !isRunning ) {
		return;
	}
	switch ( sweepType ) {
		case noScan:
			break;
			
		case sweepScan:
			runSweep();
			break;
			
		case ping1Scan:
		case ping2Scan:
		case ping3Scan:
			runScan();
			break;
		
		default:
			break;
	}
}

void Sweep::startSweep() {
	
	isRunning = true;
	sweepType = sweepScan;
	sweepPosition = minPosition;
	up = true;
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
	myservo.write( sweepPosition );		// tell servo to go to 'sweepPosition'
}

void Sweep::startScan() {
	
	isRunning = true;
	sweepType = ping1Scan;
	sweepPosition = minPosition;
	up = true;
	sweepIncrement = 10;
	interval = 100;						// pause interval (milliseconds)
	myservo.write( sweepPosition );		// tell servo to go to 'sweepPosition'
	previousMillis = millis();			// get time so we wait a cycle for move command to take effect
}

void Sweep::startPingTrack() {
	
	isRunning = true;
	sweepType = ping2Scan;
	sweepPosition = minPosition;
	up = true;
	sweepIncrement = 10;
	interval = 100;						// pause interval (milliseconds)
	myservo.write( sweepPosition );		// tell servo to go to 'sweepPosition'
	previousMillis = millis();			// get time so we wait a cycle for move command to take effect
}

void Sweep::runScan() {

	unsigned long currentMillis = millis();
	if ( ( currentMillis - previousMillis ) < interval ) {
		return;
	}
	previousMillis = currentMillis;	// save the last time we did a cycle
	
	long cm = ultrasonic.makeMeasurement();
	Serial.print( sweepPosition );
	Serial.print("º: ");
	Serial.print( cm );	//0~400cm
	Serial.println(" cm");
	delay( 1 );
	int saveSweepPosition = sweepPosition;
	if ( up ) {
		sweepPosition += sweepIncrement;
		if ( sweepPosition > maxPosition ) {
			Serial.println( sweepScan );
			if ( sweepScan == ping1Scan ) {
				sweepType = noScan;
				isRunning = false;
				sweepPosition = CenterPosition;
				myservo.write(sweepPosition);		// tell servo to go to 'sweepPosition'
//				delay( 10 );
				Serial.println();
				return;
			}
			up = false;
			sweepPosition = maxPosition - sweepIncrement;
		}
	} else {
		sweepPosition -= sweepIncrement;
		if ( sweepPosition < minPosition ) {
			up = true;
			sweepPosition = minPosition + sweepIncrement;
		}
	}
	myservo.write( saveSweepPosition );		// tell servo to go to 'sweepPosition'
}
