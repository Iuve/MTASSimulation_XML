
#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

#include <cstdlib>
#include <stdint.h>

    /** The time width of an event in seconds.*/
    //const double g_eventInSeconds = 5e-7;
    //const double g_cycleDurationInSeconds = 300.; // 15.*60. for 87Br
    
	//static double g_eventInSeconds;
    //static double g_cycleDurationInSeconds;
    
    void SetEventLength(double time) { g_eventInSeconds = time; }
	void SetCycleLength(double time) { g_cycleDurationInSeconds = time; }

#endif // GLOBALS_H
