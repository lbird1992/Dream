#ifndef _DREAM_GAMELOGIC_
#define _DREAM_GAMELOGIC_

#include <stdio.h>
#include <stdint.h>
#include "hge_include.h"

const int kMaxMapCount = 300;//游戏中地图单体的最大数量
const double kWalkSpeed = 0.27;//行走速度

struct MapData {
  int map_image;
};

class GameLogic {
public:
  inline Coordinate GetPlayerCoordinate() const {
    return map_coordinate_;
  }
  inline void SetPlayerCoordinate( const Coordinate coordinate) {
    map_coordinate_ = coordinate;
  }
  inline int GetMapID() const {
    return map_id_;
  }
  inline void SetMapID( const int map_id) {
    map_id_ = map_id;
  }
  inline int GetPlayerDirection() const {
    return player_direction_;
  }
  inline void SetPlayerDirection( const int direction) {
    player_direction_ = direction;
  }
  void LoadMapIni();
  inline MapData GetMapData( const int map_id) const {
    return map_data_[map_id];
  }

  inline Coordinate GetMapAim() const {
    return map_coordinate_to_go_;
  }
  inline void SetMapAim( const Coordinate aim) {
    map_coordinate_to_go_ = aim;
  }
  inline uint64_t* MakeTime(const uint64_t time) {
    time_ = new uint64_t(time);
    return time_;
  }
  inline uint64_t GetTime() const {
    return *time_;
  }

  void Walk();//人物行走，此处只更改人物坐标，画面中心坐标在GUIMAP中实现

private:
  int map_id_;
  Coordinate map_coordinate_;
  Coordinate map_coordinate_to_go_;//目标点坐标
  int player_direction_;
  MapData map_data_[kMaxMapCount];
  uint64_t* time_;
};

extern GameLogic* g_game_logic;

#endif //_DREAM_GAMELOGIC