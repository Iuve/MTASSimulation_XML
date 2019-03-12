/*! \file Globals.hh
  \brief constant parameters used in MTASSimulation
  
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

#include <cstdlib>
#include <stdint.h>

    /** The time width of an event in seconds.*/
    const double g_eventInSeconds = 5e-7;
    const double g_cycleDurationInSeconds = 5.*60.*60.; // 15.*60. for 87Br


#endif // GLOBALS_H
