#include "gui_begin.h"
#include "game_manage.h"

GUIBegin::GUIBegin() : GUIDialog( "image\\other\\back.jpg", Coordinate(0,0) ) {
}

void GUIBegin::Init() {
  GUIButton* begin_button = new GUIButton( "image\\button\\btnBegin.was", Coordinate( 200, 300),
                                          this, ClickOnButtonBegin);
  AddSon( begin_button);
  GUIButton* exit_button = new GUIButton( "image\\button\\btnExit.was", Coordinate( 200, 350),
                                          this, ClickOnButtonExit);
  AddSon( exit_button);
  SetOnDraw();
}

void GUIBegin::ClickOnButtonBegin() {
  g_game_manage->SetPlay();
}

void GUIBegin::ClickOnButtonExit() {
  g_game_manage->SetEnd();
}