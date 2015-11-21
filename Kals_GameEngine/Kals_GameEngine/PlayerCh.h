#pragma once
#include "OHeaders.h"
class PlayerCh 
{
public:
	PlayerCh(Ogre::SceneManager* m_pSM);
	~PlayerCh(void);

	void MoveCh(const Ogre::FrameEvent& fe,Ogre::Vector3 & m_pV3);

	Ogre::Camera * getCam(){return m_pCam;}
	Ogre::SceneNode * getCamNode(){return m_pCamNode;}
	void SetCamNode(Ogre::SceneNode * SN){m_pCamNode = SN;}
private:
	Ogre::SceneManager* m_pSceneMgr;

	Ogre::Camera * m_pCam;
	Ogre::SceneNode* m_pCamNode;
	

	void createChScene();
	
	
};

