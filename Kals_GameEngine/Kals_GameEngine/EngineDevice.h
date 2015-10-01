#pragma once
#include "KalsOgre.h"
class EngineDevice
	: public Ogre::WindowEventListener,
	public Ogre::FrameListener, 
	public KalsInputManager,
	OgreBites::SdkTrayListener
{
public:
	EngineDevice(void);
	virtual ~EngineDevice(void);

	bool go();

private:
	//�� ����.
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

	bool processUnbufferedInput(const Ogre::FrameEvent& fe);

	//����â�� ũ�� ����.
	virtual void windowResized(Ogre::RenderWindow* rw);
	//����â ����.�Է���ġ ����
	virtual void windowClosed(Ogre::RenderWindow* rw);

	//���α׷��� �ʱ� ������ ����.
	virtual bool setup();

	virtual bool configure(void);
	//������ SceneManager�� �����ϰ� OverlaySystem�� �ʱ�ȭ �Ѵ�.
	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);
	virtual void createScene(void); // Override me!
	virtual void destroyScene(void);
	virtual void createViewports(void);
	virtual void setupResources(void);
	virtual void createResourceListener(void);
	virtual void loadResources(void);
	virtual void createLight(void);

	//����ǥ��.
	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain* terrain);
	void configureTerrainDefaults(Ogre::Light* light);

	//Ű�Էºη� �ű��.
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);


	Ogre::Root*                 mRoot;
	Ogre::Camera*               mCamera;
	Ogre::SceneManager*         mSceneMgr;
	Ogre::RenderWindow*         mWindow;
	Ogre::String                mResourcesCfg;
	Ogre::String                mPluginsCfg;

	Ogre::OverlaySystem*        mOverlaySystem;


	bool mTerrainsImported;
	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::TerrainGlobalOptions* mTerrainGlobals;

	OgreBites::InputContext     mInputContext;
	OgreBites::Label* mInfoLabel;
	OgreBites::SdkTrayManager*	mTrayMgr;
	CameraMan*    mCameraMan;     	// Basic camera controller
	OgreBites::ParamsPanel*     mDetailsPanel;   	// Sample details panel
	bool                        mCursorWasVisible;	// Was cursor visible before dialog appeared?
	bool                        mShutDown;

	OIS::InputManager* mInputMgr;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;

	Ogre::Light* Light;

	Ogre::String                 m_ResourcePath;

	Mesh *Ninja;
	Mesh *Penguin;
	//Mesh *Penguin1;
	//Mesh *Penguin2;
	//Mesh *Penguin3;
	
};

