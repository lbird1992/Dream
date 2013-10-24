//TODO:增加NPC的显示
#ifndef _DREAM_GUIMAP_
#define _DREAM_GUIMAP_

#include "gui_manage.h"
#include "gui_dialog.h"
#include "gui_hero.h"

const float kMapSpeedLow = 0.23f;
const float kMapSpeedHigh = 0.34f;

struct TransformData {//地图的转换数据
  int src_x;//起始点坐标
  int src_y;
  int dst_map_id;//目标点地图ID
  int dst_x;//目标点坐标
  int dst_y;
};

struct MaskData {
  int start_x;
  int start_y;
  int width;
  int height;
  uint32_t mask_size;
  uint8_t* data;
};

class GUIMap : public GUIDialog {
public:
  GUIMap();

  void ResetMapID( const int map_id, const Coordinate map_center);
  void ReadBlock( const int block_id, const int x, const int y);
  int DecompressMask( void* in, void* out);
  uint8_t* GUIMap::JpgHandler(uint8_t* Buffer, int inSize, uint32_t* outSize);
  void SetMask( const int x, const int y);

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
  uint8_t GetCell( const Coordinate position) const;
  
  std::list<TransformData> GetTransformData() const {
    return transform_data_list_;
  }

private:
  FILE* map_file_;
  HTEXTURE big_map_texture_;

  uint32_t* block_offset_;
  int map_block_row_count_;
  int map_block_col_count_;
  int map_block_count_;
  int mask_count_;
  MaskData* mask_data_;
  std::list<int> mask_index_to_draw_;
  std::list<TransformData> transform_data_list_;
  uint8_t cell_data_[48*36];

  Coordinate map_center_;//窗口中心点对应的地图坐标
  int map_image_;
  int map_x_sum_;
  int map_y_sum_;//map_texture[0][0]所对应的地图编号
  Coordinate map_max_;//地图最大坐标
  GUIHero* gui_player_;//LOOP类型
};

#endif //_DREAM_GUIMAP_