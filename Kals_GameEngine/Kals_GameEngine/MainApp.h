#pragma once
#include "OHeaders.h"
#include "GUIManager.h"
using namespace Ogre;

class MainApp 
	:
	public WindowEventListener,
	public FrameListener,
	public OIS::KeyListener,
	public OIS::MouseListener
{
public:
	MainApp(void);
	~MainApp(void);

	bool Run();

private:
	bool initOgre();

	void createFrameListener();

	void createCamera();

	void createScene();

	bool frameRenderingQueued(const FrameEvent& fe);

	void windowResized(RenderWindow* rw);

	void windowClosed(RenderWindow* rw);


	// OIS::KeyListener
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );



	Root* m_pRoot;
	String mResourcesCfg;
	String mPluginsCfg;
	RenderWindow* m_pWindow;
	SceneManager* m_pSceneMgr;
	Camera* m_pCamera;

	OIS::InputManager* m_pInputMgr;
	OIS::Keyboard* m_pKeyboard;
	OIS::Mouse* m_pMouse;


	GUIManager * m_pUIMgr;
};

