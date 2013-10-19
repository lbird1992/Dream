#include <string>
#include <list>
#include "gui_textview.h"
#include "gui_base.h"
#include "hge_include.h"
#include "resource_manage.h"

GUITextView* g_gui_text_view;

GUITextView::GUITextView( const GUI* father):
GUI("image\\other\\hintback.png", Coordinate(0,90),father){
  //将灰色的条载入内存
  font_ = new GfxFont( "宋体", 15);
}

GUITextView::~GUITextView(){
 }

void GUITextView::AddString(std::string string){
  ListElement temp;
  temp.str = string;
  temp.x = 640;
  string_list_.push_back(temp);
  is_draw_ = true;
}
void GUITextView::Render(const Coordinate position){
  if(is_draw_ == false)
    return;
  GUI::Render( position);//显示白条
  if( string_list_.size() == 0)
    is_draw_ = false;
  font_->SetColor( 0xFF000000);

  std::list<ListElement>::iterator it;
  std::list<ListElement>::iterator iter;
  for(it = string_list_.begin();it!=string_list_.end();){
    if(it->x<640){
      iter = it;
      font_->Print( static_cast<float>(it->x), 90, "%s",it->str.c_str());
      it->x -= kHintSpeed;
      if( it->x + font_->GetTextSize( it->str.c_str()).cx <= 0)
        string_list_.erase( it++);
      else
        ++it;
      continue;
    }
    if( it != string_list_.begin() ) {
      if( iter->x + font_->GetTextSize( iter->str.c_str() ).cx >= 640)
        break;
    }

    it->x -= kHintSpeed;
    break;
  }
}

