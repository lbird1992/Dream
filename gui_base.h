//
//The GUI base class.
//All of the GUI rendered on the screen should inherit from this.
//     Code by L.Bird @ 2013-10-7
//
#ifndef _DREAM_GUIBASE_H_
#define _DREAM_GUIBASE_H_

#include <list>
#include "hge_include.h"
#include "resource_manage.h"

class GUI {
public:
  inline int GetID() {
    return id_;
  }
  inline Coordinate GetCoordinate() {
    return coordinate_;
  }
  inline std::list GetSonGuiList() {
    return son_gui_list_;
  }

  virtual void Render( Coordinate position);

protected:

private:
  int id_;
  bool is_draw_;
  Coordinate coordinate_;
  Texture texture_;
  std::list<GUI*> son_gui_list_;
};

#endif //_DREAM_GUIBASE_H_