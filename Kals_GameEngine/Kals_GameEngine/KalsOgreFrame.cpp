#include "KalsOgreFrame.h"

KalsOgreFrame::KalsOgreFrame()
	: m_pRoot(0),
	m_pResourcesCfg(Ogre::StringUtil::BLANK),
	m_pPluginsCfg(Ogre::StringUtil::BLANK),
	m_pWindow(0),
    m_pSceneMgr(0),
    m_pCamera(0),
    m_pInputMgr(0),
    m_pMouse(0),
    m_pKeyboard(0),
	m_pPlayer(0)
{
}
KalsOgreFrame::~KalsOgreFrame()
{
	Ogre::WindowEventUtilities::removeWindowEventListener(m_pWindow, this);
 
	windowClosed(m_pWindow);

	if(m_pRoot)
	{
		delete m_pRoot;
	}
}

bool KalsOgreFrame::InitOgre()
{
#ifdef _DEBUG
	m_pResourcesCfg = "resources_d.cfg";
	m_pPluginsCfg = "plugins_d.cfg";
#else
	m_pResourcesCfg = "resources.cfg";
	m_pPluginsCfg = "plugins.cfg";
#endif


	//root 객체 초기화.
	m_pRoot = new Ogre::Root(m_pPluginsCfg);

	if(m_pRoot == NULL)
		return false;

	return true;
}

bool KalsOgreFrame::InitResource()
{
	//리소스 로딩.
	Ogre::ConfigFile cf;
	cf.load(m_pResourcesCfg);

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
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}

	if (!(m_pRoot->restoreConfig() || m_pRoot->showConfigDialog()))
    return false;

	m_pWindow = m_pRoot->initialise(true, "Kals Engine.....");

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	return true;
}

void KalsOgreFrame::CreateCamera()
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


void KalsOgreFrame::CreateScene()
{
	m_pPlayer = new PlayerCh(m_pSceneMgr);

	Ogre::Entity* ogreEntity = m_pSceneMgr->createEntity("ogrehead.mesh");

	Ogre::SceneNode* ogreNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);

	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	Ogre::Light* light = m_pSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);
}

void KalsOgreFrame::InitOIS()
{
	// OIS
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

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

	m_pKeyboard->setEventCallback(this);
	m_pMouse->setEventCallback(this);

}

bool KalsOgreFrame::Run()
{
	if(!InitOgre()) return false;
	if(!InitResource()) return false; 

	CreateCamera();
	CreateScene();
	InitOIS();

	windowResized(m_pWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(m_pWindow, this);

	m_pRoot->addFrameListener(this);

	m_pRoot->startRendering();

	return true;
}


void KalsOgreFrame::windowResized(Ogre::RenderWindow* rw)
{
  int left, top;
  unsigned int width, height, depth;
 
  rw->getMetrics(width, height, depth, left, top);
 
  const OIS::MouseState& ms = m_pMouse->getMouseState();
  ms.width = width;
  ms.height = height;
}
 
bool KalsOgreFrame::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
  if (m_pWindow->isClosed()) return false;
 
  m_pKeyboard->capture();
  m_pMouse->capture();
 
  if (m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
 
  m_pPlayer->MoveCh(fe,mDirection);

  return true;
}
 

void KalsOgreFrame::windowClosed(Ogre::RenderWindow* rw)
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



bool KalsOgreFrame::keyPressed(const OIS::KeyEvent& ke)
{
  switch (ke.key)
  {
  case OIS::KC_1:
	  m_pPlayer->getCam()->getParentSceneNode()->detachObject(m_pPlayer->getCam());
    m_pCamNode = m_pSceneMgr->getSceneNode("CamNode1");
    m_pCamNode->attachObject(m_pPlayer->getCam());
	m_pPlayer->SetCamNode(m_pCamNode);
    break;
 
  case OIS::KC_2:
    m_pPlayer->getCam()->getParentSceneNode()->detachObject(m_pPlayer->getCam());
    m_pCamNode = m_pSceneMgr->getSceneNode("CamNode2");
    m_pCamNode->attachObject(m_pPlayer->getCam());
	m_pPlayer->SetCamNode(m_pCamNode);
    break;
 
  case OIS::KC_UP:
  case OIS::KC_W:
    mDirection.z = -mMove;
    break;
 
  case OIS::KC_DOWN:
  case OIS::KC_S:
    mDirection.z = mMove;
    break;
 
  case OIS::KC_LEFT:
  case OIS::KC_A:
    mDirection.x = -mMove;
    break;
 
  case OIS::KC_RIGHT:
  case OIS::KC_D:
    mDirection.x = mMove;
    break;
 
  case OIS::KC_PGDOWN:
  case OIS::KC_E:
    mDirection.y = -mMove;
    break;
 
  case OIS::KC_PGUP:
  case OIS::KC_Q:
    mDirection.y = mMove;
    break;
 
  default:
    break;
  }
 
  return true;
}
 
bool KalsOgreFrame::keyReleased(const OIS::KeyEvent& ke)
{
  switch (ke.key)
  {
  case OIS::KC_UP:
  case OIS::KC_W:
    mDirection.z = 0;
    break;
 
  case OIS::KC_DOWN:
  case OIS::KC_S:
    mDirection.z = 0;
    break;
 
  case OIS::KC_LEFT:
  case OIS::KC_A:
    mDirection.x = 0;
    break;
 
  case OIS::KC_RIGHT:
  case OIS::KC_D:
    mDirection.x = 0;
    break;
 
  case OIS::KC_PGDOWN:
  case OIS::KC_E:
    mDirection.y = 0;
    break;
 
  case OIS::KC_PGUP:
  case OIS::KC_Q:
    mDirection.y = 0;
    break;
 
  default:
    break;
  }
 
  return true;
}
 
bool KalsOgreFrame::mouseMoved(const OIS::MouseEvent& me)
{
  if (me.state.buttonDown(OIS::MB_Right))
  {
    m_pCamNode->yaw(Ogre::Degree(-mRotate * me.state.X.rel), Ogre::Node::TS_WORLD);
    m_pCamNode->pitch(Ogre::Degree(-mRotate * me.state.Y.rel), Ogre::Node::TS_LOCAL);
  }
 
  return true;
}
 
bool KalsOgreFrame::mousePressed(
  const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
  Ogre::Light* light = m_pSceneMgr->getLight("Light1");
 
  switch (id)
  {
  case OIS::MB_Left:
    light->setVisible(!light->isVisible());
    break;
  default:
    break;
  }
 
  return true;
}
 
bool KalsOgreFrame::mouseReleased(
  const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
  return true;
}