#include "EngineDevice.h"

//
/*
생성자:
오우거3D의 필수 객체들을 생성한다.
*/
//

EngineDevice::EngineDevice(void)
	: mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mWindow(0),
	mSceneMgr(0),
	mCamera(0),
	mInputMgr(0),
	mMouse(0),
	mKeyboard(0),
	mOverlaySystem(0),
	mTrayMgr(0),
	mCameraMan(0),
	mShutDown(false),
	mCursorWasVisible(true),
	mTerrainGroup(0),
	mTerrainGlobals(0),
	mInfoLabel(0)
{
}

/*
소멸자:
오우거가 종료될때 실행되며 생성된 객체를 지운다.
*/
EngineDevice::~EngineDevice(void)
{
	if(Ninja) delete Ninja;
	if(Penguin) delete Penguin;
	if (mTrayMgr) delete mTrayMgr;
	if (mCameraMan) delete mCameraMan;
	if (mOverlaySystem) delete mOverlaySystem;

	destroyScene();

	// Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	delete mRoot;
}


bool EngineDevice::configure(void)
{
	// Show the configuration dialog and initialise the system.
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg.
	if(mRoot->restoreConfig() || mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise.
		// Here we choose to let the system create a default rendering window by passing 'true'.
		mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

		return true;
	}
	else
	{
		return false;
	}
}

/*
함수명 : void chooseSceneManager
함수 기능 : 생성된 SceneManager를 선택하고 OverlaySystem을 초기화 한다.
*/

void EngineDevice::chooseSceneManager(void)
{
	// Get the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

	// Initialize the OverlaySystem (changed for Ogre 1.9)
	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);
}


/*
함수명 : void createCamera
함수 기능 : 카메라 객체를 생성하고 초기 위치와 시점,시야를 결정하고 카메라 맨 객체에 할당한다.
*/

void EngineDevice::createCamera()
{
	mCamera = mSceneMgr->createCamera("PlayerCam");

	mCamera->setPosition(Ogre::Vector3(500, 700, 600));
	mCamera->lookAt(Ogre::Vector3(500, 700, -1000));
	mCamera->setNearClipDistance(5);

	mCameraMan = new CameraMan(mCamera);
}

/*
함수명 : void createFrameListener
함수 기능 : 각종 입력 객체의 초기화 및 실행창의 설정.UI용 SdkTrayManager 초기화.
*/
void EngineDevice::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	mWindow->reposition(100,100);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	// Set initial mouse clipping size
	windowResized(mWindow);

	// Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mInputContext.mKeyboard = mKeyboard;
	mInputContext.mMouse = mMouse;
	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	mTrayMgr->hideCursor();

	// Create a params panel for displaying sample details
	Ogre::StringVector items;
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

	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	mDetailsPanel->setParamValue(9, "Bilinear");
	mDetailsPanel->setParamValue(10, "Solid");
	mDetailsPanel->hide();

	mRoot->addFrameListener(this);

	mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TerrainInfo", "", 350);

}
//---------------------------------------------------------------------------
void EngineDevice::destroyScene(void)
{

	OGRE_DELETE mTerrainGroup;
	OGRE_DELETE mTerrainGlobals;
}


void EngineDevice::createViewports()
{
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);

	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));

}

void EngineDevice::createResourceListener(void)
{
	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
}

void EngineDevice::loadResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

bool EngineDevice::processUnbufferedInput(const Ogre::FrameEvent & fe)
{


	return true;
}



void EngineDevice::createLight(void)
{
	//// Spotlight
	//Ogre::Light* spotLight = mSceneMgr->createLight("Spotlight");
	//spotLight->setType(Ogre::Light::LT_SPOTLIGHT);

	//spotLight->setDiffuseColour(Ogre::ColourValue(0, 0, 1));
	//spotLight->setSpecularColour(Ogre::ColourValue(0, 0, 1));

	//spotLight->setDirection(-1, -1, 0);
	//spotLight->setPosition(Ogre::Vector3(200, 200, 0));

	//spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

	// Directional light
	Light = mSceneMgr->createLight("DirectionalLight");
	Light->setType(Ogre::Light::LT_DIRECTIONAL);



	Ogre::Vector3 lightDir(.55, -.3, .75);
	lightDir.normalise();

	Light->setDirection(lightDir);


	Light->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	Light->setSpecularColour(Ogre::ColourValue(1, 1, 1));
	//// Point light
	//Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
	//pointLight->setType(Ogre::Light::LT_POINT);

	//pointLight->setDiffuseColour(.3, .3, .3);
	//pointLight->setSpecularColour(.3, .3, .3);

	//pointLight->setPosition(Ogre::Vector3(0, 150, 250));
}


void EngineDevice::createScene()
{

	bool infiniteClip =
		mRoot->getRenderSystem()->getCapabilities()->hasCapability(
		Ogre::RSC_INFINITE_FAR_PLANE);

	if (infiniteClip)
		mCamera->setFarClipDistance(0);
	else
		mCamera->setFarClipDistance(50000);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);


	// Terrain
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
		mSceneMgr,
		Ogre::Terrain::ALIGN_X_Z,
		513, 12000.0);
	mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	configureTerrainDefaults(Light);

	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);

	mTerrainGroup->loadAllTerrains(true);

	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();

		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	mTerrainGroup->freeTemporaryResources();

	// Sky Techniques
	// mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 300, false);
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);



	// 지형효과.
	// Ogre::Plane plane;
	// plane.d = 1000;
	// plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;

	// mSceneMgr->setSkyPlane(
	//   true, plane, "Examples/SpaceSkyPlane", 1500, 40, true, 1.5, 150, 150);


	//// Create Plane ground
	//Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	//Ogre::MeshManager::getSingleton().createPlane(
	//	"ground",
	//	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	//	plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	//Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);

	//groundEntity->setMaterialName("Examples/Rockwall");
	//groundEntity->setCastShadows(false);


}

void EngineDevice::setupResources(void)
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app.
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location.
			if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
				archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}

/*
함수명 : bool setup
함수 기능 : 루트객체 생성과 각종 초기설정함수 호출과 구동.
*/
bool EngineDevice::setup()
{
	mRoot = new Ogre::Root(mPluginsCfg);

	setupResources();

	bool carryOn = configure();
	if (!carryOn) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	createLight();
	// Create the scene
	createScene();
	// Create Light
	

	createFrameListener();

	Ninja = new Mesh("ninja.mesh",Ogre::Vector3(500,500,500));

	Penguin = new Mesh("penguin.mesh",Ogre::Vector3(500,500,100));

	Ninja->Render(mSceneMgr);
	Penguin->Render(mSceneMgr);
	return true;
}

/*
함수명 : bool go
함수 기능 : 오우거의 모든 렌더링부를 실행한다.
*/

bool EngineDevice::go()
{

#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	if (!setup())
		return false;

	mRoot->startRendering();

	//destroyScene();

	return true;
}

/*
함수명 : bool frameRenderingQueued
함수 기능 : 렌더링버퍼가 입력받기 전에 호출됨. 키입력을 받아들임.
*/

bool EngineDevice::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	
	if(mWindow->isClosed())
		return false;

	if(mShutDown)
		return false;

	// Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	mTrayMgr->frameRenderingQueued(fe);

	if (!mTrayMgr->isDialogVisible())
	{
		mCameraMan->frameRenderingQueued(fe);   // If dialog isn't up, then update the camera
		if (mDetailsPanel->isVisible())          // If details panel is visible, then update its contents
		{
			mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
			mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
			mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
			mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
			mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
			mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
			mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
		}
	}

	//지형표현쪽 UI 라벨
	if (mTerrainGroup->isDerivedDataUpdateInProgress())
	{
		mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
		mInfoLabel->show();

		if (mTerrainsImported)
			mInfoLabel->setCaption("Building terrain...");
		else
			mInfoLabel->setCaption("Updating textures...");
	}
	else
	{
		mTrayMgr->removeWidgetFromTray(mInfoLabel);
		mInfoLabel->hide();

		if (mTerrainsImported)
		{
			mTerrainGroup->saveAllTerrains(true);
			mTerrainsImported = false;
		}
	}

	if(!processUnbufferedInput(fe))
		return false;



	//// Moving the Ninja

	Ninja->Move(mKeyboard,mMouse,fe);
	Penguin->Move();
	
	return true;
}



void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
  img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
 
  if (flipX)
    img.flipAroundY();
  if (flipY)
    img.flipAroundX();
 
}
 
void EngineDevice::defineTerrain(long x, long y)
{
  Ogre::String filename = mTerrainGroup->generateFilename(x, y);
 
  bool exists =
    Ogre::ResourceGroupManager::getSingleton().resourceExists(
      mTerrainGroup->getResourceGroup(),
      filename);
 
  if (exists)
    mTerrainGroup->defineTerrain(x, y);
  else
  {
    Ogre::Image img;
    getTerrainImage(x % 2 != 0, y % 2 != 0, img);
    mTerrainGroup->defineTerrain(x, y, &img);
 
    mTerrainsImported = true;
  }
}
 
void EngineDevice::initBlendMaps(Ogre::Terrain* terrain)
{
  Ogre::Real minHeight0 = 70;
  Ogre::Real fadeDist0 = 40;
  Ogre::Real minHeight1 = 70;
  Ogre::Real fadeDist1 = 15;
 
  Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
  Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
 
  float* pBlend0 = blendMap0->getBlendPointer();
  float* pBlend1 = blendMap1->getBlendPointer();
 
  for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
  {
    for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
    {
      Ogre::Real tx, ty;
 
      blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
      Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
      Ogre::Real val = (height - minHeight0) / fadeDist0;
      val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
      *pBlend0++ = val;
 
      val = (height - minHeight1) / fadeDist1;
      val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
      *pBlend1++ = val;
    }
  }
 
  blendMap0->dirty();
  blendMap1->dirty();
  blendMap0->update();
  blendMap1->update();
 
}
 
void EngineDevice::configureTerrainDefaults(Ogre::Light* light)
{
  mTerrainGlobals->setMaxPixelError(8);
  mTerrainGlobals->setCompositeMapDistance(3000);
 
  mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
  mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
  mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
 
  Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
  importData.terrainSize = 513;
  importData.worldSize = 12000.0;
  importData.inputScale = 600;
  importData.minBatchSize = 33;
  importData.maxBatchSize = 65;
 
  importData.layerList.resize(3);
  importData.layerList[0].worldSize = 100;
  importData.layerList[0].textureNames.push_back(
    "dirt_grayrocky_diffusespecular.dds");
  importData.layerList[0].textureNames.push_back(
    "dirt_grayrocky_normalheight.dds");
  importData.layerList[1].worldSize = 30;
  importData.layerList[1].textureNames.push_back(
    "grass_green-01_diffusespecular.dds");
  importData.layerList[1].textureNames.push_back(
    "grass_green-01_normalheight.dds");
  importData.layerList[2].worldSize = 200;
  importData.layerList[2].textureNames.push_back(
    "growth_weirdfungus-03_diffusespecular.dds");
  importData.layerList[2].textureNames.push_back(
    "growth_weirdfungus-03_normalheight.dds");
 
}
 





//---------------------------------------------------------------------------
bool EngineDevice::keyPressed( const OIS::KeyEvent &arg )
{
	if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

	if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
	{
		mTrayMgr->toggleAdvancedFrameStats();
	}
	else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
	{
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
		else
		{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
		}
	}
	else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
	{
		Ogre::String newVal;
		Ogre::TextureFilterOptions tfo;
		unsigned int aniso;

		switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
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
		mDetailsPanel->setParamValue(9, newVal);
	}
	else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
	{
		Ogre::String newVal;
		Ogre::PolygonMode pm;

		switch (mCamera->getPolygonMode())
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

		mCamera->setPolygonMode(pm);
		mDetailsPanel->setParamValue(10, newVal);
	}
	else if(arg.key == OIS::KC_F5)   // refresh all textures
	{
		Ogre::TextureManager::getSingleton().reloadAll();
	}
	else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
	{
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
	}
	else if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}

	mCameraMan->injectKeyDown(arg);
	return true;
}
//---------------------------------------------------------------------------
bool EngineDevice::keyReleased(const OIS::KeyEvent &arg)
{
	mCameraMan->injectKeyUp(arg);
	return true;
}
//---------------------------------------------------------------------------
bool EngineDevice::mouseMoved(const OIS::MouseEvent &arg)
{
	//if (mTrayMgr->injectMouseMove(arg)) return true;
	//mCameraMan->injectMouseMove(arg);
	return true;
}
//---------------------------------------------------------------------------
bool EngineDevice::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (mTrayMgr->injectMouseDown(arg, id)) return true;
	mCameraMan->injectMouseDown(arg, id);
	return true;
}
//---------------------------------------------------------------------------
bool EngineDevice::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (mTrayMgr->injectMouseUp(arg, id)) return true;
	mCameraMan->injectMouseUp(arg, id);
	return true;
}

/*
함수명 : void windowResized
함수 기능 : 실행창의 위치와 크기를 설정.
*/
void EngineDevice::windowResized(Ogre::RenderWindow* rw)
{
	int left, top;
	unsigned int width, height, depth;

	rw->getMetrics(width, height, depth, left, top);
	rw->resize(1024,768);

	//rw->resize(768,768);
	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

/*
함수명 : void windowClosed
함수 기능 : 실행창을 종료한다. 입력장치 또한 제거한다.
*/

void EngineDevice::windowClosed(Ogre::RenderWindow* rw)
{
	if(rw == mWindow)
	{
		if(mInputMgr)
		{
			mInputMgr->destroyInputObject(mMouse);
			mInputMgr->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputMgr);
			mInputMgr = 0;
		}
	}
}









#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		EngineDevice app;

		try
		{
			app.go();
		}
		catch(Ogre::Exception& e)
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(
				NULL,
				e.getFullDescription().c_str(),
				"An exception has occured!",
				MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}
#ifdef __cplusplus
}
#endif