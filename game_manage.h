#ifndef _DREAM_GAMEMANAGE_
#define _DREAM_GAMEMANAGE_

#include "gui_map.h"

enum GameState {
  GS_BEGIN = 1,
  GS_PLAYING,
  GS_END
};

class GameManage {
public:
  inline GameState GetGameState() const {
    return game_state_;
  }

  void SetBegin();
  void SetPlay();
  void SetEnd();
  inline GUIMap* GetMap() {
    return gui_map_;
  }

private:
  GameState game_state_;
  GUIMap* gui_map_;
};

extern GameManage* g_game_manage;

#endif