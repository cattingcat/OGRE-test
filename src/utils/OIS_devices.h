#pragma once

#include <OIS.h>
#include <Ogre.h>
#include <iostream>

struct PCInputDevices {
	OIS::Keyboard *keyboard;
	OIS::Mouse *mouse;
};

PCInputDevices getForWindow(Ogre::RenderWindow *window) {

	size_t wndHandler = 0;



	//tell OIS about the Ogre window
	Ogre::String key = "WINDOW";
	window->getCustomAttribute(key, &wndHandler);

	std::ostringstream converter;
	converter << wndHandler;
	Ogre::String val = converter.str();

	OIS::ParamList params;
	params.insert(std::make_pair(key, val));

	//setup the manager, keyboard and mouse to handle input
	OIS::InputManager *inputManager = OIS::InputManager::createInputSystem(params);

	OIS::Keyboard *keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
	OIS::Mouse *mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));

	//tell OIS about the window's dimensions
	uint width, height, depth;
	int top, left;

	window->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mouse->getMouseState();
	ms.width = width;
	ms.height = height;

	return {
		keyboard,
		mouse
	};
}
