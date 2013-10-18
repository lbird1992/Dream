#pragma once
#ifndef _DREAM_SHOWTEXT_
#define _DREAM_SHOWTEXT_

#include <string>
#include "gui_base.h"
#include "GfxFont.h"

const int kHintSpeed = 2;

struct ListElement{
  int x;//×Ö·û´®Í·×Ö·ûµÄºá×ø±ê
  std::string str;
};

class GUITextView : public GUI{
public:
  GUITextView(const GUI* father);
  virtual ~GUITextView();
  void Render(const Coordinate position);
  void AddString(std::string string);
private:
  GfxFont* font_;
  std::list<ListElement> string_list_;
};

extern GUITextView* g_gui_text_view;

#endif//_DREAM_SHOWTEXT_
