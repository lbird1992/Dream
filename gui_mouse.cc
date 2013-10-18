#include "gui_mouse.h"
#include "gui_manage.h"

GUIMouse* g_gui_mouse;

GUIMouse::GUIMouse() {
  texture_ = g_resource_manage->GetTex( "image\\other\\cursor.was");
  frame_ = frame_2_ = 0;
}

void GUIMouse::Render() {
  texture_.Render( g_gui_manage->GetMousePosition(), frame_, D_ONLY);
  ++frame_2_;
  if( frame_2_ >= kMouseFrameFill) {
    ++frame_;
    frame_2_ = 0;
    if( frame_ >= texture_.GetFrameCount() ) {
      frame_ = 0;
    }
  }
}

void GUIMouse::ResetFile( const char* filename) {
  texture_ = g_resource_manage->GetTex( filename);
  frame_ = frame_2_ = 0;
}