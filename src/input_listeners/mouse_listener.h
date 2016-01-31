#pragma once
#include <OIS.h>
#include <iostream>

class SimpleMouseListener : public OIS::MouseListener {
public:
	bool mouseMoved(const OIS::MouseEvent& e) {
		std::cout <<"sppeds : " << e.state.X.rel << " " << e.state.Y.rel << std::endl;
		std::cout <<"abs pos: " << e.state.X.abs << " " << e.state.Y.abs << std::endl;
		return true;
	}

	bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
		return true;
	}

	bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
		return true;
	}
};
