#include "KalsOgre.h"


CameraMan::CameraMan(Ogre::Camera* cam)
	:SdkCameraMan(cam)
{
}

CameraMan::~CameraMan(void)
{
}


void CameraMan::injectKeyDown(const OIS::KeyEvent& evt)
{
	if (mStyle == OgreBites::CS_FREELOOK)
	{
		if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) mGoingForward = true;
		else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) mGoingBack = true;
		else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) mGoingLeft = true;
		else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) mGoingRight = true;
		else if (evt.key == OIS::KC_PGUP) mGoingUp = true;
		else if (evt.key == OIS::KC_PGDOWN) mGoingDown = true;
		else if (evt.key == OIS::KC_LSHIFT) mFastMove = true;
	}
}

void CameraMan::injectKeyUp(const OIS::KeyEvent& evt)
{
	if (mStyle == OgreBites::CS_FREELOOK)
	{
		if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) mGoingForward = false;
		else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) mGoingBack = false;
		else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) mGoingLeft = false;
		else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) mGoingRight = false;
		else if (evt.key == OIS::KC_PGUP) mGoingUp = false;
		else if (evt.key == OIS::KC_PGDOWN) mGoingDown = false;
		else if (evt.key == OIS::KC_LSHIFT) mFastMove = false;
	}
}