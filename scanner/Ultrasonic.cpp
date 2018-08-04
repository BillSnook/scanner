//
//  Ultrasonic.cpp
//  Index
//
//  Created by William Snook on 6/8/18.
//  Copyright © 2018 billsnook. All rights reserved.
//

#include "Ultrasonic.h"


Ultrasonic::Ultrasonic() {
	_pin = 7;
	isInitialized = false;
	isRunning = false;
	previousMillis = 0;
	interval = 1000;

	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	
}

/*Begin the detection and get the pulse back signal*/
bool Ultrasonic::setupForUltrasonic() {
	
	isInitialized = true;
//	isRunning = true;
	
	return true;
}

void Ultrasonic::distanceMeasure(void) {
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pin,LOW);
	pinMode(_pin,INPUT);
	duration = pulseIn(_pin,HIGH);
}

/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::microsecondsToCentimeters(void) {
	return duration/29/2;
}
/*The measured distance from the range 0 to 157 Inches*/
long Ultrasonic::microsecondsToInches(void) {
	return duration/74/2;
}

void Ultrasonic::measure() {

	if ( !isInitialized || !isRunning ) {
		return;
	}
	
	unsigned long currentMillis = millis();
	bool ready = ( ( currentMillis - previousMillis ) >= interval );
	if ( !ready ) {
		return;
	}
	previousMillis = currentMillis;	// save the last time we measured
	makeMeasurement();
}

void Ultrasonic::makeMeasurement() {
	
	distanceMeasure();// get the current signal time;
	long RangeInInches;
	long RangeInCentimeters;
	RangeInInches = microsecondsToInches();//convert the time to inches;
	RangeInCentimeters = microsecondsToCentimeters();//convert the time to centimeters
	Serial.print(duration);
	Serial.print("uS: ");
//	Serial.print(RangeInInches);		//0~157 inches
//	Serial.print(" inch, ");
	Serial.print(RangeInCentimeters);	//0~400cm
	Serial.println(" cm");
	delay(100);
	isRunning = false;
}
