///
/// @file		Sweep.h
/// @brief		Library header
/// @details	<#details#>
/// @n
/// @n @b		Project vmdev
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		William Snook
/// @author		billsnook
///
/// @date		6/30/18 4:11 PM
///
/// @copyright	(c) William Snook, 2018
///


#include "Arduino.h"

#ifndef Sweep_h
#define Sweep_h

#include <Servo.h>

class Sweep {
	
	int				sweepPin;					// the number of the PWM pin for the servo
	int				sweepPosition;				// sweepPosition used for the servo position
	unsigned long	previousMillis;				// will store last time sweep was updated
	long			interval;					// blink interval (milliseconds)
	int				maxPosition;
	int				minPosition;
	int				sweepIncrement;

	bool			up;
	
	bool			isInitialized;
	
	Servo			myservo;  // create servo object to control a servo

public:
	bool			isRunning;
	int				sweepType;

	explicit Sweep();
	
	bool	setupForSweep();
	bool	resetForSweep();
	
	void	centerSweep();
	
	void	checkSweep();
	
	void	startSweep();
	void	runSweep();
	
	void	startScan();
	void	startPingTrack();
	void	runScan();
	
};

#endif // Sweep_h
