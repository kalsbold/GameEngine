#include "KalsOgreFrame.h"

KalsOgreFrame::KalsOgreFrame()
	: m_pRoot(0),
	m_pResourcePath(Ogre::StringUtil::BLANK),
	m_pWindow(0),
	m_pSceneMgr(0),
	m_pCamera(0),
	m_pFrameListener(0),
	m_pResourcesCfg(Ogre::StringUtil::BLANK),
	m_pPluginsCfg(Ogre::StringUtil::BLANK)
{
}


bool KalsOgreFrame::setup(void)
{

#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
    m_pResourcesCfg = m_pResourcePath + "resources_d.cfg";
    m_pPluginsCfg = m_pResourcePath + "plugins_d.cfg";
#else
    m_pResourcesCfg = "resources_d.cfg";
    m_pPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
    m_pResourcesCfg = m_pResourcePath + "resources.cfg";
    m_pPluginsCfg = m_pResourcePath + "plugins.cfg";
#else
    m_pResourcesCfg = "resources.cfg";
    m_pPluginsCfg = "plugins.cfg";
#endif
#endif

	m_pRoot = OGRE_NEW Root(m_pPluginsCfg);

	setupResources();

	bool carryOn = configure();
	if (!carryOn) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	// Create the scene
	createScene();

	createFrameListener();

	return true;

}
/** Configures the application - returns false if the user chooses to abandon configuration. */
bool KalsOgreFrame::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(m_pRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		m_pWindow = m_pRoot->initialise(true);
		return true;
	}
	else
	{
		return false;
	}
}

void KalsOgreFrame::chooseSceneManager(void)
{
	// Create the SceneManager, in this case a generic one
	m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
}
void KalsOgreFrame::createCamera(void)
{
	// Create the camera
	m_pCamera = m_pSceneMgr->createCamera("PlayerCam");

	// Position it at 500 in Z direction
	m_pCamera->setPosition(Vector3(0,0,500));
	// Look back along -Z
	m_pCamera->lookAt(Vector3(0,0,-300));
	m_pCamera->setNearClipDistance(5);

}
void KalsOgreFrame::createFrameListener(void)
{
	m_pFrameListener= new KalsFrameListener(m_pWindow, m_pCamera);

	m_pRoot->addFrameListener(m_pFrameListener);
}



void KalsOgreFrame::destroyScene(void)
{
}    // Optional to override this

void KalsOgreFrame::createViewports(void)
{
	// Create one viewport, entire window
	Viewport* vp = m_pWindow->addViewport(m_pCamera);
	vp->setBackgroundColour(ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	m_pCamera->setAspectRatio(
		Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

/// Method which will define the source of resources (other than current folder)
void KalsOgreFrame::setupResources(void)
{
#ifdef _DEBUG
	// Load resource paths from config file
	ConfigFile cf;
	cf.load(m_pResourcesCfg);
#else
	// Load resource paths from config file
	ConfigFile cf;
	cf.load(m_pResourcesCfg);
#endif
	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app,
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location
			ResourceGroupManager::getSingleton().addResourceLocation(
				String(macBundlePath() + "/" + archName), typeName, secName);
#else
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
#endif
		}
	}
}

/// Optional override method where you can create resource listeners (e.g. for loading screens)
void KalsOgreFrame::createResourceListener(void)
{

}

/// Optional override method where you can perform resource group loading
/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
void KalsOgreFrame::loadResources(void)
{
	// Initialise, parse scripts etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

}