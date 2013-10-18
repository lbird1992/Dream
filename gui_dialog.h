#ifndef _DREAM_GUIDIALOG_
#define _DREAM_GUIDIALOG_

#include "gui_base.h"

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

  virtual void EventMouseLeftHolding();
  virtual void EventMouseLeftUp() {}

protected:


};

#endif