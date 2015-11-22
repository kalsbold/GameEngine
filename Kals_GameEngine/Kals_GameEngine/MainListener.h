#pragma once
#include "KalsFrameListener.h"

class MainListener :
	public KalsFrameListener,
	public OIS::MouseListener,
	public OIS::KeyListener
{
public:
	MainListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr);

	bool frameStarted(const FrameEvent &evt);

	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

private:
	Real mRotate;          // The rotate constant
    Real mMove;            // The movement constant

    SceneManager *mSceneMgr;   // The current SceneManager
    SceneNode *mCamNode;   // The SceneNode the camera is currently attached to

    bool mContinue;        // Whether to continue rendering or not
    Vector3 mDirection;     // Value to move in the correct direction

};

