
#if TEMP_PROBE_TYPE != TempMax31865 && TEMP_PROBE_TYPE != TempDS18B20
# error "Invalid temp probe type !"
#endif

#if HEATER_CONTROL_TYPE == TRIAC_CONTROL
# if !defined(ZERO_CROSSING_DETECT_PIN)
#  error "Triac control uses zero crossing ! Please setup pin correctly"
# endif
# if !defined(TRIAC_CONTROL_PIN)
#  error "Triac control pin not defined"
# endif
#else
# error "Undefined heater control"
#endif

#if defined(ZERO_CROSSING_DETECT_PIN)
# if ZERO_CROSSING_DETECT_PIN != 2 && ZERO_CROSSING_DETECT_PIN != 3
#  error "Invalid zero crossing pin (must be 2 or 3)"
# endif
#endif
