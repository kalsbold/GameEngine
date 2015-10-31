//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef MAIN_APP_H
#define MAIN_APP_H
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <Ogre/Renderer.h>

//#include "OgreFramework.h"
#include "Dotsceneloader.h"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class ExitListener : public Ogre::FrameListener
{
public:
    ExitListener(OIS::Keyboard *keyboard)
        : mKeyboard(keyboard)
    {
    }
 
    bool frameStarted(const Ogre::FrameEvent& evt)
    {
        mKeyboard->capture();
        return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
    }
 
private:
    OIS::Keyboard *mKeyboard;
};

class MainApp 
{
public:
    void go()
    {
        createRoot();
        defineResources();
        setupRenderSystem();
        createRenderWindow();
        initializeResourceGroups();
        setupScene();
        setupInputSystem();
        setupCEGUI();
        createFrameListener();
        startRenderLoop();
    }
 
    ~MainApp()
    {
    }
 
private:
    Ogre::Root *mRoot;
    OIS::Keyboard *mKeyboard;
    OIS::InputManager *mInputManager;
    CEGUI::OgreRenderer *mRenderer;
    CEGUI::System *mSystem;
    ExitListener *mListener;
 
    void createRoot();
     
    void defineResources();
   
    void setupRenderSystem();

    void createRenderWindow();  
 
    void initializeResourceGroups();
    
     void setupScene();
 
    void setupInputSystem();

    void setupCEGUI();
   
    void createFrameListener();

    void startRenderLoop();
     
     
};
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||