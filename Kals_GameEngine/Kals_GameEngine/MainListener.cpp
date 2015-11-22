#include "MainListener.h"


MainListener::MainListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr)
	: KalsFrameListener(win, cam, true, true)
{
	// Populate the camera and scene manager containers
	mCamNode = cam->getParentSceneNode();
	mSceneMgr = sceneMgr;

	// set the rotation and move speed
	mRotate = 0.13;
	mMove = 250;

	// continue rendering
	mContinue = true;

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	mDirection = Vector3::ZERO;

}

bool MainListener::frameStarted(const FrameEvent &evt)
{
	if(mMouse)
		mMouse->capture();
	if(mKeyboard) 
		mKeyboard->capture();
	mCamNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);

	return mContinue;
}

// MouseListener
bool MainListener::mouseMoved(const OIS::MouseEvent &e) 
{ 
	if (e.state.buttonDown(OIS::MB_Right))
	{
		mCamNode->yaw(Degree(-mRotate * e.state.X.rel), Node::TS_WORLD);
		mCamNode->pitch(Degree(-mRotate * e.state.Y.rel), Node::TS_LOCAL);
	}

	return true; 
}
bool MainListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) 
{ 
	Light *light = mSceneMgr->getLight("Light1");
	switch (id)
	{
	case OIS::MB_Left:
		light->setVisible(! light->isVisible());
		break;
	default:
		break;
	}
	return true; 
}
bool MainListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) 
{ 
	return true; 
}

// KeyListener
bool MainListener::keyPressed(const OIS::KeyEvent &e)
{
	switch (e.key)
	{
	case OIS::KC_ESCAPE: 
		mContinue = false;
		break;
	case OIS::KC_1:
		mCamera->getParentSceneNode()->detachObject(mCamera);
		mCamNode = mSceneMgr->getSceneNode("CamNode1");
		mCamNode->attachObject(mCamera);
		break;

	case OIS::KC_2:
		mCamera->getParentSceneNode()->detachObject(mCamera);
		mCamNode = mSceneMgr->getSceneNode("CamNode2");
		mCamNode->attachObject(mCamera);
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
	return mContinue;
}

bool MainListener::keyReleased(const OIS::KeyEvent &e) { 
	switch (e.key)
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
	} // switch
	return true;
}
