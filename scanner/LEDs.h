///
/// @file		LEDs.h
/// @author		William Snook
/// @author		billsnook
///
/// @date		6/2/18 8:42 AM
/// @copyright	(c) William Snook, 2018

#ifndef LEDs_h
#define LEDs_h

#include <Arduino.h>


class LEDs {
	
//	bool    		debug;
	int				ledPin =  LED_BUILTIN;		// the number of the LED pin
	int				ledState;					// ledState used to set the LED
	unsigned long	previousMillis;				// will store last time LED was updated
	long			interval;					// blink interval (milliseconds)

	bool			isInitialized;

public:
	bool			isRunning;

	explicit LEDs();
	
//	speed_array 	forward[SPEED_ARRAY];
//	speed_array 	reverse[SPEED_ARRAY];
	
	
	bool	setupForLEDs();
	bool	resetForLEDs();
	
	void	toggle();
};

#endif // LEDs_h
