#include "MainApp.h"

MainApp::MainApp()
{
	m_pNinjaNode		= 0;
	m_pNinjaEntity		= 0;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
MainApp::~MainApp()
{
       
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MainApp::startDemo()
{
	m_pOgreFrame = new OgreFramework();
	if(!m_pOgreFrame->initOgre("DemoApp v1.0", this, 0))
		return;
 
	m_bShutdown = false;
 
	m_pOgreFrame->m_pLog->logMessage("Demo initialized!");
 
	setupDemoScene();
	runDemo();
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MainApp::setupDemoScene()
{

	//loader.parseDotScene("Samplescene.xml","General", m_pOgreFrame->m_pSceneMgr);
	

	m_pOgreFrame->m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
 
	m_pOgreFrame->m_pSceneMgr->createLight("Light")->setPosition(75, 75, 75);

	m_pNinjaEntity = m_pOgreFrame->m_pSceneMgr->createEntity("NinjaEntity", "Ninja.mesh");
	m_pNinjaNode = m_pOgreFrame->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
	m_pNinjaNode->attachObject(m_pNinjaEntity);
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MainApp::runDemo()
{
	m_pOgreFrame->m_pLog->logMessage("Start main loop...");
 
	double timeSinceLastFrame = 0;
	double startTime = 0;
 
      m_pOgreFrame->m_pRenderWnd->resetStatistics();
 
	while(!m_bShutdown && !m_pOgreFrame->isOgreToBeShutDown()) 
	{
		if(m_pOgreFrame->m_pRenderWnd->isClosed())m_bShutdown = true;
 
		Ogre::WindowEventUtilities::messagePump();
 
		if(m_pOgreFrame->m_pRenderWnd->isActive())
		{
			startTime = m_pOgreFrame->m_pTimer->getMillisecondsCPU();
 
			m_pOgreFrame->m_pKeyboard->capture();
			m_pOgreFrame->m_pMouse->capture();
 
			m_pOgreFrame->updateOgre(timeSinceLastFrame);
			m_pOgreFrame->m_pRoot->renderOneFrame();
 
			timeSinceLastFrame = m_pOgreFrame->m_pTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}
 
	m_pOgreFrame->m_pLog->logMessage("Main loop quit");
	m_pOgreFrame->m_pLog->logMessage("Shutdown OGRE...");
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MainApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	m_pOgreFrame->keyPressed(keyEventRef);
 
	if(m_pOgreFrame->m_pKeyboard->isKeyDown(OIS::KC_F))
	{
		 //do something
	}
 
	return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MainApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	m_pOgreFrame->keyReleased(keyEventRef);
 
	return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||