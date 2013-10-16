#include "game_manage.h"
#include "gui_manage.h"
#include "gui_begin.h"

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
}

void GameManage::SetEnd() {
  game_state_ = GS_END;
}