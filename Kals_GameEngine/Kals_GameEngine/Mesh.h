#pragma once

class Mesh
{

public:
	Mesh(void);
	Mesh(const Ogre::String & meshName,const Ogre::Vector3 &Position);
	~Mesh(void);

	void Render(Ogre::SceneManager * sc);

	void Move(const OIS::Keyboard *key, const OIS::Mouse * mouse,const Ogre::FrameEvent& fe);
	void Move();
private:
	Ogre::Entity * mesh;
	Ogre::SceneNode * meshNode;
	
	
	Ogre::String MeshName;
	Ogre::Vector3 Position;

};

