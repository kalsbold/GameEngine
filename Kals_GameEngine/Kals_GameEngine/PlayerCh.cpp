#include "PlayerCh.h"


PlayerCh::PlayerCh(Ogre::SceneManager* m_pSM)
	:
	m_pCam(0),
    m_pCamNode(0),
	m_pSceneMgr(m_pSM)
{
	createChScene();
}


PlayerCh::~PlayerCh(void)
{
}


void PlayerCh::createChScene()
{
  m_pSceneMgr->setAmbientLight(Ogre::ColourValue(.2, .2, .2));
 
  Ogre::Entity* tudorEntity = m_pSceneMgr->createEntity("Ninja.mesh");
  Ogre::SceneNode* node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(
    "NinjaNode");
  node->attachObject(tudorEntity);
 
  Ogre::Light* light = m_pSceneMgr->createLight("Light1");
  light->setType(Ogre::Light::LT_POINT);
  light->setPosition(Ogre::Vector3(300, 150, 0));
  light->setDiffuseColour(Ogre::ColourValue::White);
  light->setSpecularColour(Ogre::ColourValue::White);
 
  m_pCam = m_pSceneMgr->createCamera("PlayerCam");

  node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(
    "CamNode1", Ogre::Vector3(1200, -370, 0));
  node->yaw(Ogre::Degree(90));
 
  m_pCamNode = node;
  node->attachObject(m_pCam);
 
  node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(
    "CamNode2", Ogre::Vector3(-500, -370, 1000));
  node->yaw(Ogre::Degree(-30));
 
}
 
void PlayerCh::MoveCh(const Ogre::FrameEvent& fe,Ogre::Vector3 & m_pV3)
{
  m_pCamNode->translate(m_pV3 * fe.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
}
 