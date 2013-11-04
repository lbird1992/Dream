#include <process.h>
#include "game_manage.h"
#include "gui_manage.h"
#include "gui_begin.h"
#include "gui_map.h"
#include "game_logic.h"
#include "gui_hud.h"
#include "gui_textview.h"
#include "thread_time.h"

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
  g_game_logic->GetPlayer()->NewPlayer( 1, "测试人物", 2);
  g_game_logic->SetPlayerCoordinate( Coordinate(11, 274));
  g_game_logic->SetMapAim( Coordinate(11,274));
  g_game_logic->SetPlayerDirection( 1);
  time_thread_handle_ = (HANDLE)_beginthreadex( NULL, 0, TimeThreadFunction, g_game_logic->MakeTime(0), 0, &time_thread_id_);

  gui_map_ = new GUIMap();
  gui_map_->ResetMapID( 1, Coordinate( 11, 274));
  GUIHero* player = new GUIHero( "image\\hero\\2", Coordinate(0, 0), gui_map_);
  player->SetPlayer( g_game_logic->GetPlayer());
  gui_map_->SetPlayerGUI( player);
  gui_map_->SetOnDraw();
  GUIHud* gui_hud = new GUIHud();
  gui_hud->Init();
  gui_hud->SetOnDraw();
  g_gui_manage->GetRoot()->AddSon( gui_map_);
  g_gui_manage->GetRoot()->AddSon( gui_hud);
  g_gui_manage->GetRoot()->AddSon( gui_hud);//人物小头像
  g_gui_text_view = new GUITextView(g_gui_manage->GetRoot());//整个GUI树的根节点
  g_gui_text_view->SetOnDraw();
  g_gui_manage->GetRoot()->AddSon( g_gui_text_view);
  g_gui_manage->GetRoot()->SetOnDraw();

}

void GameManage::SetEnd() {
  game_state_ = GS_END;
}