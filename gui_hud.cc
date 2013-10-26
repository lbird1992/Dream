#include "gui_hud.h"
#include "gui_button.h"
#include "game_logic.h"
#include "resource_manage.h"

GUIHud::GUIHud() : GUIDialog( NULL, Coordinate( 0, 0)) {
}

void GUIHud::Init() {
  AddSon( new GUIButton( "image\\button\\attack.was", Coordinate( 325, 450), this, ClickOnAttack, 3, 3,3));
  AddSon( new GUIButton( "image\\button\\wupin.was", Coordinate( 351, 450), this, ClickOnWupin, 0, 2, 1));
  AddSon( new GUIButton( "image\\button\\give.was", Coordinate( 377, 450), this, ClickOnGive, 3, 3,3));
  AddSon( new GUIButton( "image\\button\\bargin.was", Coordinate( 403, 450), this, ClickOnBargin, 3, 3, 3));
  AddSon( new GUIButton( "image\\button\\team.was", Coordinate( 429, 450), this, ClickOnTeam, 3, 3, 3));
  AddSon( new GUIButton( "image\\button\\pet.was", Coordinate( 455,450), this, ClickOnPet, 3, 3, 3));
  AddSon( new GUIButton( "image\\button\\renwu.was", Coordinate( 481, 450), this, ClickOnRenwu, 0, 2,1));
  AddSon( new GUIButton( "image\\button\\bangpai.was", Coordinate( 507, 450), this, ClickOnBangpai, 3, 3, 3));
  AddSon( new GUIButton( "image\\button\\fashukuaijie.was", Coordinate( 533, 450), this, ClickOnFashukuaijie, 3, 3,3));
  AddSon( new GUIButton( "image\\button\\friend.was", Coordinate( 559, 450), this,ClickOnFriend, 3, 3, 3));
  AddSon( new GUIButton( "image\\button\\action.was", Coordinate( 585, 450), this, ClickOnAction, 3, 3,3));
  AddSon( new GUIButton( "image\\button\\system.was", Coordinate( 611, 450), this, ClickOnSystem, 0, 2, 1));
  char filename[256];
  sprintf( filename, "image\\head\\rightup\\%d.was", 2);//TODO: 增加人物结构体
  AddSon( new GUIButton( filename, Coordinate( 528, 3),this, ClickOnPlayerHead, 0, 0, 0));
  font_ = new GfxFont( "宋体", 14);
}

void GUIHud::Render( const Coordinate position) {
  if( is_draw_ == false)
    return;
  g_resource_manage->GetTex( "image\\other\\back1.was").Render( Coordinate( 325, 420), 0, 0);
  g_resource_manage->GetTex( "image\\other\\playerhead.was").Render( Coordinate( 525, 0), 0, 0);
  g_resource_manage->GetTex( "image\\other\\back2.was").Render( Coordinate( 573, 0), 0, 0);//人物HP
  g_resource_manage->GetTex( "image\\other\\back2.was").Render( Coordinate( 573, 12), 0, 0);
  g_resource_manage->GetTex( "image\\other\\back2.was").Render( Coordinate( 573, 24), 0, 0);
  g_resource_manage->GetTex( "image\\other\\back2.was").Render( Coordinate( 573, 36), 0, 0);

  int second = g_game_logic->GetTime()%1800;
  if( second >= 600 && second <= 1500) {
    int tmp_int = second - 600;
    g_resource_manage->GetTex( "image\\other\\timeday.png").RenderEx( Coordinate(24,32), Coordinate( 
                                                                                    static_cast<float>(tmp_int/900.0*360), 0), 
                                                  70, 30, 0, 0);
  }
  else {
    int tmp_int = second - 1500;
    if( tmp_int < 0)
      tmp_int += 1800;
    g_resource_manage->GetTex( "image\\other\\timenight.png").RenderEx( Coordinate(24,32), Coordinate( 
                                                                                     static_cast<float>(tmp_int/900.0*360), 0), 
                                                  70, 30, 0, 0);
  }
  g_resource_manage->GetTex( "image\\other\\timeback.png").Render( Coordinate( 0, 0), 0, 0);
  char filename[256];
  sprintf( filename, "image\\other\\time\\%d.was", second/150+1);
  g_resource_manage->GetTex( filename).Render( Coordinate( 0, 25), 0, 0);

  GUIDialog::Render( position);

  font_->SetColor( 0xFFFFFFFF);
  font_->Print( 25, 65, "X:%.0f Y:%.0f", g_game_logic->GetPlayerCoordinate().GetX(), g_game_logic->GetPlayerCoordinate().GetY());
}

void GUIHud::ClickOnAttack() {}
void GUIHud::ClickOnAction() {}
void GUIHud::ClickOnBangpai() {}
void GUIHud::ClickOnBargin() {}
void GUIHud::ClickOnFashukuaijie() {}
void GUIHud::ClickOnFriend() {}
void GUIHud::ClickOnGive() {}
void GUIHud::ClickOnPet() {}
void GUIHud::ClickOnPlayerHead() {}
void GUIHud::ClickOnRenwu() {}
void GUIHud::ClickOnSystem() {}
void GUIHud::ClickOnTeam() {}
void GUIHud::ClickOnWupin() {}
