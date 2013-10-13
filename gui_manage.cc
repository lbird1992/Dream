#include "gui_manage.h"

void GUIManage::Init() {
  root_gui_pointer_ = new GUI( NULL, Coordinate( 0, 0), NULL);
  mouse_position_ = mouse_position_previous_ = Coordinate( 0, 0);
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
    mouse_on_previous_->EventMouseLeave();
  }
  if( hge->Input_KeyDown( HGEK_LBUTTON)) {
    mouse_on_->EventMouseLeftDown();
  }
  else if( hge->Input_KeyUp( HGEK_LBUTTON) ) {
    mouse_on_->EventMouseLeftUp();
  }
  else if( hge->Input_GetKeyState( HGEK_LBUTTON)) {
    mouse_on_->EventMouseLeftHolding();
  }
  if( hge->Input_KeyUp( HGEK_RBUTTON) ) {
    mouse_on_->EventMouseRightClick();
  }
}

GUI* GUIManage::GetMouseOn() {
  for( std::list<GUI*>::reverse_iterator reverse_iter = root_gui_pointer_->GetSonGuiList().rbegin();
      reverse_iter != root_gui_pointer_->GetSonGuiList().rend(); ++reverse_iter) {  
        //使用反向迭代器，使第一个结果就是最终的结果
    if( (*reverse_iter)->IsDraw() == false)
      continue;
    if( (*reverse_iter)->GetTexture().GetDirectionCount() == 0) {
      for( std::list<GUI*>::iterator iter = (*reverse_iter)->GetSonGuiList().begin(); 
          iter != (*reverse_iter)->GetSonGuiList().end(); ++iter) {
        RECT rect = (*iter)->GetRectDrawed();
        if( rect.left <= mouse_position_.GetX() && rect.right >= mouse_position_.GetX() &&
            rect.top <= mouse_position_.GetY() && rect.bottom >= mouse_position_.GetY() ) {
          if( ((*iter)->GetPixel( mouse_position_) & 0xFF000000) != 0)
            return (*iter);
        }
      }
    }
    else{
      RECT rect = (*reverse_iter)->GetRectDrawed();
      if( rect.left <= mouse_position_.GetX() && rect.right >= mouse_position_.GetX() &&
          rect.top <= mouse_position_.GetY() && rect.bottom >= mouse_position_.GetY() ) {
        for( std::list<GUI*>::iterator iter = (*reverse_iter)->GetSonGuiList().begin(); 
             iter != (*reverse_iter)->GetSonGuiList().end(); ++iter) {
          RECT rect_son = (*iter)->GetRectDrawed();
          if( rect_son.left <= mouse_position_.GetX() && rect_son.right >= mouse_position_.GetX() &&
             rect_son.top <= mouse_position_.GetY() && rect_son.bottom >= mouse_position_.GetY() ) {
            if( ((*iter)->GetPixel( mouse_position_) & 0xFF000000) != 0)
              return (*iter);
          }
        }
        return (*reverse_iter);
      }
    }
  }
  return NULL;
}