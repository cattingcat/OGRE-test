#pragma once
#include "OGRE/OgreFrameListener.h"
#include "OIS.h"



class MyFrameListener : public Ogre::FrameListener {
public:
	MyFrameListener(OIS::Keyboard *kb, OIS::Mouse *mc): _mouse(mc), _keyboard(kb) {

	}

	virtual bool frameStarted(const Ogre::FrameEvent &evt) override;
	virtual bool frameEnded(const Ogre::FrameEvent &evt) override;

private:
	OIS::Mouse *_mouse;
	OIS::Keyboard *_keyboard;
};
