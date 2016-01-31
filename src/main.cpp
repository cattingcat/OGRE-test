#include <iostream>
#include <cstdint>
#include <OGRE/Ogre.h>
#include "base_app/BaseApp.h"


using namespace std;
using namespace Ogre;

int main(int argc, char *argv[]) {
	BaseApplication app;
	app.go();

	/*
	Root *root = new Root("resources/plugins.cfg");


	root->showConfigDialog();
	root->initialise(false);

	SceneManager *sceneMgr = root->createSceneManager(ST_GENERIC, 2, INSTANCING_CULLING_THREADED, "MySceneManager");
	RenderWindow *window = root->createRenderWindow("RenderTargetName", 800, 600, false);
	Camera *camera = sceneMgr->createCamera("MainCamera");
	Viewport *vp = window->addViewport();

	sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Entity* ogreEntity = sceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode* ogreNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);

	auto input = getForWindow(window);


	SimpleKeyListener* keyListener = new SimpleKeyListener();
	input.keyboard->setEventCallback(keyListener);

	SimpleMouseListener* mouseListener = new SimpleMouseListener();
	input.mouse->setEventCallback(mouseListener);

	MyFrameListener *mfl = new MyFrameListener(input.keyboard, input.mouse);
	root->addFrameListener(mfl);

	root->startRendering();

	uint64_t i = 42u;
	cout << "Configured! " << i << endl;*/

	return 0;
}
