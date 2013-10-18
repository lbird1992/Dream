#ifndef _DREAM_GUIBEGIN_
#define _DREAM_GUIBEGIN_

#include "gui_manage.h"

class GUIBegin : public GUIDialog {
public:
  GUIBegin();
  virtual ~GUIBegin() {}

  void Init();

  virtual void EventMouseLeftHolding() {}
  virtual void EventMouseRightClick() {}

  static void ClickOnButtonBegin();
  static void ClickOnButtonExit();
protected:
};

#endif