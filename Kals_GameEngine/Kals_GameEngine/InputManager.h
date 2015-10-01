#pragma once
/*
���� ���� Ŭ����, 
�ʿ��� �� ��ü���� ��ӹ޾� �����ϸ� ��.
���� Ű�Է��� ���� KeyListener��
���콺 �Է��� ���� MouseListener �� ��ӹ޾�.
�Է¹޴� �Լ��� ó����.
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

