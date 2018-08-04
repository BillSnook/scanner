//
//  Ultrasonic.h
//  Index
//
//  Created by William Snook on 6/8/18.
//  Copyright © 2018 billsnook. All rights reserved.
//

#ifndef Ultrasonic_h
#define Ultrasonic_h

#include <Arduino.h>


class Ultrasonic {
	
private:
	int				_pin;		// Arduino pin connected with SIG pin of Ultrasonic
	long			duration;	// the Pulse time received;
	unsigned long	previousMillis;				// will store last time LED was updated
	long			interval;					// blink interval (milliseconds)
	bool			isInitialized;
	
public:
	bool			isRunning;

	explicit 		Ultrasonic();
	
	bool			setupForUltrasonic();
	void 			measure();
	void			makeMeasurement();

private:
	void 			distanceMeasure(void);
	long 			microsecondsToCentimeters(void);
	long 			microsecondsToInches(void);
};

#endif /* Ultrasonic_h */