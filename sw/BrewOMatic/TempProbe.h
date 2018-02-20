#ifndef _TEMP_PROBE_H
#define _TEMP_PROBE_H

class TempProbe {
	public:
		virtual int setup() = 0;
		virtual int getTemp(float *value) = 0;
};

#endif /* _TEMP_PROBE_H */
