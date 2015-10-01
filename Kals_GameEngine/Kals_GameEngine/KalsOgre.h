#ifndef KALSOGRE_H
#define KALSOGRE_H

#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreLogManager.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

//사용자 정의부.
#include "InputManager.h"
#include "Mesh.h"
#include "CameraMan.h"

static Ogre::Real rotate = .13;
static Ogre::Real move = 250;

#endif