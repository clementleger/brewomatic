#ifndef _BREWOMATIC_CONFIG_SANITY
#define _BREWOMATIC_CONFIG_SANITY

#include "Macros.h"

#if !defined(HEATER_CONTROL_TYPE)
# error "Invalid heater control"
#else
# if !defined(ZERO_CROSSING_DETECT_PIN)
#  error "Triac control uses zero crossing ! Please setup pin correctly"
# endif
# if !defined(TRIAC_CONTROL_PIN)
#  error "Triac control pin not defined"
# endif
#endif

#if defined(ZERO_CROSSING_DETECT_PIN)
# if ZERO_CROSSING_DETECT_PIN != 2 && ZERO_CROSSING_DETECT_PIN != 3
#  error "Invalid zero crossing pin (must be 2 or 3)"
# endif
#endif

#if !defined(DISP_LANGUAGE)
# error "Undefined language"
#endif

#if ENABLED(LIQUID_CRYSTAL_DISPLAY)
# define DISPLAY_TYPE	DisplayLiquidCrystal
#elif ENABLED(UCGLIB_DISPLAY)
# define DISPLAY_TYPE	DisplayUcglib
#else
# error "Undefined display"
#endif

#if ENABLED(TEMP_MAX31865)
# define TEMP_PROBE_TYPE	TempMax31865
#elif ENABLED(TEMP_DS18B20)
# define TEMP_PROBE_TYPE	TempDS18B20
#else
# error "Undefined temp probe"
#endif

#endif
