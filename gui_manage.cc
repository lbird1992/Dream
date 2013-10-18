#include "gui_manage.h"

GUIManage* g_gui_manage;

void GUIManage::Init() {
  root_gui_pointer_ = new GUI( NULL, Coordinate( 0, 0), NULL);
  mouse_position_ = mouse_position_previous_ = Coordinate( 0, 0);
  mouse_on_ = mouse_on_previous_ = NULL;
}

void GUIManage::Reset() {
  if( root_gui_pointer_ != NULL) {
    delete root_gui_pointer_;
  }
  root_gui_pointer_ = new GUI( NULL, Coordinate( 0, 0), NULL);
  mouse_position_previous_ = Coordinate(0,0);
  mouse_on_ = mouse_on_previous_ = NULL;
}

void GUIManage::Render() const {
  root_gui_pointer_->Render( Coordinate( 0, 0));
}

void GUIManage::Control() {
  HGE* hge = hgeCreate( HGE_VERSION);
  UpdateMousePosition();
  mouse_on_previous_ = mouse_on_;
  mouse_on_ = GetMouseOn();
  if( mouse_on_ != mouse_on_previous_ && mouse_on_ != NULL) { //得到鼠标事件
    mouse_on_->EventMouseGet();
    if( mouse_on_previous_ != NULL)
      mouse_on_previous_->EventMouseLeave();
  }
  if( hge->Input_KeyDown( HGEK_LBUTTON)) {
    if( mouse_on_ != NULL)
      mouse_on_->EventMouseLeftDown();
  }
  else if( hge->Input_KeyUp( HGEK_LBUTTON) ) {
    if( mouse_on_ != NULL)
      mouse_on_->EventMouseLeftUp();
  }
  else if( hge->Input_GetKeyState( HGEK_LBUTTON)) {
    if( mouse_on_ != NULL)
      mouse_on_->EventMouseLeftHolding();
  }
  if( hge->Input_KeyUp( HGEK_RBUTTON) ) {
    if( mouse_on_ != NULL)
      mouse_on_->EventMouseRightClick();
  }
  KillAnimation();
}

GUI* GUIManage::GetMouseOn() const {
  std::list<GUI*> son_list = root_gui_pointer_->GetSonGuiList();
  for( std::list<GUI*>::reverse_iterator reverse_iter = son_list.rbegin();
      reverse_iter != son_list.rend(); ++reverse_iter) {
    //使用反向迭代器，使第一个结果就是最终的结果
    if( (*reverse_iter)->IsDraw() == false) 
      continue;
    if( (*reverse_iter)->GetTexture().GetDirectionCount() == 0) {
      GUI* gui = this->GetMouseOnInNoTextureDialog( (*reverse_iter));
      if( gui == NULL)
        continue;
      else
        return gui;
    }
    else {
      RECT rect = (*reverse_iter)->GetRectDrawed();
      if( rect.top <= mouse_position_.GetY() && rect.bottom >= mouse_position_.GetY() &&
          rect.left <= mouse_position_.GetX() && rect.right >= mouse_position_.GetX() ) {
         return this->GetMouseOnInDialogWithTexture( (*reverse_iter));
      }
    }
  }
  return NULL;
}

GUI* GUIManage::GetMouseOnInNoTextureDialog( const GUI* gui) const {
  std::list<GUI*> son_list = gui->GetSonGuiList();
  for( std::list<GUI*>::iterator iter = son_list.begin(); iter != son_list.end(); ++iter) {
    if( (*iter)->IsDraw() == false)
      continue;
    RECT rect = (*iter)->GetRectDrawed();
    if( rect.top <= mouse_position_.GetY() && rect.bottom >= mouse_position_.GetY() &&
        rect.left <= mouse_position_.GetX() && rect.right >= mouse_position_.GetY() ) {
      if( ((*iter)->GetPixel( mouse_position_) & 0xFF000000) != 0)
        return (*iter);
    }
  }
  return NULL;
}

GUI* GUIManage::GetMouseOnInDialogWithTexture( const GUI* gui) const {
  std::list<GUI*> son_list = gui->GetSonGuiList();
  for( std::list<GUI*>::iterator iter = son_list.begin();
      iter != son_list.end(); ++iter) {
    RECT rect = (*iter)->GetRectDrawed();
    if( rect.top <= mouse_position_.GetY() && rect.bottom >= mouse_position_.GetY() &&
        rect.left <= mouse_position_.GetX() && rect.right >= mouse_position_.GetX() ) {
      if( ((*iter)->GetPixel( mouse_position_) & 0xFF000000) != 0)
        return (*iter);
    }
  }
  return const_cast<GUI*>(gui);
}

void GUIManage::AddAnimation( GUIAnimation* animation) {
  animations_.push_back( animation);
}

void GUIManage::KillAnimation() {
  for( std::list<GUIAnimation*>::iterator iter = animations_.begin(); iter != animations_.end(); ) {
    if( (*iter)->IsEnd() ) {
      (*iter)->GetFatherGUI()->DeleteSon( (*iter));
      delete (*iter++);
    }
    else {
      ++iter;
    }
  }
}

