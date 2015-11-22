#ifndef __KalsFrameListener_H__
#define __KalsFrameListener_H__

#include "OHeaders.h"

using namespace Ogre;

class KalsFrameListener: public FrameListener, public WindowEventListener
{

public:
	// Constructor takes a RenderWindow because it uses that to determine input context
	KalsFrameListener(RenderWindow* win, Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false,
			     bool bufferedJoy = false );

	//Adjust mouse clipping area
	virtual void windowResized(RenderWindow* rw);

	//Unattach OIS before window shutdown (very important under Linux)
	virtual void windowClosed(RenderWindow* rw);

	virtual ~KalsFrameListener();

	virtual bool processUnbufferedKeyInput(const FrameEvent& evt);
	virtual bool processUnbufferedMouseInput(const FrameEvent& evt);

	virtual void moveCamera();

	// Override frameRenderingQueued event to process that (don't care about frameEnded)
	bool frameRenderingQueued(const FrameEvent& evt);


protected:
	Camera* mCamera;

	Vector3 mTranslateVector;
	Real mCurrentSpeed;
	RenderWindow* mWindow;
	bool mStatsOn;

	std::string mDebugText;

	unsigned int mNumScreenShots;
	float mMoveScale;
	float mSpeedLimit;
	Degree mRotScale;
	// just to stop toggles flipping too fast
	Real mTimeUntilNextToggle ;
	Radian mRotX, mRotY;
	TextureFilterOptions mFiltering;
	int mAniso;

	int mSceneDetailIndex ;
	Real mMoveSpeed;
	Degree mRotateSpeed;


	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;
};

#endif