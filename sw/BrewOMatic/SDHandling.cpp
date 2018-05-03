#include "Config.h"
#include "SerialOutput.h"

#if ENABLED(USE_SD_CARD)

#include "SDHandling.h"

SDCard SDCard::mInstance = SDCard();


SDCard::SDCard():
mCard(),
mVolume(),
mRoot(),
mPresent(false)
{

}

int SDCard::init()
{
	int ret;

	ret = mCard.init(SPI_HALF_SPEED, SD_CARD_CHIP_SELECT);
	if (!ret) {
		dbgOutput("Card init failed\n");
		return 1;
	}

	ret = mVolume.init(mCard);
	if (!ret) {
		dbgOutput("Volume init failed\n");
		return 1;
	}

	mRoot.openRoot(mVolume);
	mPresent = true;
	dbgOutput("SD card present\n");
	
	return 0;
}

#endif
