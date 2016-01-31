#pragma once
#include <OIS.h>

class SimpleKeyListener : public OIS::KeyListener {
public:
	bool keyPressed(const OIS::KeyEvent& e){ return true; }

	bool keyReleased(const OIS::KeyEvent& e){ return true; }
};
