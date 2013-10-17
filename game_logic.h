#ifndef _DREAM_GAMELOGIC_
#define _DREAM_GAMELOGIC_

#include <stdio.h>
#include <stdint.h>
#include "hge_include.h"

const int kMaxMapCount = 100;//��Ϸ�е�ͼ������������
const double kWalkSpeed = 0.27;//�����ٶ�

struct MapData {
  int map_image;
  int map_x_max;
  int map_y_max;
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

  void Walk();//�������ߣ��˴�ֻ�����������꣬��������������GUIMAP��ʵ��

private:
  int map_id_;
  Coordinate map_coordinate_;
  Coordinate map_coordinate_to_go_;//Ŀ�������
  int player_direction_;
  MapData map_data_[kMaxMapCount];
};

extern GameLogic* g_game_logic;

#endif //_DREAM_GAMELOGIC