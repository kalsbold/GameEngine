#include "KalsOgre.h"



Mesh::Mesh(void)
{
}

Mesh::Mesh(const Ogre::String &meshName,const Ogre::Vector3 &Position )
	:MeshName(meshName),Position(Position),dirVec(0,0,0)
{
}

Mesh::~Mesh(void)
{
}

void Mesh::Render(Ogre::SceneManager * sc)
{
	Position += dirVec;
	mesh = sc->createEntity(MeshName);
	
	mesh->setCastShadows(true);

	meshNode = sc->getRootSceneNode()->createChildSceneNode(
    MeshName);
    meshNode->attachObject(mesh);
	meshNode->setPosition(Position);

}

void Mesh::Move(const OIS::Keyboard *key, const OIS::Mouse * mouse)
{
	if(key->isKeyDown(OIS::KC_W))
	{
		dirVec.z -= MOVE;
	}

	if(key->isKeyDown(OIS::KC_S))
	{
		dirVec.z += MOVE;
	}

	if(key->isKeyDown(OIS::KC_A))
	{
		dirVec.y -= MOVE;
	}

	if(key->isKeyDown(OIS::KC_D))
	{
		dirVec.y += MOVE;
	}

	
}