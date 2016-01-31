#include "BaseApp.h"
#include "../utils/OIS_devices.h"

using namespace Ogre;

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
	: _root(0),
	_camera(0),
	_sceneMgr(0),
	_window(0),
	_resourcesCfg(""),
	_pluginsCfg(""),
	_trayMgr(0),
	_cameraCtrl(0),
	_detailsPanel(0),
	mCursorWasVisible(false),
	mShutDown(false),
	mInputManager(0),
	_mouse(0),
	_keyboard(0) {

}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void) {
	if (_trayMgr) delete _trayMgr;
	if (_cameraCtrl) delete _cameraCtrl;
	delete _root;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void) {
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(_root->showConfigDialog()) {
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		_window = _root->initialise(true, "TutorialApplication Render Window");

		return true;
	} else {
		return false;
	}
}
//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void) {
	// Get the SceneManager, in this case a generic one
	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, 2, INSTANCING_CULLING_THREADED, "MySceneManager");

#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 9
	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);
#endif
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void) {
	// Create the camera
	_camera = _sceneMgr->createCamera("PlayerCam");

	// Position it at 500 in Z direction
	_camera->setPosition(Vector3(0,0,80));
	// Look back along -Z
	_camera->lookAt(Vector3(0,0,-300));
	_camera->setNearClipDistance(5);

	// create a default camera controller
	_cameraCtrl = new OgreBites::SdkCameraMan(_camera);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void) {
	LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	auto keyMouse = getForWindow(_window);

	_keyboard = keyMouse.keyboard;
	_mouse = keyMouse.mouse;

	OgreBites::InputContext inputContext;
	inputContext.mKeyboard = _keyboard;
	inputContext.mMouse =_mouse;

	_trayMgr = new OgreBites::SdkTrayManager("InterfaceName", _window, inputContext, this);
	_trayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	_trayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	_trayMgr->hideCursor();

	// create a params panel for displaying sample details
	StringVector items;
	items.push_back("cam.pX");
	items.push_back("cam.pY");
	items.push_back("cam.pZ");
	items.push_back("");
	items.push_back("cam.oW");
	items.push_back("cam.oX");
	items.push_back("cam.oY");
	items.push_back("cam.oZ");
	items.push_back("");
	items.push_back("Filtering");
	items.push_back("Poly Mode");

	_detailsPanel = _trayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	_detailsPanel->setParamValue(9, "Bilinear");
	_detailsPanel->setParamValue(10, "Solid");
	_detailsPanel->hide();

	_root->addFrameListener(this);
	_mouse->setEventCallback(this);
	_keyboard->setEventCallback(this);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void) {

}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void) {
	// Create one viewport, entire window
	Viewport *vp = _window->addViewport();
	vp->clear(FBT_COLOUR | FBT_DEPTH, Ogre::ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	_camera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void) {
	// Load resource paths from config file
	ConfigFile cf;
	cf.load(_resourcesCfg);

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	while (seci.hasMoreElements()) {
		String secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();

		for (auto i = settings->begin(); i != settings->end(); ++i) {
			String
				typeName = i->first,
				archName = i->second;

			ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void) {

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void) {
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
	_resourcesCfg = "resources/resources.cfg";
	_pluginsCfg = "resources/plugins.cfg";

	if (!setup())
		return;

	_root->startRendering();

	// clean up
	destroyScene();
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void) {
	_root = new Root(_pluginsCfg);

	setupResources();

	bool carryOn = configure();
	if (!carryOn) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	// Create the scene
	createScene();

	createFrameListener();

	return true;
};
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(_window->isClosed())
		return false;

	if(mShutDown)
		return false;

	//Need to capture/update each device
	_keyboard->capture();
	_mouse->capture();

	_trayMgr->frameRenderingQueued(evt);

	if (!_trayMgr->isDialogVisible())
	{
		_cameraCtrl->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
		if (_detailsPanel->isVisible())   // if details panel is visible, then update its contents
		{
			_detailsPanel->setParamValue(0, Ogre::StringConverter::toString(_camera->getDerivedPosition().x));
			_detailsPanel->setParamValue(1, Ogre::StringConverter::toString(_camera->getDerivedPosition().y));
			_detailsPanel->setParamValue(2, Ogre::StringConverter::toString(_camera->getDerivedPosition().z));
			_detailsPanel->setParamValue(4, Ogre::StringConverter::toString(_camera->getDerivedOrientation().w));
			_detailsPanel->setParamValue(5, Ogre::StringConverter::toString(_camera->getDerivedOrientation().x));
			_detailsPanel->setParamValue(6, Ogre::StringConverter::toString(_camera->getDerivedOrientation().y));
			_detailsPanel->setParamValue(7, Ogre::StringConverter::toString(_camera->getDerivedOrientation().z));
		}
	}

	return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
	if (_trayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

	if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
	{
		_trayMgr->toggleAdvancedFrameStats();
	}
	else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
	{
		if (_detailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			_trayMgr->moveWidgetToTray(_detailsPanel, OgreBites::TL_TOPRIGHT, 0);
			_detailsPanel->show();
		}
		else
		{
			_trayMgr->removeWidgetFromTray(_detailsPanel);
			_detailsPanel->hide();
		}
	}
	else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
	{
		Ogre::String newVal;
		Ogre::TextureFilterOptions tfo;
		unsigned int aniso;

		switch (_detailsPanel->getParamValue(9).asUTF8()[0])
		{
		case 'B':
			newVal = "Trilinear";
			tfo = Ogre::TFO_TRILINEAR;
			aniso = 1;
			break;
		case 'T':
			newVal = "Anisotropic";
			tfo = Ogre::TFO_ANISOTROPIC;
			aniso = 8;
			break;
		case 'A':
			newVal = "None";
			tfo = Ogre::TFO_NONE;
			aniso = 1;
			break;
		default:
			newVal = "Bilinear";
			tfo = Ogre::TFO_BILINEAR;
			aniso = 1;
		}

		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
		_detailsPanel->setParamValue(9, newVal);
	}
	else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
	{
		Ogre::String newVal;
		Ogre::PolygonMode pm;

		switch (_camera->getPolygonMode())
		{
		case Ogre::PM_SOLID:
			newVal = "Wireframe";
			pm = Ogre::PM_WIREFRAME;
			break;
		case Ogre::PM_WIREFRAME:
			newVal = "Points";
			pm = Ogre::PM_POINTS;
			break;
		default:
			newVal = "Solid";
			pm = Ogre::PM_SOLID;
		}

		_camera->setPolygonMode(pm);
		_detailsPanel->setParamValue(10, newVal);
	}
	else if(arg.key == OIS::KC_F5)   // refresh all textures
	{
		Ogre::TextureManager::getSingleton().reloadAll();
	}
	else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
	{
		_window->writeContentsToTimestampedFile("screenshot", ".jpg");
	}
	else if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}

	_cameraCtrl->injectKeyDown(arg);
	return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
	_cameraCtrl->injectKeyUp(arg);
	return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg ) {
	if (_trayMgr->injectMouseMove(arg)) return true;
	_cameraCtrl->injectMouseMove(arg);
	return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
	if (_trayMgr->injectMouseDown(arg, id)) return true;
	_cameraCtrl->injectMouseDown(arg, id);
	return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
	if (_trayMgr->injectMouseUp(arg, id)) return true;
	_cameraCtrl->injectMouseUp(arg, id);
	return true;
}


void BaseApplication::createScene(void) {

}
