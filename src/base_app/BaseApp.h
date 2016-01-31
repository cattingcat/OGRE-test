#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

class BaseApplication :
		public Ogre::FrameListener,
		public OIS::KeyListener,
		public OIS::MouseListener,
		OgreBites::SdkTrayListener {

public:
	BaseApplication(void);
	virtual ~BaseApplication(void);

	virtual void go(void);

protected:
	virtual bool setup();
	virtual bool configure(void);
	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);
	virtual void createScene(void);
	virtual void destroyScene(void);
	virtual void createViewports(void);
	virtual void setupResources(void);
	virtual void createResourceListener(void);
	virtual void loadResources(void);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// OIS::KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	// OIS::MouseListener
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	Ogre::Root *_root;
	Ogre::Camera* _camera;
	Ogre::SceneManager* _sceneMgr;
	Ogre::RenderWindow* _window;
	Ogre::String _resourcesCfg;
	Ogre::String _pluginsCfg;
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 9
	Ogre::OverlaySystem *mOverlaySystem;
#endif
	// OgreBites
	OgreBites::SdkTrayManager* _trayMgr;
	OgreBites::SdkCameraMan* _cameraCtrl;       // basic camera controller
	OgreBites::ParamsPanel* _detailsPanel;     // sample details panel
	bool mCursorWasVisible;                    // was cursor visible before dialog appeared
	bool mShutDown;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    _mouse;
	OIS::Keyboard* _keyboard;
};
