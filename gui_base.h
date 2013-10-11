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
  GUI( const char* filename, const Coordinate position,const GUI* father);
  virtual ~GUI();

  inline int GetID() const {
    return id_;
  }

  inline Coordinate GetCoordinate() const {
    return coordinate_;
  }

  inline std::list<GUI*> GetSonGuiList() const {
    return son_gui_list_;
  }

  inline RECT GetRectDrawed() const {
    return rect_drawed_;
  }

  inline void SetOnDraw() {
    is_draw_ = true;
  }

  inline void SetOffDraw() {
    is_draw_ = false;
  }

  inline GUI* GetFatherGUI() const {
    return father_gui_;
  }
  inline void SetFatherGUI(const GUI* father) {
    father_gui_ = const_cast<GUI*>(father);
  }

  virtual void Render( const Coordinate position);

  virtual void EventMouseLeftDown() { 
    father_gui_->EventMouseLeftDown();
  }
  virtual void EventMouseLeftHolding() {
    father_gui_->EventMouseLeftHolding();
  }
  virtual void EventMouseLeftUp() {
    father_gui_->EventMouseLeftUp();
  }
  virtual void EventMouseRightClick() {
    father_gui_->EventMouseRightClick();
  }

protected:
  int id_;
  bool is_draw_;
  Coordinate coordinate_;
  RECT rect_drawed_;
  Texture texture_;
  std::list<GUI*> son_gui_list_;
  GUI* father_gui_;
};

#endif //_DREAM_GUIBASE_H_