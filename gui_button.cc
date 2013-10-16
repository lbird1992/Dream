#include "gui_button.h"
#include "gui_base.h"
#include "hge_include.h"
#include "resource_manage.h"


GUIButton::GUIButton( const char* filename,const Coordinate position,const GUI* father, void (*plick_func)(),
    int normal_frame, int mouse_on_frame, int mouse_click_frame) : GUI(filename, position, father){
    normal_frame_ = normal_frame;
    mouse_on_frame_ = mouse_on_frame;
    mouse_click_frame_ = mouse_click_frame;
    state_ = BS_Normal;
    click_func_ = plick_func;
    type_ = GUI_BUTTON;
    is_draw_ = true;
}

void GUIButton::Render( const Coordinate position)
{
	int frame = normal_frame_;
	if( state_ == BS_MouseOn)
		frame = mouse_on_frame_;
	else if(state_ == BS_MouseClick)
		frame = mouse_click_frame_;
  if(texture_.GetDirectionCount() == 0)
    return;
  texture_.Render( position + coordinate_, frame, 0);
  rect_drawed_.top = static_cast<int>((position + coordinate_).GetY());
  rect_drawed_.bottom = static_cast<int>( (position + coordinate_).GetY() + texture_.GetHeight( frame, 0));
  rect_drawed_.left = static_cast<int>((position + coordinate_).GetX());
  rect_drawed_.right = static_cast<int>( (position + coordinate_).GetX() + texture_.GetWidth( frame, 0));
  frame_drawed_ = frame;
  direction_drawed_ = 0;
  position_drawed_ = position + coordinate_;
  //ResourceManage::GetWas(m_filename.c_str())->DrawWas( x+m_x, y+m_y, 0, m_id, frame); 
}

void GUIButton::EventMouseLeftDown(){
	state_ = BS_MouseClick;
}

void GUIButton::EventMouseLeftUp(){
	state_ = BS_MouseOn;
	(click_func_)();
}

void GUIButton::EventMouseLeave(){
  state_ = BS_Normal;
}

void GUIButton::EventMouseGet(){
  state_ = BS_MouseOn;
}