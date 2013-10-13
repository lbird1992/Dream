#ifndef _DREAM_GUIMANAGE_
#define _DREAM_GUIMANAGE_

#include "gui_base.h"

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

  GUI* GetMouseOn();
  

private:
  std::map<int, GUI*> gui_map_;
  GUI* root_gui_pointer_;
  GUI* mouse_on_;
  GUI* mouse_on_previous_;
  Coordinate mouse_position_;
  Coordinate mouse_position_previous_;
};

#endif