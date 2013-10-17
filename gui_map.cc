#include "gui_map.h"
#include "game_logic.h"

GUIMap::GUIMap() : GUIDialog( "image\\other\\back.jpg", Coordinate(0, 0)) {
  memset( map_texture_, 0, sizeof(map_texture_));
}

void GUIMap::ResetMapID( const int map_id, const Coordinate map_center) {
  MapData map_data = g_game_logic->GetMapData( map_id);
  map_image_ = map_data.map_image;
  map_max_ = Coordinate( map_data.map_x_max+0.5f, map_data.map_y_max+0.5f);//加0.5防止数据类型转换时的舍入误差
  if( map_max_.GetX() - 16 < map_center.GetX())
    map_center_.SetX( map_max_.GetX()-16);
  else
    map_center_.SetX( max(16, map_center.GetX()));
  if( map_max_.GetY() - 12 < map_center.GetY())
    map_center_.SetY( map_max_.GetY() - 12);
  else
    map_center_.SetY( max(12, map_center.GetX()));
  map_x_sum_ = static_cast<int>(map_center_.GetX())/16;
  map_y_sum_ = static_cast<int>(map_center_.GetY())/12;
  char filename[256];
  for( int x = 0; x < 3; ++x) {
    for( int y = 0; y < 3; ++y) {
      if( map_texture_[x][y].GetDirectionCount() != 0)
        g_resource_manage->DeleteTexWithTexture( map_texture_[x][y]);
      if( map_y_sum_ + y < 10)
        sprintf( filename, "image\\map\\%d\\%d.map_0%d_", map_image_, map_image_, map_y_sum_+y);
      else
        sprintf( filename, "image\\map\\%d\\%d.map_%d_", map_image_, map_image_, map_y_sum_+y);
      if( map_x_sum_ + x < 10)
        sprintf( filename, "%s0%d.jpg", filename, map_x_sum_+x);
      else
        sprintf( filename, "%s%d.jpg", filename, map_x_sum_+x);
      map_texture_[x][y] = g_resource_manage->GetTex( filename);
    }
  }
}

void GUIMap::LoadMap() {
  //TODO:解决释放原有纹理会出错的问题
  int map_x_sum = static_cast<int>(map_center_.GetX())/16;
  int map_y_sum = static_cast<int>(map_center_.GetY())/12;//计算地图序号
  if( map_x_sum_ == map_x_sum && map_y_sum_ == map_y_sum)
    return;//不需要重新载入地图
  map_x_sum_ = map_x_sum;
  map_y_sum_ = map_y_sum;
  char filename[256];
  for( int x = 0; x < 3; ++x) {
    for( int y = 0; y < 3; ++y) {
      if( map_y_sum + y < 10)
        sprintf( filename, "image\\map\\%d\\%d.map_0%d_", map_image_, map_image_, map_y_sum+y);
      else
        sprintf( filename, "image\\map\\%d\\%d.map_%d_", map_image_, map_image_, map_y_sum+y);
      if( map_x_sum + x < 10)
        sprintf( filename, "%s0%d.jpg", filename, map_x_sum+x);
      else
        sprintf( filename, "%s%d.jpg", filename, map_x_sum+x);
      map_texture_[x][y] = g_resource_manage->GetTex( filename);
    }
  }
}

void GUIMap::Render( const Coordinate coordinate) {
  LoadMap();
  float x_position = -( static_cast<int>(map_center_.GetX())%16 + map_center_.GetX() 
                                - static_cast<int>(map_center_.GetX())) * 20;//0,0号地图渲染位置
  float y_position = -( static_cast<int>(map_center_.GetY())%12 + map_center_.GetY()
                                - static_cast<int>(map_center_.GetY())) * 20;
  for( int x=0; x<3; ++x) {
    for( int y=0; y<3; ++y) {
      if( map_texture_[x][y].GetDirectionCount() != 0) {
        map_texture_[x][y].Render( Coordinate( static_cast<float>( static_cast<int>(x_position) + 320*x),
                                               static_cast<float>( static_cast<int>(y_position) + 240*y) ) ,
                                               0, D_ONLY);
      }
    }
  }
  frame_drawed_ = 0;
  direction_drawed_ = 0;
  rect_drawed_.top = 0;
  rect_drawed_.left = 0;
  rect_drawed_.bottom = 480;
  rect_drawed_.right = 640;
  position_drawed_ = Coordinate( 0, 0);

  Coordinate player_position = g_game_logic->GetPlayerCoordinate();
  float x_position_draw_player = 320 + (player_position-map_center_).GetX() * 20;
  float y_position_draw_player = 240 + (player_position-map_center_).GetY() * 20;
  gui_player_->Render( Coordinate( x_position_draw_player, y_position_draw_player));

  float map_speed = kMapSpeedLow;
  if( abs( map_center_.GetX() - player_position.GetX()) >= 8)
    map_speed = kMapSpeedHigh;
  else if( abs( map_center_.GetY() - player_position.GetY()) >= 6)
    map_speed = kMapSpeedHigh;

  float x_length = (player_position - map_center_).GetX();
  float y_length = (player_position - map_center_).GetY();
  if( x_length * x_length + y_length * y_length < map_speed * map_speed) {
    //偏移量不足一帧移动
    map_center_ = player_position;
  }
  else {
    double x_speed, y_speed;
    if( x_length == 0) {
      x_speed = 0;
      y_speed = map_speed;
    }
    else {
      x_speed = map_speed * cos( atan( abs(y_length)/abs(x_length)));
      y_speed = map_speed * sin( atan( abs(y_length)/abs(x_length)));
    }
    if( x_length < 0)
      x_speed *= -1;
    if( y_length < 0)
      y_speed *= -1;
    map_center_.Plus( Coordinate(static_cast<float>(x_speed), static_cast<float>(y_speed)));
  }
  if( map_center_.GetX() < 16)
    map_center_.SetX( 16);
  else if( map_center_.GetX() > map_max_.GetX() - 16)
    map_center_.SetX( map_max_.GetX() - 16);
  if( map_center_.GetY() < 12)
    map_center_.SetY( 12);
  else if( map_center_.GetY() > map_max_.GetY() - 12)
    map_center_.SetY( map_max_.GetY() - 12);
}

void GUIMap::EventMouseLeftUp() {
  Coordinate mouse_position = g_gui_manage->GetMousePosition();
  float x_aim = (mouse_position.GetX() - 320)/20 + map_center_.GetX();
  float y_aim = (mouse_position.GetY() - 240)/20 + map_center_.GetY();
  g_game_logic->SetMapAim( Coordinate(x_aim, y_aim));
}