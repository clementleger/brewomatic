#ifndef _BREWOMATIC_CONFIG_SANITY
#define _BREWOMATIC_CONFIG_SANITY

#include "Macros.h"

#if !defined(TEMP_PROBE_TYPE)
# error "Invalid temp probe type !"
#endif

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

#endif
