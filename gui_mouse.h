#ifndef _DREAM_GUIMOUSE_
#define _DREAM_GUIMOUSE_

#include "resource_manage.h"

const int kMouseFrameFill = 4;

class GUIMouse{
public:
  GUIMouse();
  ~GUIMouse();

  void Render();
  void ResetFile(const char* filename);

private:
  Texture texture_;
  int frame_;
  int frame_2_;
};

extern GUIMouse* g_gui_mouse;

#endif