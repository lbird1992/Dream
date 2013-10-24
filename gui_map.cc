#include "gui_map.h"
#include "game_logic.h"
#include "GfxFont.h"

GUIMap::GUIMap() : GUIDialog( "image\\other\\back.jpg", Coordinate(0, 0)) {
  big_map_texture_ = 0;
  map_file_ = NULL;
  block_offset_ = 0;
  mask_data_ = 0;
}

void GUIMap::ResetMapID( const int map_id, const Coordinate map_center) {
  if( map_file_ != NULL) {
    fclose( map_file_);
  }
  if( block_offset_ != NULL)
    delete block_offset_;
  if( mask_data_ != NULL)
    delete mask_data_;
  HGE* hge = hgeCreate( HGE_VERSION);
  if( big_map_texture_ != 0)
    hge->Texture_Free( big_map_texture_);
  big_map_texture_ = hge->Texture_Create( 960, 720);
  map_image_ = g_game_logic->GetMapData( map_id).map_image;
  char filename[256];
  sprintf( filename, "image\\scene\\%d.map", map_image_);
  map_file_ = fopen( filename, "rb");
  if( map_file_ == NULL)
    return;

  uint32_t map_width, map_height;
  int map_flag;
  fread( &map_flag, sizeof( map_flag), 1, map_file_);
  fread( &map_width, sizeof( map_width), 1, map_file_);
  fread( &map_height, sizeof( map_height), 1, map_file_);
  map_block_col_count_ = static_cast<int>(ceil( static_cast<double>(map_width / 320.0)));
  map_block_row_count_ = static_cast<int>(ceil( static_cast<double>(map_height/ 240.0)));
  map_block_count_ = map_block_row_count_ * map_block_col_count_;
  block_offset_ = new uint32_t[map_block_count_];
  fread( block_offset_, sizeof(uint32_t)*map_block_count_, 1, map_file_);//读取地图数据偏移量
  //以下读取MASK数据
  uint32_t unknown;
  fread( &unknown, sizeof( unknown), 1, map_file_);
  fread( &mask_count_, sizeof( mask_count_), 1, map_file_);
  uint32_t* mask_offset = new uint32_t[mask_count_];
  fread( mask_offset, sizeof(uint32_t)*mask_count_, 1, map_file_);
  mask_data_ = new MaskData[mask_count_];
  for( int i=0; i<mask_count_; i++) {
    fseek( map_file_, mask_offset[i], SEEK_SET);
    fread( &mask_data_[i].start_x, sizeof(mask_data_[i].start_x), 1, map_file_);
    fread( &mask_data_[i].start_y, sizeof(mask_data_[i].start_y), 1, map_file_);
    fread( &mask_data_[i].width, sizeof( mask_data_[i].width), 1, map_file_);
    fread( &mask_data_[i].height, sizeof( mask_data_[i].height), 1, map_file_);
    uint32_t mask_size;
    fread( &mask_size, sizeof( mask_size), 1, map_file_);
    if( mask_data_[i].width == 0 || mask_data_[i].height == 0 || mask_size == 0)
      break;
    int align_width = (mask_data_[i].width/4+(mask_data_[i].width%4!=0))*4;
    uint8_t* data = new uint8_t[align_width*mask_data_[i].height/4];
    mask_data_[i].data = new uint8_t[mask_data_[i].width * mask_data_[i].height];
    uint8_t* mask_data = new uint8_t[mask_size];
    fread( mask_data, sizeof(uint8_t), mask_size, map_file_);
    mask_data_[i].mask_size = DecompressMask( mask_data, data);
    delete mask_data;
    for( int h=0; h<mask_data_[i].height; h++) {
      for( int w=0; w<mask_data_[i].width; w++) {
        int mask_index = (h*align_width+w)*2;
        uint8_t mask_value = data[mask_index/8] >> (mask_index%8);
        if( (mask_value & 3) == 3) {
          mask_data_[i].data[ h*mask_data_[i].width+w] = 1;
        }
        else {
          mask_data_[i].data[ h*mask_data_[i].width+w] = 0;
        }
      }
    }
    delete data;
  }
  delete mask_offset;

  map_max_ = Coordinate( static_cast<float>( map_width/20), static_cast<float>(map_height/20));
  if( map_max_.GetX() - 16 < map_center.GetX())
    map_center_.SetX( map_max_.GetX()-16);
  else
    map_center_.SetX( max(16, map_center.GetX()));
  if( map_max_.GetY() - 12 < map_center.GetY())
    map_center_.SetY( map_max_.GetY() - 12);
  else
    map_center_.SetY( max(12, map_center.GetY()));
  map_x_sum_ = static_cast<int>(map_center_.GetX())/16-1;
  map_y_sum_ = static_cast<int>(map_center_.GetY())/12-1;
  for( int x = 0; x<3; ++x) {
    for( int y=0; y<3; ++y) {
      if( map_x_sum_ + x < 0)
        continue;
      if( map_y_sum_ + y < 0)
        continue;
      if( map_x_sum_ + x >= map_block_col_count_)
        continue;
      if( map_y_sum_ + y >= map_block_row_count_)
        continue;
      ReadBlock( (map_y_sum_ + y)*map_block_col_count_ + map_x_sum_ + x, x, y);
    }
  }
  SetMask( map_x_sum_, map_y_sum_);

  //读取转换数据
  sprintf( filename, "data\\map_data\\%d.ini", map_id);
  FILE* fp = fopen( filename, "r");
  if( fp == 0)
    return;
  transform_data_list_.clear();
  int src_x, src_y, dst_id, dst_x, dst_y;
  while( fscanf(fp, "%d", &src_x) != EOF) {
    fscanf( fp, "%d %d %d %d", &src_y, &dst_id, &dst_x, &dst_y);
    src_y = static_cast<int>(map_max_.GetY() - src_y);
    sprintf( filename, "image\\scene\\%d.map", g_game_logic->GetMapData(dst_id).map_image );
    FILE* map_tmp = fopen( filename, "rb");
    fread( &map_flag, sizeof( map_flag), 1, map_tmp);
    fread( &map_width, sizeof( map_width), 1, map_tmp);
    fread( &map_height, sizeof( map_height), 1, map_tmp);
    fclose(map_tmp);
    dst_y = map_height/20 - dst_y;
    TransformData transform_data;
    transform_data.src_x = src_x;
    transform_data.src_y = src_y;
    transform_data.dst_map_id = dst_id;
    transform_data.dst_x = dst_x;
    transform_data.dst_y = dst_y;
    transform_data_list_.push_back( transform_data);
  }
  fclose( fp);
}

uint8_t GUIMap::GetCell( const Coordinate position) const {
  int x = static_cast<int>(position.GetX() - map_x_sum_*16);
  int y = static_cast<int>(position.GetY() - map_y_sum_*12);
  return cell_data_[y*48+x];
}

void GUIMap::ReadBlock( const int block_id, const int x, const int y) {
  fseek( map_file_, block_offset_[block_id], SEEK_SET);
  uint32_t unknown_unit_size;//未知数据大小
  fread( &unknown_unit_size, sizeof(unknown_unit_size), 1, map_file_);
  if( unknown_unit_size > 0) {
    uint8_t* unknown_data = new uint8_t[unknown_unit_size*4];
    fread( unknown_data, sizeof(uint8_t), unknown_unit_size*4, map_file_);
    delete unknown_data;
  }
  //读取JPG
  uint32_t flag,size;
  fread( &flag, sizeof(flag), 1, map_file_);
  fread( &size, sizeof(size), 1, map_file_);
  if( flag != 0x4A504547) {
    //文件格式可能错误
  }
  uint8_t* map_jpg_data = new uint8_t[size];
  fread( map_jpg_data, sizeof(uint8_t), size, map_file_);
  uint32_t jpg_size = 0;
  uint8_t* jpg_data = JpgHandler( map_jpg_data, size, &jpg_size);
  HGE* hge = hgeCreate(HGE_VERSION);
  HTEXTURE map_texture = hge->Texture_Load( (const char*)jpg_data, jpg_size);
  delete map_jpg_data;
  delete jpg_data;

  uint32_t* map_pixel = reinterpret_cast<uint32_t*>(hge->Texture_Lock( big_map_texture_, false));
  uint32_t* map_block_pixel = reinterpret_cast<uint32_t*>(hge->Texture_Lock( map_texture));
  for( int k=0; k<240; k++) {
    memcpy( map_pixel+(y*240+k)*960+ x*320, map_block_pixel + k*320, sizeof( uint32_t)*320);
  }
  hge->Texture_Unlock( map_texture);
  hge->Texture_Unlock( big_map_texture_);
  hge->Texture_Free( map_texture);
  //读取CELL
  fread( &flag, sizeof(flag), 1, map_file_);
  fread( &size, sizeof(size), 1, map_file_);
  if( flag != 0x43454C4C) {
    //文件格式可能错误
  }
  for( int i=0; i<12; i++) {
    fread( &cell_data_[(y*12+i)*48+x*16], sizeof(uint8_t), 16, map_file_);
    //fread( cell_data_[y*12+i] + x*16, sizeof(uint8_t), 16, map_file_);
  }
}

void GUIMap::SetMask(const int x, const int y) {
  int x_pixel_position = x*320;
  int y_pixel_position = y*240;
  HGE* hge = hgeCreate( HGE_VERSION);
  mask_index_to_draw_.clear();
  for( int i=0; i<mask_count_; ++i) {
    if( mask_data_[i].start_x > x_pixel_position + 960)
      continue;
    if( mask_data_[i].start_y > y_pixel_position + 720)
      continue;
    if( (mask_data_[i].start_x + mask_data_[i].width )< x_pixel_position)
      continue;
    if( (mask_data_[i].start_y + mask_data_[i].height) < y_pixel_position)
      continue;
    mask_index_to_draw_.push_back( i);
  }
}

void GUIMap::LoadMap() {
  int map_x_sum = static_cast<int>(map_center_.GetX())/16-1;
  int map_y_sum = static_cast<int>(map_center_.GetY())/12-1;//计算地图序号
  if( map_x_sum_ == map_x_sum && map_y_sum_ == map_y_sum)
    return;
  HGE* hge = hgeCreate( HGE_VERSION);
  if( big_map_texture_ != 0) {
    hge->Texture_Free( big_map_texture_);
  }
  hge->Texture_Create( 960, 720);
  map_x_sum_ = map_x_sum;
  map_y_sum_ = map_y_sum;  
  for( int x = 0; x < 3; ++x) {
    for( int y = 0; y < 3; ++y) {
      if( map_y_sum + y < 0)
        continue;
      if( map_x_sum + x < 0)
        continue;
      if( map_y_sum + y >= map_block_row_count_)
        continue;
      if( map_x_sum + x >= map_block_col_count_)
        continue;
      ReadBlock( (map_y_sum_ + y)*map_block_col_count_ + map_x_sum_ + x, x, y);
    }
  }
  SetMask( map_x_sum_, map_y_sum_);
}

void GUIMap::Render( const Coordinate coordinate) {
  LoadMap();
  HGE* hge = hgeCreate( HGE_VERSION);
  float x_position = -( static_cast<int>(map_center_.GetX())%16 + map_center_.GetX() 
                                - static_cast<int>(map_center_.GetX())) * 20;//0,0号地图渲染位置
  float y_position = -( static_cast<int>(map_center_.GetY())%12 + map_center_.GetY()
                                - static_cast<int>(map_center_.GetY())) * 20;
  
  hgeSprite* sprite = new hgeSprite( big_map_texture_, 0, 0, 960, 720);
  sprite->Render( x_position, y_position);
  delete sprite;

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
  int map_x_sum = static_cast<int>(map_center_.GetX())/16-1;
  int map_y_sum = static_cast<int>(map_center_.GetY())/12-1;//计算地图序号
  int x_pixel_sum = map_x_sum*320;
  int y_pixel_sum = map_y_sum*240;
  
  uint32_t* map_pixel = reinterpret_cast<uint32_t*>(hge->Texture_Lock( big_map_texture_, true, 0, 0, 960, 720));

  for( std::list<int>::iterator iter = mask_index_to_draw_.begin(); 
        iter != mask_index_to_draw_.end(); ++iter) {
    if( map_pixel == NULL)
      break;
    MaskData mask_data = mask_data_[*iter];
    int offset_x = mask_data.start_x - x_pixel_sum;
    int offset_y = mask_data.start_y - y_pixel_sum;
    if( mask_data.start_x > map_center_.GetX()*20 + 320)
      continue;
    if( mask_data.start_y > map_center_.GetY()*20 + 240)
      continue;
    if( mask_data.start_x + mask_data.width < map_center_.GetX()*20-320)
      continue;
    if( mask_data.start_y + mask_data.height < map_center_.GetY()*20 - 240)
      continue;

    for( int x=0; x<mask_data.width; ++x) {
      if( offset_x + x >= 960)
        break;
      if( offset_x + x < 0)
        continue;
      for( int y=0; y<mask_data.height; ++y) {
        if( mask_data.data[ y*mask_data.width+x] == 0)
          continue;
        if( (offset_y + y) >= 720)
          break;
        if( (offset_y + y) < 0)
          continue;
        uint32_t color = map_pixel[ (offset_y + y)*960+offset_x +x];
        uint32_t rgb = color & 0x00FFFFFF;
        color = rgb + 0x8F000000;
        hge->Gfx_RenderLine( static_cast<float>(offset_x + x_position + x), 
                             static_cast<float>(offset_y + y_position + y), 
                             static_cast<float>(offset_x + x_position + x + 1 ), 
                             static_cast<float>(offset_y + y_position + y ), color);
      }
    }
  }

  hge->Texture_Unlock( big_map_texture_);

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
  else if( map_center_.GetX() >= map_max_.GetX() - 16)
    map_center_.SetX( map_max_.GetX() - 16);
  if( map_center_.GetY() < 12)
    map_center_.SetY( 12);
  else if( map_center_.GetY() >= map_max_.GetY() - 12)
    map_center_.SetY( map_max_.GetY() - 12);
}

void GUIMap::EventMouseLeftUp() {
  Coordinate mouse_position = g_gui_manage->GetMousePosition();
  float x_aim = (mouse_position.GetX() - 320)/20 + map_center_.GetX();
  float y_aim = (mouse_position.GetY() - 240)/20 + map_center_.GetY();
  g_game_logic->SetMapAim( Coordinate(x_aim, y_aim));
}

// 2字节高低位调换
void ByteSwap(WORD& value)
{
	WORD tempvalue = value >> 8;
	value = (value << 8) | tempvalue;
}

int GUIMap::DecompressMask( void* in, void* out)
{
  register BYTE *op;
	register  BYTE *ip;
	register unsigned t;
	register  BYTE *m_pos;

	op = (BYTE *) out;
	ip = (BYTE *) in;

	if (*ip > 17) {
		t = *ip++ - 17;
		if (t < 4)
			goto match_next;
		do *op++ = *ip++; while (--t > 0);
		goto first_literal_run;
	}

	while (1) {
		t = *ip++;
		if (t >= 16) goto match;
		if (t == 0) {
			while (*ip == 0) {
				t += 255;
				ip++;
			}
			t += 15 + *ip++;
		}

		* (unsigned *) op = * ( unsigned *) ip;
		op += 4; ip += 4;
		if (--t > 0)
		{
			if (t >= 4)
			{
				do {
					* (unsigned * ) op = * ( unsigned * ) ip;
					op += 4; ip += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *ip++; while (--t > 0);
			}
			else do *op++ = *ip++; while (--t > 0);
		}

first_literal_run:

		t = *ip++;
		if (t >= 16)
			goto match;

		m_pos = op - 0x0801;
		m_pos -= t >> 2;
		m_pos -= *ip++ << 2;

		*op++ = *m_pos++; *op++ = *m_pos++; *op++ = *m_pos;

		goto match_done;

		while (1)
		{
match:
			if (t >= 64)
			{

				m_pos = op - 1;
				m_pos -= (t >> 2) & 7;
				m_pos -= *ip++ << 3;
				t = (t >> 5) - 1;

				goto copy_match;

			}
			else if (t >= 32)
			{
				t &= 31;
				if (t == 0)	{
					while (*ip == 0) {
						t += 255;
						ip++;
					}
					t += 31 + *ip++;
				}

				m_pos = op - 1;
				m_pos -= (* ( unsigned short * ) ip) >> 2;
				ip += 2;
			}
			else if (t >= 16) {
				m_pos = op;
				m_pos -= (t & 8) << 11;
				t &= 7;
				if (t == 0) {
					while (*ip == 0) {
						t += 255;
						ip++;
					}
					t += 7 + *ip++;
				}
				m_pos -= (* ( unsigned short *) ip) >> 2;
				ip += 2;
				if (m_pos == op)
					goto eof_found;
				m_pos -= 0x4000;
			}
			else {
				m_pos = op - 1;
				m_pos -= t >> 2;
				m_pos -= *ip++ << 2;
				*op++ = *m_pos++; *op++ = *m_pos;
				goto match_done;
			}

			if (t >= 6 && (op - m_pos) >= 4) {
				* (unsigned *) op = * ( unsigned *) m_pos;
				op += 4; m_pos += 4; t -= 2;
				do {
					* (unsigned *) op = * ( unsigned *) m_pos;
					op += 4; m_pos += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *m_pos++; while (--t > 0);
			}
			else {
copy_match:
				*op++ = *m_pos++; *op++ = *m_pos++;
				do *op++ = *m_pos++; while (--t > 0);
			}

match_done:

			t = ip[-2] & 3;
			if (t == 0)	break;

match_next:
			do *op++ = *ip++; while (--t > 0);
			t = *ip++;
		}
	}

eof_found:
	//   if (ip != ip_end) return -1;
	return (op - (BYTE*)out);
}

// 地图的JPEG数据处理器
uint8_t* GUIMap::JpgHandler(uint8_t* Buffer, int inSize, uint32_t* outSize)
{
	// JPEG数据处理原理
	// 1、复制D8到D9的数据到缓冲区中
	// 2、删除第3、4个字节 FFA0
	// 3、修改FFDA的长度00 09 为 00 0C
	// 4、在FFDA数据的最后添加00 3F 00
	// 5、替换FFDA到FF D9之间的FF数据为FF 00

	int TempNum = 0;						// 临时变量，表示已读取的长度
	BYTE* outBuffer;
	BYTE* TempBuffer = new BYTE[inSize*2];		// 临时变量，表示处理后的数据
	memset(TempBuffer,0,inSize*2);
	outBuffer = TempBuffer;					// 已处理数据的开始地址
	WORD TempTimes = 0;					// 临时变量，表示循环的次数
	UINT Temp = 0;

	// 当已读取数据的长度小于总长度时继续
	while(TempNum < inSize && *Buffer++==0xFF)
	{
		*TempBuffer++=0xFF;
		TempNum++;
		switch(*Buffer)
		{
		case 0xD8:
			*TempBuffer++=0xD8;
			*Buffer++;
			TempNum++;
			break;
		case 0xA0:
			*Buffer++;
			TempBuffer--;
			TempNum++;
			break;
		case 0xC0:
			*TempBuffer++=0xC0;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序


			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}

			break;
		case 0xC4:
			*TempBuffer++=0xC4;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			break;
		case 0xDB:
			*TempBuffer++=0xDB;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			break;
		case 0xDA:
			*TempBuffer++=0xDA;
			*TempBuffer++=0x00;
			*TempBuffer++=0x0C;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序
			*Buffer++;
			TempNum++;
			*Buffer++;

			for(int i=2;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			*TempBuffer++=0x00;
			*TempBuffer++=0x3F;
			*TempBuffer++=0x00;
			Temp += 1; // 这里应该是+3的，因为前面的0xFFA0没有-2，所以这里只+1。

			// 循环处理0xFFDA到0xFFD9之间所有的0xFF替换为0xFF00
			for (;TempNum < inSize-2;)
			{
				if (*Buffer==0xFF)
				{
					*TempBuffer++=0xFF;
					*TempBuffer++=0x00;
					*Buffer++;
					TempNum++;	
					Temp ++;
				}
				else
				{
					*TempBuffer++=*Buffer++;
					TempNum++;
				}
			}
			// 直接在这里写上了0xFFD9结束Jpeg图片.
			Temp--; // 这里多了一个字节，所以减去。
			*TempBuffer--;
			*TempBuffer--=0xD9;
			break;
		case 0xD9:
			// 算法问题，这里不会被执行，但结果一样。
			*TempBuffer++=0xD9;
			TempNum++;
			break;
		default:
			break;
		}
	}
	Temp+=inSize;
	*outSize=Temp;
	return outBuffer;
}