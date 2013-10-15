#ifndef _DREAM_GUIBUTTON_H_
#define _DREAM_GUIBUTTON_H_

#include "gui_base.h"
#include "hge_include.h"
#include "resource_manage.h"


enum ButtonState{//按钮状态
		BS_Normal = 1,
		BS_MouseOn,
		BS_MouseClick
	};

class GUIButton:public GUI{	
    
public:
  GUIButton(  const char* filename,const Coordinate position,const GUI*father,void (*pclick_func)(),
    int normal_frame = 0, int mouse_on_frame = 2, int mouse_click_frame = 1);
  virtual ~GUIButton() {};
  virtual void Render( const Coordinate position);
  virtual void EventMouseLeftDown();
  virtual void EventMouseLeftUp();
  virtual void EventMouseGet();
  virtual void EventMouseLeave();
protected:  
  	int normal_frame_;
		int mouse_on_frame_;
		int mouse_click_frame_;

    ButtonState state_;
		void (*click_func_)();//鼠标左键在按钮上松开时，要调用的函数

};

#endif //_DREAM_GUIBUTTON_H_