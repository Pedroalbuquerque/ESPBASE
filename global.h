#ifndef GLOBAL_H
#define GLOBAL_H


bool firstStart = true;								// On firststart = true, NTP will try to get a valid time
int AdminTimeOutCounter = 0;							// Counter for Disabling the AdminMode
volatile unsigned long UnixTimestamp = 0;	// GLOBALTIME  ( Will be set by NTP)
int cNTP_Update = 0;											// Counter for Updating the time via NTP
Ticker tkSecond;												  // Second - Timer for Updating Datetime Structure

#define LED_esp 2

//custom declarations
long absoluteActualTime, actualTime;
long  customWatchdog;                     // WatchDog to detect main loop blocking. There is a builtin WatchDog to the chip firmare not related to this one


#endif
