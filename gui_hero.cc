#include "gui_hero.h"

GUIHero::GUIHero( const std::string directory_path, const Coordinate position, const GUI* father) :
            GUIAnimation( (directory_path + "\\1.was").c_str(), position, father, LOOP) {
  directory_path_ = directory_path;
  action_ = 1;
}

void GUIHero::AddAnimation( const int action, const int direction, const bool is_loop)
{
  if( action_ == action && direction_ == direction)
    return;
  if( action_ == action && direction_ != direction) {
    direction_ = direction;
    return;
  }
  if( action_ == 1) {
    frame_second_ = frame_;
    frame_fill_second_ = frame_fill_;
    texture_second_ = texture_;
  }
  action_ = action;

  frame_ = 0;
  frame_fill_ = 0;
  GUIAnimation::SetDirection( direction);
  if( is_loop == false)
    animation_type_ = ONCE;
  char filename[256];
  if( action <= 2)
    sprintf( filename, "%s\\%d.was", directory_path_.c_str(), action);
  else {} //TODO:根据武器计算文件名
  texture_ = g_resource_manage->GetTex( filename);
}

void GUIHero::EndAnimation(){
  if( action_ == 1)
    return;
  animation_type_ = LOOP;
  action_ = 1;
  frame_ = frame_second_;
  frame_fill_ = frame_fill_second_;
  texture_ = texture_second_;
}

void GUIHero::Render( const Coordinate position) {
  GUIAnimation::Render(position);
  if( is_end_ == true) {
    is_end_ = false;
    action_ = 1;
    animation_type_ = LOOP;
    frame_ = frame_second_;
    frame_fill_ = frame_fill_second_;
    texture_ = texture_second_;
  }
}