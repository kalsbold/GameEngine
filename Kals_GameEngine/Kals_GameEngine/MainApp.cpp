#include "MainApp.h"


MainApp::MainApp(void)
	: m_pRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	m_pWindow(0),
	m_pSceneMgr(0),
	m_pCamera(0),
	m_pInputMgr(0),
	m_pMouse(0),
	m_pKeyboard(0),
	m_pUIMgr(0)
{
}

MainApp::~MainApp(void)
{
	if(m_pUIMgr) delete m_pUIMgr;

	Ogre::WindowEventUtilities::removeWindowEventListener(m_pWindow, this);

	windowClosed(m_pWindow);

	delete m_pRoot;
}
bool MainApp::initOgre()
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	m_pRoot = new Ogre::Root(mPluginsCfg);

	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::String name, locType;
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;

			ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}

	if (!(m_pRoot->restoreConfig() || m_pRoot->showConfigDialog()))
		return false;

	m_pWindow = m_pRoot->initialise(true, "TutorialApplication Render Window");

	TextureManager::getSingleton().setDefaultNumMipmaps(5);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	m_pUIMgr = new GUIManager();

	return true;
}

void MainApp::createCamera()
{
	m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_GENERIC);

	m_pCamera = m_pSceneMgr->createCamera("MainCam");
	m_pCamera->setPosition(0, 0, 80);
	m_pCamera->lookAt(0, 0, -300);
	m_pCamera->setNearClipDistance(5);

	Ogre::Viewport* vp = m_pWindow->addViewport(m_pCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	m_pCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));
}

void MainApp::createScene()
{

	m_pUIMgr->createGUIScene();

	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	m_pSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	Entity* ogreHead = m_pSceneMgr->createEntity("Head", "ogrehead.mesh");
	SceneNode* headNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 0, -300));
	headNode->attachObject(ogreHead);


	Ogre::TexturePtr tex = m_pRoot->getTextureManager()->createManual(
		"RTT",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D,
		512,
		512,
		0,
		Ogre::PF_R8G8B8,
		Ogre::TU_RENDERTARGET);
	Ogre::RenderTexture *rtex = tex->getBuffer()->getRenderTarget();


	Ogre::Camera *cam = m_pSceneMgr->createCamera("RTTCam");
	cam->setPosition(0, 50, 100);
	cam->lookAt(0, 0, -300);
	Ogre::Viewport *v = rtex->addViewport(cam);
	v->setOverlaysEnabled(false);
	v->setClearEveryFrame(true);
	v->setBackgroundColour(Ogre::ColourValue::Black);


	CEGUI::Texture &guiTex = m_pUIMgr->GetRender()->createTexture("texname", tex);

	const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), guiTex.getOriginalDataSize());
	CEGUI::BasicImage* image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", "RTTImage"));
	image->setTexture(&guiTex);
	image->setArea(rect);
	image->setAutoScaled(CEGUI::ASM_Both);

	

}

bool MainApp::Run()
{
	if(!initOgre())return false;

	createCamera();

	createScene();

	createFrameListener();

	m_pRoot->startRendering();

	return true;
}

void MainApp::createFrameListener()
{
	// OIS
	LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	OIS::ParamList pl;
	size_t windowHandle = 0;
	std::ostringstream windowHandleStr;

	m_pWindow->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleStr << windowHandle;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

	m_pInputMgr = OIS::InputManager::createInputSystem(pl);

	m_pKeyboard = static_cast<OIS::Keyboard*>(
		m_pInputMgr->createInputObject(OIS::OISKeyboard, false));
	m_pMouse = static_cast<OIS::Mouse*>(
		m_pInputMgr->createInputObject(OIS::OISMouse, false));

	windowResized(m_pWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(m_pWindow, this);

	m_pRoot->addFrameListener(this);
}


bool MainApp::frameRenderingQueued(const FrameEvent& fe)
{
	if (m_pWindow->isClosed()) return false;

	if(m_pUIMgr->Exit())return false;
	m_pKeyboard->capture();
	m_pMouse->capture();

	if (m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;

	return true;
}

void MainApp::windowResized(RenderWindow* rw)
{
	int left, top;
	unsigned int width, height, depth;

	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& ms = m_pMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}
void MainApp::windowClosed(RenderWindow* rw)
{
	if(rw == m_pWindow)
	{
		if(m_pInputMgr)
		{
			m_pInputMgr->destroyInputObject(m_pMouse);
			m_pInputMgr->destroyInputObject(m_pKeyboard);

			OIS::InputManager::destroyInputSystem(m_pInputMgr);
			m_pInputMgr = 0;
		}
	}
}


// OIS::KeyListener
bool MainApp::keyPressed( const OIS::KeyEvent &arg )
{
	m_pUIMgr->keyPressed(arg);
	return true;
}
bool MainApp::keyReleased( const OIS::KeyEvent &arg )
{
	m_pUIMgr->keyReleased(arg);
	return true;
}
// OIS::MouseListener
bool MainApp::mouseMoved( const OIS::MouseEvent &arg )
{
	m_pUIMgr->mouseMoved(arg);
	return true;
}
bool MainApp::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	m_pUIMgr->mousePressed(arg,id);
	return true;
}
bool MainApp::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	m_pUIMgr->mouseReleased(arg,id);
	return true;
}