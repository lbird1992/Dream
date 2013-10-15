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

enum GuiType {
  NONE = 1,
  GUI_BASE,
  GUI_BUTTON,
  GUI_DIALOG,
  GUI_ANIMATION
};

class GUI {
public:
  GUI( const char* filename, const Coordinate position,const GUI* father);
  virtual ~GUI();

  //inline int GetID() const {
  //  return id_;
  //}

  inline Coordinate GetCoordinate() const {
    return coordinate_;
  }
  inline void SetCoordinate(const Coordinate coordinate) {
    coordinate_ = coordinate;
  }


  inline std::list<GUI*> GetSonGuiList() const {
    return son_gui_list_;
  }

  inline RECT GetRectDrawed() const {
    return rect_drawed_;
  }

  inline Texture GetTexture() const {
    return texture_;
  }

  inline bool IsDraw() {
    return is_draw_;
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

  uint32_t GetPixel( const Coordinate position) const;

  virtual void Render( const Coordinate position);

  virtual void EventMouseLeftDown() { 
    if( father_gui_ != NULL)
      father_gui_->EventMouseLeftDown();
  }
  virtual void EventMouseLeftHolding() {
    if( father_gui_ != NULL)  
      father_gui_->EventMouseLeftHolding();
  }
  virtual void EventMouseLeftUp() {
    if( father_gui_ != NULL)
      father_gui_->EventMouseLeftUp();
  }
  virtual void EventMouseRightClick() {
    if( father_gui_ != NULL)
      father_gui_->EventMouseRightClick();
  }
  virtual void EventMouseGet() {
    if( father_gui_ != NULL)
      father_gui_->EventMouseGet();
  }
  virtual void EventMouseLeave() {
    if( father_gui_ != NULL)
      father_gui_->EventMouseLeave();
  }

  void AddSon( GUI* son) {
    son_gui_list_.push_back( son);
  }
  void DeleteSon( GUI* son) {
    son_gui_list_.remove( son);
  }

  inline GuiType GetType() {
    return type_;
  }

protected:
  //int id_;
  GuiType type_;
  bool is_draw_;
  Coordinate coordinate_;
  Coordinate position_drawed_;
  RECT rect_drawed_;
  int frame_drawed_;
  int direction_drawed_;
  Texture texture_;
  std::list<GUI*> son_gui_list_;
  GUI* father_gui_;
};

#endif //_DREAM_GUIBASE_H_