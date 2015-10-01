#pragma once
/*
순수 가상 클래스, 
필요한 각 객체에서 상속받아 구현하면 됨.
각종 키입력을 위한 KeyListener와
마우스 입력을 위한 MouseListener 를 상속받아.
입력받는 함수를 처리함.
*/
class KalsInputManager
	:public OIS::KeyListener, 
	public OIS::MouseListener
{
public:
	KalsInputManager(void);
	virtual ~KalsInputManager(void);

	virtual bool keyPressed(const OIS::KeyEvent &arg) =0;
	virtual bool keyReleased(const OIS::KeyEvent &arg) =0;
	virtual bool mouseMoved(const OIS::MouseEvent &arg) =0;
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) =0;
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) =0;
};

