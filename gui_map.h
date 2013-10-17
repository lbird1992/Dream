//TODO:增加NPC的显示
#ifndef _DREAM_GUIMAP_
#define _DREAM_GUIMAP_

#include "gui_manage.h"
#include "gui_dialog.h"
#include "gui_hero.h"

const float kMapSpeedLow = 0.2f;
const float kMapSpeedHigh = 0.34f;

class GUIMap : public GUIDialog {
public:
  GUIMap();

  void ResetMapID( const int map_id, const Coordinate map_center);

  void LoadMap();
  virtual void Render( const Coordinate coordinate);

  virtual void EventMouseLeftHolding() {}
  virtual void EventMouseLeftUp();
  virtual void EventMouseLeftDown() {}
  virtual void EventMouseRightClick() {}

  inline void SetPlayerGUI( GUIHero* player) {
    gui_player_ = player;
  }

  inline GUIHero* GetPlayerGUI() const {
    return gui_player_;
  }

private:
  Texture map_texture_[3][3];
  Coordinate map_center_;//窗口中心点对应的地图坐标
  int map_image_;
  int map_x_sum_;
  int map_y_sum_;//map_texture[0][0]所对应的地图编号
  Coordinate map_max_;//地图最大坐标
  GUIHero* gui_player_;//LOOP类型
};

#endif //_DREAM_GUIMAP_