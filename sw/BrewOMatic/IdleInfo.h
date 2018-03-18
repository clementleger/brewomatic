#ifndef _IDLE_INFO_H
#define _IDLE_INFO_H

class BrewOMatic;

class IdleInfo {
	public:
		IdleInfo(float temp, const char *title) {
			mTemp = temp;
			mTitle = title;
		};
		float mTemp;
		const char *mTitle;
};

#endif
