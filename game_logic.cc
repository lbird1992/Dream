#include "game_logic.h"
#include "game_manage.h"

GameLogic* g_game_logic;

void GameLogic::LoadMapIni() {
  int i;
  FILE* fp = fopen( "data\\map.ini","r");
  while( fscanf( fp, "%d", &i) != EOF)
    fscanf(fp, "%d %d %d", &map_data_[i].map_image, &map_data_[i].map_x_max, &map_data_[i].map_y_max);
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