﻿#include "gui_base.h"

GUI::GUI( const char* filename, const Coordinate position,const GUI* father) {
  //TODO: 加入ID项的初始化
  type_ = GUI_BASE;
  coordinate_ = position;
  father_gui_ = const_cast<GUI*>(father);
  if( filename != NULL)
    texture_ = g_resource_manage->GetTex( filename);
  is_draw_ = false;
}

GUI::~GUI() {
  //删除所有子节点
  for( std::list<GUI*>::iterator iter = son_gui_list_.begin(); iter != son_gui_list_.end(); ++iter)
    delete (*iter);
  texture_.Release();
}

void GUI::Render( const Coordinate position) {
  if( is_draw_ == false)
    return;
  //显示自身纹理
  Coordinate position_to_draw = position + coordinate_;
  if( texture_.GetDirectionCount() != 0) {
    texture_.Render( position_to_draw, 0, 0);
    rect_drawed_.top = static_cast<int>(position_to_draw.GetY());
    rect_drawed_.left = static_cast<int>(position_to_draw.GetX());
    rect_drawed_.bottom = static_cast<int>(position_to_draw.GetY() + texture_.GetHeight( 0, 0));
    rect_drawed_.right = static_cast<int>(position_to_draw.GetX() + texture_.GetWidth( 0, 0));
    frame_drawed_ = 0;
    direction_drawed_ = 0;
    position_drawed_ = Coordinate( static_cast<float>(rect_drawed_.left), 
                                  static_cast<float>(rect_drawed_.top) );
  }
  else {
    frame_drawed_ = -1;
    direction_drawed_ = -1;
    rect_drawed_.top = 0;
    rect_drawed_.left = 0;
    rect_drawed_.bottom = 480;
    rect_drawed_.right = 640;
    position_drawed_ = Coordinate( 0, 0);
  }
  //调用所有子节点的显示函数
  for( std::list<GUI*>::iterator iter = son_gui_list_.begin(); iter != son_gui_list_.end(); ++iter) {
    (*iter)->Render( position_to_draw);
  }
}

uint32_t GUI::GetPixel( const Coordinate position) const {
  if( texture_.GetDirectionCount() == 0)
    return 0;
  return texture_.GetColor( frame_drawed_, direction_drawed_, position - position_drawed_);
}