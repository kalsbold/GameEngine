#pragma once
#include "OHeaders.h"

#include "PlayerCh.h"

/*
�⺻�� Ogre ��ü���� �ʱ�ȭ�ϰ� 
��ü���� ������ ����ϴ� Ŭ����
*/
class KalsOgreFrame : 
	public Ogre::WindowEventListener,
	public OIS::KeyListener,
	public OIS::MouseListener,
	public Ogre::FrameListener
{
public:
	KalsOgreFrame();
	~KalsOgreFrame();

	bool Run();

private:
	bool InitOgre();
	bool InitResource();
	void CreateCamera();

	void CreateScene();

	void InitOIS();

	bool frameRenderingQueued(const Ogre::FrameEvent& fe);
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);
	Ogre::Root* m_pRoot;
	Ogre::String m_pResourcesCfg;
	Ogre::String m_pPluginsCfg;

	Ogre::RenderWindow* m_pWindow;
	Ogre::SceneManager* m_pSceneMgr;
	Ogre::Camera* m_pCamera;

	OIS::InputManager* m_pInputMgr;
	OIS::Keyboard* m_pKeyboard;
	OIS::Mouse* m_pMouse;

	PlayerCh * m_pPlayer;

	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::SceneNode* m_pCamNode;
	Ogre::Vector3 mDirection;

	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);
 
	virtual bool mouseMoved(const OIS::MouseEvent& me);
	virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

};