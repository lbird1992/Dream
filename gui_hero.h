#ifndef _DREAM_GUIHERO_
#define _DREAM_GUIHERO_

#include "gui_animation.h"
#include "game_logic.h"
#include "GfxFont.h"

class GUIHero : public GUIAnimation {
public:
  GUIHero( const std::string directory_path, const Coordinate position, const GUI* father);

  void AddAnimation( const int action, const int direction, const bool is_loop);
  void EndAnimation();

  virtual void Render( const Coordinate position);

  inline void SetPlayer( const Player* player) {
    player_ = const_cast<Player*>(player);
  }

protected:
  int action_;
  std::string directory_path_;
  Texture texture_second_;//纹理备份——仅放置站立动作
  int frame_second_;
  float frame_fill_second_;
  Player* player_; 
public:
  static GfxFont* name_font_;
};


#endif