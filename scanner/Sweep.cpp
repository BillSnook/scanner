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
	
	for ( int i = 0; i < ReadingsMax; i++ ) {
		readings[i] = 0;
	}
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
	interval = 200;						// pause interval (milliseconds)
	myservo.write( sweepPosition );		// tell servo to go to 'sweepPosition'
	previousMillis = millis();			// get time so we wait a cycle for move command to take effect
}

void Sweep::startPingTrack() {
	
	isRunning = true;
	sweepType = ping2Scan;
	sweepPosition = minPosition;
	up = true;
	sweepIncrement = 10;
	interval = 500;						// pause interval (milliseconds)
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
	int index = sweepPosition/10;
	long diff = cm;
	char angle[8];
	char pulse[8];
	if ( readings[index] != 0 ) {
		diff = readings[index] - cm;
	}

	sprintf( pulse, "%5u", ultrasonic.duration );
	sprintf( angle, "%3d", sweepPosition);
	Serial.print( pulse );
	Serial.print( " uS, " );
	Serial.print( angle );
	Serial.print("º: ");
	Serial.print( cm );	//0~400cm
	Serial.print( " " );	//0~400cm
	Serial.print( diff );	//0~400cm
	Serial.println(" cm");
	readings[index] = cm;
//	delay( 1 );
	int saveSweepPosition = sweepPosition;
	if ( up ) {
		sweepPosition += sweepIncrement;
//		Serial.print( "+" );
		if ( sweepPosition > maxPosition ) {
//			Serial.print( "sweepType: " );
//			Serial.println( sweepType );
			if ( sweepType == ping1Scan ) {
				sweepType = noScan;
				isRunning = false;
				sweepPosition = CenterPosition;
				myservo.write(sweepPosition);		// tell servo to go to 'sweepPosition'
//				delay( 10 );
				Serial.println( "Exit" );
				return;
			} else {		// Continuous scan
				int min = readings[0];
				int idx = 0;
				for ( int i = 1; i < ReadingsMax; i++ ) {
					if ( readings[i] < min ) {
						idx = i;
						min = readings[i];
					}
				}
				minPosition = ( idx - 2 ) * sweepIncrement;
				maxPosition = ( idx + 2 ) * sweepIncrement;
				sweepIncrement = 2;
			}
//			up = false;
			sweepPosition = minPosition; // - sweepIncrement;
		}
	} else {
		sweepPosition -= sweepIncrement;
//		Serial.print( "-" );
		if ( sweepPosition < minPosition ) {
			up = true;
			sweepPosition = minPosition + sweepIncrement;
		}
	}
	myservo.write( saveSweepPosition );		// tell servo to go to 'sweepPosition'
}


/*

0º: 102 cm
10º: 102 cm
20º: 102 cm
30º: 102 cm
40º: 130 cm
50º: 162 cm
60º: 132 cm
70º: 131 cm
80º: 131 cm
90º: 251 cm
100º: 250 cm
110º: 269 cm
120º: 760 cm
130º: 759 cm
140º: 48 cm
150º: 48 cm
160º: 49 cm
170º: 88 cm
180º: 70 cm
170º: 86 cm
160º: 86 cm
150º: 87 cm
140º: 49 cm
130º: 46 cm
120º: 46 cm
110º: 760 cm
100º: 760 cm
90º: 249 cm
80º: 250 cm
70º: 134 cm
60º: 132 cm
50º: 131 cm
40º: 133 cm
30º: 146 cm
20º: 133 cm
10º: 103 cm
0º: 102 cm

*/
