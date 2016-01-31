#include "my_frame_listener.h"
#include <iostream>
#include <cstdint>

uint64_t fc = 0;

bool MyFrameListener::frameStarted(const Ogre::FrameEvent &evt) {
	_keyboard->capture();
	_mouse->capture();

	if(_keyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	return true;
}

bool MyFrameListener::frameEnded(const Ogre::FrameEvent &evt) {
	return true;
}
