#include "KalsOgre.h"



Mesh::Mesh(void)
{
}

Mesh::Mesh(const Ogre::String &meshName,const Ogre::Vector3 &Position )
	:MeshName(meshName),Position(Position)
{
}

Mesh::~Mesh(void)
{
}

void Mesh::Render(Ogre::SceneManager * sc)
{

	mesh = sc->createEntity(MeshName);
	
	mesh->setCastShadows(true);

	meshNode = sc->getRootSceneNode()->createChildSceneNode(
    MeshName);
    
	meshNode->setPosition(Position);

	meshNode->attachObject(mesh);

	

}
/*
Unbuffered Input/
*/
void Mesh::Move(const OIS::Keyboard *key, const OIS::Mouse * mouse,const Ogre::FrameEvent& fe)
{

	 Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;
 
	 if (key->isKeyDown(OIS::KC_W) || key->isKeyDown(OIS::KC_UP))
    dirVec.z -= move;
 
  if (key->isKeyDown(OIS::KC_S)|| key->isKeyDown(OIS::KC_DOWN))
    dirVec.z += move;
 
  if (key->isKeyDown(OIS::KC_A)|| key->isKeyDown(OIS::KC_LEFT))
    dirVec.x -= move;
 
  if (key->isKeyDown(OIS::KC_D)|| key->isKeyDown(OIS::KC_RIGHT))
    dirVec.x += move;
 
  if (key->isKeyDown(OIS::KC_J))
  {
    if (key->isKeyDown(OIS::KC_LSHIFT))
      meshNode->yaw(Ogre::Degree(5 * rotate));
    else
      dirVec.x -= move;
  }
 
  if (key->isKeyDown(OIS::KC_L))
  {
    if (key->isKeyDown(OIS::KC_LSHIFT))
      meshNode->yaw(Ogre::Degree(-5 * rotate));
    else
      dirVec.x += move;
  }
 
  meshNode->translate(
    dirVec * fe.timeSinceLastFrame * 0.6,
    Ogre::Node::TS_LOCAL);

}

void Mesh::Move()
{

}


