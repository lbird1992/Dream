#include "game_logic.h"
#include "game_manage.h"

GameLogic* g_game_logic;

void GameLogic::LoadMapIni() {
  char map_name[256];
  FILE* fp = fopen( "data\\map.ini","r");
  for( int i=1; ; ++i) {
    if( fscanf(fp, "%d %s", &map_data_[i].map_image, map_name) == EOF)
      break;
  }
  fclose(fp);
}

void GameLogic::Walk() {
  float x_length = (map_coordinate_to_go_ - map_coordinate_).GetX();
  float y_length = (map_coordinate_to_go_ - map_coordinate_).GetY();
  if( x_length * x_length + y_length * y_length < kWalkSpeed * kWalkSpeed) {
    //偏移量不足一帧移动
    map_coordinate_ = map_coordinate_to_go_;
    g_game_manage->GetMap()->GetPlayerGUI()->EndAnimation();
    return;
  }
  double x_speed, y_speed;
  if( x_length == 0) {
    x_speed = 0;
    y_speed = kWalkSpeed;
  }
  else {
    x_speed = kWalkSpeed * cos( atan( abs(y_length)/abs(x_length)));
    y_speed = kWalkSpeed * sin( atan( abs(y_length)/abs(x_length)));
  }
  if( x_length < 0)
    x_speed *= -1;
  if( y_length < 0)
    y_speed *= -1;
  if( g_game_manage->GetMap()->GetCell( 
                          map_coordinate_ + Coordinate(static_cast<float>(x_speed), 
                                                      static_cast<float>(y_speed))) == 1)
  {
    map_coordinate_to_go_ = map_coordinate_;
    g_game_manage->GetMap()->GetPlayerGUI()->EndAnimation();
    return;
  }
  else if( g_game_manage->GetMap()->GetCell( 
                                    map_coordinate_ + Coordinate(static_cast<float>(x_speed),
                                                                static_cast<float>(y_speed))) == 2) {
    //TODO:找到传送点
    int x = static_cast<int>(map_coordinate_.GetX() + x_speed);
    int y = static_cast<int>(map_coordinate_.GetY() + y_speed);
    std::list<TransformData> transform_data = g_game_manage->GetMap()->GetTransformData();
    if( transform_data.size() != 0) {
      double min_distance = 10000.0;
      TransformData nearest_transform;
      for( std::list<TransformData>::iterator iter = transform_data.begin(); iter != transform_data.end(); ++iter) {
        int x_distance = x-iter->src_x;
        int y_distance = y-iter->src_y;
        if( min_distance*min_distance > x_distance*x_distance + y_distance*y_distance) {
          min_distance = sqrt( static_cast<double>(x_distance*x_distance+y_distance*y_distance));
          nearest_transform = *iter;
        }
      }
      map_coordinate_ = Coordinate( static_cast<float>(nearest_transform.dst_x), 
                                    static_cast<float>(nearest_transform.dst_y));
      map_id_ = nearest_transform.dst_map_id;
      g_game_manage->GetMap()->ResetMapID( map_id_, map_coordinate_);
      map_coordinate_to_go_ = map_coordinate_;
      g_game_manage->GetMap()->GetPlayerGUI()->EndAnimation();
    }
  }
  map_coordinate_.Plus( Coordinate(static_cast<float>(x_speed), static_cast<float>(y_speed)));
  
  if( x_speed > 0 && y_speed > 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 0, true);
  else if( x_speed < 0 && y_speed > 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 1, true);
  else if( x_speed < 0 && y_speed < 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 2, true);
  else if( x_speed > 0 && y_speed < 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 3, true);
  else if( x_speed == 0 && y_speed > 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 4, true);
  else if( x_speed < 0 && y_speed == 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 5, true);
  else if( x_speed == 0 && y_speed < 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 6, true);
  else if( x_speed > 0 && y_speed == 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 7, true);
}