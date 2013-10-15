#ifndef _DREAM_GUIMANAGE_
#define _DREAM_GUIMANAGE_

#include "gui_base.h"
#include "gui_animation.h"

class GUIManage {
public:

  void Init();

  void Render() const;
  void Control();

  void UpdateMousePosition() {
    mouse_position_previous_ = mouse_position_;
    HGE* hge = hgeCreate( HGE_VERSION);
    float mouse_position_x, mouse_position_y;
    hge->Input_GetMousePos( &mouse_position_x, &mouse_position_y);
    mouse_position_ = Coordinate( mouse_position_x, mouse_position_y);
  }

  void AddAnimation( GUIAnimation* animation);
  void KillAnimation();

  GUI* GetMouseOn() const;
  GUI* GetMouseOnInNoTextureDialog( const GUI* dialog) const;
  GUI* GetMouseOnInDialogWithTexture( const GUI* dialog) const;

  inline GUI* GetRoot() {
    return root_gui_pointer_;
  }
  inline Coordinate GetMousePosition() {
    return mouse_position_;
  }
  inline Coordinate GetMousePositionPrevious() {
    return mouse_position_previous_;
  }
  

private:
  std::list<GUIAnimation*> animations_;
  GUI* root_gui_pointer_;
  GUI* mouse_on_;
  GUI* mouse_on_previous_;
  Coordinate mouse_position_;
  Coordinate mouse_position_previous_;
};

extern GUIManage* g_gui_manage;

#endif