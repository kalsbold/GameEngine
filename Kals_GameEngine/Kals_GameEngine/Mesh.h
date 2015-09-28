#pragma once
static Ogre::Real MOVE = 500;
class Mesh
{

public:
	Mesh(void);
	Mesh(const Ogre::String & meshName,const Ogre::Vector3 &Position);
	~Mesh(void);

	void Render(Ogre::SceneManager * sc);

	void Move(const OIS::Keyboard *key, const OIS::Mouse * mouse);

private:
	Ogre::Entity * mesh;
	Ogre::SceneNode * meshNode;
	Ogre::Vector3 dirVec;
	Ogre::Camera * cam;
	Ogre::String MeshName;
	Ogre::Vector3 Position;

};

