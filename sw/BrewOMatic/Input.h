#ifndef _INPUT_H
#define _INPUT_H

class Input {
	public:
		enum BUTTON {
			BUTTON_NONE = 0,
			BUTTON_OK,
			BUTTON_NEXT,
			BUTTON_PREV,
		};

		virtual int getButtonPressed() = 0;
};

#endif /* _INPUT_H */
