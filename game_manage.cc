#include "game_manage.h"
#include "gui_manage.h"
#include "gui_begin.h"
#include "gui_map.h"
#include "game_logic.h"

GameManage* g_game_manage;

void GameManage::SetBegin() {
  game_state_ = GS_BEGIN;
  GUIBegin* begin_dialog = new GUIBegin();
  begin_dialog->Init();
  g_gui_manage->GetRoot()->AddSon( begin_dialog);
};

void GameManage::SetPlay() {
  game_state_ = GS_PLAYING;
  g_gui_manage->Reset();
  g_game_logic->LoadMapIni();
  g_game_logic->SetMapID( 1);
  g_game_logic->SetPlayerCoordinate( Coordinate(10, 10));
  g_game_logic->SetMapAim( Coordinate(10,10));
  g_game_logic->SetPlayerDirection( 1);
  gui_map_ = new GUIMap();
  gui_map_->ResetMapID( 1, Coordinate( 10, 10));
  GUIHero* player = new GUIHero( "image\\hero\\2", Coordinate(0, 0), gui_map_);
  gui_map_->SetPlayerGUI( player);
  gui_map_->SetOnDraw();
  g_gui_manage->GetRoot()->AddSon( gui_map_);
  g_gui_manage->GetRoot()->SetOnDraw();
}

void GameManage::SetEnd() {
  game_state_ = GS_END;
}