#ifndef _DREAM_GUIDIALOG_
#define _DREAM_GUIDIALOG_

#include "gui_base.h"
#include "gui_manage.h"

class GUIDialog : public GUI {
public:
  GUIDialog( const char* filename, const Coordinate position);
  virtual ~GUIDialog();

  virtual void EventMouseRightClick() {
    if( is_draw_)
      is_draw_ = false;
  }

  virtual void EventMouseLeftDown() {
    father_gui_->DeleteSon( this);
    father_gui_->AddSon( this);
  }

  virtual void EventMouseLeftHolding() {
    Coordinate mouse_position = g_gui_manage->GetMousePosition();
    Coordinate mouse_position_previous = g_gui_manage->GetMousePositionPrevious();
    coordinate_.Plus( mouse_position - mouse_position_previous);
  }

protected:


};

#endif