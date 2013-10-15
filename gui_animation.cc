#include "gui_animation.h"

GUIAnimation::GUIAnimation( const char* filename, const Coordinate position, const GUI* father,
                            const AnimationType animation_type) : GUI(filename, position, father) {
  animation_type_ = animation_type;
  is_end_ = false;
  frame_ = frame_fill_ = 0;
  direction_ = 0;
  is_draw_ = true;
  type_ = GUI_ANIMATION;
}

void GUIAnimation::Render( const Coordinate position) {
  if( is_draw_ == false)
    return;
  if( is_end_)
    return;
  if( texture_.GetDirectionCount() == 0)
    return;
  Coordinate position_to_draw = position + coordinate_;
  texture_.Render( position_to_draw, frame_, direction_);
  rect_drawed_.top = static_cast<int>(position_to_draw.GetY());
  rect_drawed_.left = static_cast<int>(position_to_draw.GetX());
  rect_drawed_.bottom = static_cast<int>(position_to_draw.GetY() + texture_.GetHeight( 0, 0));
  rect_drawed_.right = static_cast<int>(position_to_draw.GetX() + texture_.GetWidth( 0, 0));
  frame_drawed_ = 0;
  direction_drawed_ = 0;
  position_drawed_ = position_to_draw;

  ++frame_fill_;
  if( frame_fill_ >= kFrameFill) {
    frame_fill_ = 0;
    ++frame_;
    if( frame_ >= texture_.GetFrameCount() - 1) {
      if( animation_type_ == ONCE)
        is_end_ = true;
      else
        frame_ = 0;
    }
      
  }
}