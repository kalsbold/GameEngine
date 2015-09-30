#pragma once
#include <SdkCameraMan.h>
class CameraMan :
	public OgreBites::SdkCameraMan
{
public:
	CameraMan();
	~CameraMan(void);
	CameraMan(Ogre::Camera* cam);

	void injectKeyDown(const OIS::KeyEvent& evt);
	void injectKeyUp(const OIS::KeyEvent& evt);

private:

};

