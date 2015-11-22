#pragma once
#include "OHeaders.h"
#include "KalsFrameListener.h"
#include "PlayerCh.h"

/*
기본적 Ogre 객체들을 초기화하고 
전체적인 루프를 담당하는 클래스
*/
class KalsOgreFrame
{
public:
	KalsOgreFrame();
	virtual ~KalsOgreFrame()
	{
		
		if(m_pFrameListener)
		{
			delete m_pFrameListener;
		}
		if(m_pRoot)
		{
			OGRE_DELETE m_pRoot;
		}
	}

	/// Start the example
	virtual void go(void)
	{
		if (!setup())
			return;

		m_pRoot->startRendering();

		// clean up
		destroyScene();
	}

	

protected:
	
	Ogre::Root *m_pRoot;
	Ogre::Camera* m_pCamera;
	Ogre::SceneManager* m_pSceneMgr;
	KalsFrameListener* m_pFrameListener;
	Ogre::RenderWindow* m_pWindow;
	Ogre::String m_pResourcePath;
	Ogre::String m_pResourcesCfg;
    Ogre::String m_pPluginsCfg;

	// These internal methods package up the stages in the startup process
	/** Sets up the application - returns false if the user chooses to abandon configuration. */
	virtual bool setup(void);
	/** Configures the application - returns false if the user chooses to abandon configuration. */
	virtual bool configure(void);

	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);

	virtual void createScene(void) = 0;    // pure virtual - this has to be overridden

	virtual void destroyScene(void);    // Optional to override this

	virtual void createViewports(void);

	/// Method which will define the source of resources (other than current folder)
	virtual void setupResources(void);

	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	virtual void createResourceListener(void);

	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	virtual void loadResources(void);

};