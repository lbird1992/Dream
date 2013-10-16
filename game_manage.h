#ifndef _DREAM_GAMEMANAGE_
#define _DREAM_GAMEMANAGE_

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

private:
  GameState game_state_;
};

extern GameManage* g_game_manage;

#endif