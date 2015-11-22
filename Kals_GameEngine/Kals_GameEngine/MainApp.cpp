#include "MainApp.h"


void MainApp::createCamera(void)
{
	// create camera, but leave at default position
	m_pCamera = m_pSceneMgr->createCamera("PlayerCam"); 
	m_pCamera->setNearClipDistance(5);
}

void MainApp::createScene(void)
{
	m_pSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));

	// add the ninja
	Entity *ent = m_pSceneMgr->createEntity("Ninja", "ninja.mesh");
	SceneNode *node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
	node->attachObject(ent);

	// create the light
	Light *light = m_pSceneMgr->createLight("Light1");
	light->setType(Light::LT_POINT);
	light->setPosition(Vector3(250, 150, 250));
	light->setDiffuseColour(ColourValue::White);
	light->setSpecularColour(ColourValue::White);

	// Create the scene node
	node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Vector3(-400, 200, 400));

	// Make it look towards the ninja
	node->yaw(Degree(-45));

	// Create the pitch node
	node = node->createChildSceneNode("PitchNode1");
	node->attachObject(m_pCamera);

	// create the second camera node/pitch node
	node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Vector3(0, 200, 400));
	node = node->createChildSceneNode("PitchNode2");
}

void MainApp::createFrameListener(void)
{
	// Create the FrameListener
	m_pFrameListener = new MainListener(m_pWindow, m_pCamera, m_pSceneMgr);
	m_pRoot->addFrameListener(m_pFrameListener);

}