#pragma once
#include "OHeaders.h"


class InputListener
	:
	public Ogre::FrameListener,
	public OIS::KeyListener,
	public OIS::MouseListener
{
public:
	InputListener(void);
	virtual ~InputListener(void);

	// Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) =0;
 
    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg ) =0;
    virtual bool keyReleased( const OIS::KeyEvent &arg )=0;
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg )=0;
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )=0;
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )=0;
};

