#include "GUIManager.h"


GUIManager::GUIManager(void)
{
	InitCEGUI();
}


GUIManager::~GUIManager(void)
{
}

void GUIManager::InitCEGUI()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing CEGUI ***");

	m_pRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();

	// add resource groups that we use
	rgm.createResourceGroup("imagesets");
	rgm.createResourceGroup("fonts");
	rgm.createResourceGroup("layouts");
	rgm.createResourceGroup("schemes");
	rgm.createResourceGroup("looknfeels");
	rgm.createResourceGroup("lua_scripts");
	rgm.createResourceGroup("schemas");

	// add CEGUI sample framework datafile dirs as resource locations
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem");

	// for each resource type, set a resource group directory
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("G:/cegui-0.8.4/cegui-0.8.4/datafiles/schemes", "FileSystem", "schemes");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("G:/cegui-0.8.4/cegui-0.8.4/datafiles/imagesets", "FileSystem", "imagesets");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("G:/cegui-0.8.4/cegui-0.8.4/datafiles/fonts", "FileSystem", "fonts");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("G:/cegui-0.8.4/cegui-0.8.4/datafiles/layouts", "FileSystem", "layouts");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("G:/cegui-0.8.4/cegui-0.8.4/datafiles/looknfeel", "FileSystem", "looknfeels");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("G:/cegui-0.8.4/cegui-0.8.4/datafiles/xml_schemas", "FileSystem", "schemas");

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::AnimationManager::setDefaultResourceGroup("animations");


	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	Ogre::LogManager::getSingletonPtr()->logMessage("Finished");
}

void GUIManager::createGUIScene()
{
	//InitCEGUI();
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	quit->setText("Quit");
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

	sheet->addChild(quit);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

	quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::quit, this));

	//CEGUI::Window *si = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage", "RTTWindow");
	////si->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0),
	//si->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0),
	//	CEGUI::UDim(0.4f, 0)));
	//si->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0),
	//	CEGUI::UDim(0.0f, 0)));

	//si->setProperty("Image", "RTTImage");
	//sheet->addChild(si);
}

// Ogre::FrameListener
bool GUIManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	return true;
}

// OIS::KeyListener
bool GUIManager::keyPressed( const OIS::KeyEvent &arg )
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown((CEGUI::Key::Scan)arg.key);
	context.injectChar((CEGUI::Key::Scan)arg.text);
	return true;
}
bool GUIManager::keyReleased( const OIS::KeyEvent &arg )
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
	return true;
}
// OIS::MouseListener
bool GUIManager::mouseMoved( const OIS::MouseEvent &arg )
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys .getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
		sys .getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}
bool GUIManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton() .getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	return true;
}
bool GUIManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton() .getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

bool GUIManager::quit(const CEGUI::EventArgs &e)
{
	shutdown = true;
	return true;
}

bool GUIManager::Exit()
{
	if(shutdown != true)return false;
	return true;
}

CEGUI::MouseButton GUIManager::convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;

	case OIS::MB_Right:
		return CEGUI::RightButton;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;

	default:
		return CEGUI::LeftButton;
	}
}