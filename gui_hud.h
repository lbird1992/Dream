#ifndef _DREAM_GUIHUD_
#define _DREAM_GUIHUD_

#include "gui_dialog.h"

class GUIHud : public GUIDialog {
public:
  GUIHud();
  void Init();
  virtual void Render( const Coordinate position);
  virtual void EventMouseLeftHolding() {}

  static void ClickOnAttack();
  static void ClickOnWupin();
  static void ClickOnGive();
  static void ClickOnBargin();
  static void ClickOnTeam();
  static void ClickOnPet();
  static void ClickOnRenwu();
  static void ClickOnBangpai();
  static void ClickOnFashukuaijie();
  static void ClickOnFriend();
  static void ClickOnAction();
  static void ClickOnSystem();
  static void ClickOnPlayerHead();
protected:
};

#endif