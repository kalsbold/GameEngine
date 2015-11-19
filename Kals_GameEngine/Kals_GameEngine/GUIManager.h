#pragma once
#include "OHeaders.h"

#include <CEGUI/CEGUI.h>
#include <OGRE/Renderer.h>

#include "InputListener.h"



class GUIManager 
	:
	public InputListener
{
public:
	GUIManager(void);
	~GUIManager(void);

	bool Exit();

	// Ogre::FrameListener
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	CEGUI::OgreRenderer * GetRender(){return m_pRenderer;}
	void createGUIScene();

	void InitCEGUI();

	 // OIS::KeyListener
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

private:
	bool shutdown;

	CEGUI::OgreRenderer* m_pRenderer;

	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
	bool quit(const CEGUI::EventArgs &e);



};

