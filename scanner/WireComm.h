///
/// @file		WireComm.h
/// @brief		Library header
/// @details	<#details#>
/// @n
/// @n @b		Project scanner
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		William Snook
/// @author		billsnook
///
/// @date		7/22/18 5:02 PM
///
/// @copyright	(c) William Snook, 2018
///

#include <Wire.h>

#ifndef WireComm_h
#define WireComm_h


class WireComm {
	
	int				wireAddress;	// the address of i2c if slave
	bool			isSetup;		// can only be setup once
	bool			isMaster;		// mode of this device

public:
	bool			isRunning;

	explicit		WireComm();

	void			setupForWireComm( bool beMaster );
	void			resetForWireComm();

	void			readWireComm( int getBytes );
	void			writeWireComm( char *data );
	
	void			runWireComm();
	
	static void		requestEvent();
	static void		receiveEvent( int howMany );

};

#endif // WireComm_h
