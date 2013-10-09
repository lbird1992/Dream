#include <stdint.h>
#include <cstring>
#include "resource_manage.h"

ResourceManage* g_resource_manage;

Texture::Texture() : frame_count_(0), direction_count_(0), key_point_x_(0), key_point_y_(0) {
  memset( tex_, 0, sizeof(tex_));
}

Texture::~Texture() {

}

void Texture::Release() {
  HGE* hge = hgeCreate( HGE_VERSION);
  for( int i=0; i<frame_count_; i++)
    for( int j=0; j<direction_count_; j++)
      if( tex_[i][j] != 0) {
        hge->Texture_Free( tex_[i][j]);
        tex_[i][j] = 0;
      }
}

Texture ResourceManage::GetTex( const char* filename) {
  std::map< std::string, Texture>::iterator iter = resource_map_.find( filename);
  if( iter != resource_map_.end() )
  {
    return iter->second;
  }
  int len = strlen( filename);
  Texture tex;
  if( filename[len-1] == 's' && filename[len-2] == 'a' && filename[len-3] == 'w')
    tex.LoadWasFile( filename);
  else
    tex.LoadFile( filename);
  resource_map_[ filename] = tex;
  return tex;
}

void ResourceManage::DeleteTex( const char* filename) {
  std::map< std::string, Texture>::iterator it = resource_map_.find( filename);
  if( it == resource_map_.end()) // Don't have the file want to delete
    return;
  it->second.Release();
  resource_map_.erase( it);
}

void Texture::LoadFile( const char* filename) {
  HGE* hge = hgeCreate( HGE_VERSION);
  filename_ = filename;
  tex_[0][0] = hge->Texture_Load( filename);
  frame_count_ = 1;
  direction_count_ = 1;
  key_point_x_ = 0;
  key_point_y_ = 0;
}

void Texture::LoadWasFile( const char* filename) {
  //TODO: Think about the color pal in *.wpal files.
  HGE* hge = hgeCreate( HGE_VERSION);
  filename_ = filename;
  FILE* fp = fopen( filename,"rb");
  if( fp == NULL) {
    hge->System_Log( "Load the file %s failed. The file doesn't exist.", filename);
    return;
  }

  uint16_t file_flag;//which should be 'SP'
  fread( &file_flag, sizeof( file_flag), 1, fp);
  if( file_flag != 0x5053) {//'SP' 
    hge->System_Log( "Load the file %s as was format failed.", filename);
    return;
  }

  uint16_t file_head_length;//which should be 12
  fread( &file_head_length, sizeof( file_head_length), 1, fp);

  fread( &direction_count_, sizeof(direction_count_), 1, fp);
  fread( &frame_count_, sizeof(frame_count_), 1, fp);

  uint16_t was_width_all,was_height_all;//I don't really know where to use these two.o(¨s¡õ¨t)o
  fread( &was_width_all, sizeof(was_width_all), 1, fp);
  fread( &was_height_all, sizeof(was_height_all), 1, fp);
  fread( &key_point_x_, sizeof(key_point_x_), 1, fp);
  fread( &key_point_y_, sizeof(key_point_y_), 1, fp);

  WPixel color_board_from_file[256];
  fread( color_board_from_file, sizeof(color_board_from_file), 1, fp);//Read the pal.

  uint32_t* image_offset = new uint32_t[ direction_count_ * frame_count_];
  fread( image_offset, sizeof(uint32_t), direction_count_*frame_count_, fp);
  
  uint32_t offset_start = file_head_length + 4;//the point where image_offset equals 0
  for( int i=0; i<direction_count_; ++i) {
    for( int j=0; j<frame_count_; ++j) {
      //Jump to the frame head.
      fseek( fp, image_offset[ i*frame_count_ + j] + offset_start, SEEK_SET);

      int key_point_x, key_point_y, frame_width, frame_height;
      fread( &key_point_x, sizeof(key_point_x), 1, fp);
      fread( &key_point_y, sizeof(key_point_y), 1, fp);
      fread( &frame_width, sizeof(frame_width), 1, fp);
      fread( &frame_height, sizeof(frame_height), 1, fp);

      tex_[j][i] = hge->Texture_Create( frame_width, frame_height);
      uint32_t* pixels = (uint32_t*)hge->Texture_Lock( tex_[j][i], false, 
                                                     0, 0,//the coordinate of left-top 
                                                     frame_width, frame_height);

      uint32_t* line_offset = new uint32_t[frame_height];
      fread( line_offset, sizeof(*line_offset), frame_height, fp);
      WPixel pixel;

      for( int line = 0; line < frame_height; ++line) {
        fseek( fp, line_offset[line] + image_offset[ i*frame_count_ + j] + offset_start, SEEK_SET);
        int column = 0;
        uint8_t image_data;
        fread( &image_data, sizeof(image_data), 1, fp);
        while( image_data != 0x00 && column < frame_width) {
          if( (image_data & 0xE0) == 0x20) { //1~3 bit : 001
            uint8_t alpha = static_cast<int>(image_data & 0x1F) * 255 / 32;
            fread( &image_data, sizeof(image_data), 1, fp);
            pixel = color_board_from_file[image_data];
            pixels[ line * frame_width + column] = Pixel32( alpha, pixel).GetColor();
            ++column;
          }
          else if( (image_data & 0xE0) == 0x00) { // 1~3 bit: 000
            uint8_t pixel_count = image_data & 0x1F;
            uint8_t alpha;
            fread( &alpha, sizeof(alpha), 1, fp);
            alpha = static_cast<int>(alpha) * 255 / 31;
            fread( &image_data, sizeof(image_data), 1, fp);
            while( pixel_count > 0) {
              pixel = color_board_from_file[image_data];
              pixels[ line*frame_width + column] = Pixel32( alpha, pixel).GetColor();
              ++column;
              --pixel_count;
            }
          }
          else if( (image_data & 0xC0) == 0x40) { //1~2bit : 01
            uint8_t pixel_count = image_data & 0x3F;
            while( pixel_count > 0) {
              fread( &image_data, sizeof(image_data), 1, fp);
              pixel = color_board_from_file[image_data];
              pixels[ line*frame_width + column] = Pixel32(pixel).GetColor();
              ++column;
              --pixel_count;
            }
          }
          else if( (image_data & 0xC0) == 0x80) { //1~2 bit : 10
            uint8_t pixel_count = image_data & 0x3F;
            fread( &image_data, sizeof(image_data), 1, fp);
            pixel = color_board_from_file[image_data];
            Pixel32 pixel32(pixel);
            while( pixel_count > 0) {
              pixels[line*frame_width + column] = pixel32.GetColor();
              ++column;
              --pixel_count;
            }
          }
          else if( (image_data & 0xC0) == 0xC0) { // 1~2bit: 11
            uint8_t pixel_count = image_data & 0x3F;
            Pixel32 pixel32( 0, 0, 0, 0);
            while( pixel_count > 0) {
              pixels[line*frame_width + column] = pixel32.GetColor();
              ++column;
              --pixel_count;
            }
          }
          fread( &image_data, sizeof(image_data), 1, fp);
        }
        for( ; column < frame_width; ++column)
          pixels[ line * frame_width + column] = Pixel32( 0, 0, 0, 0).GetColor();
      }
      hge->Texture_Unlock( tex_[j][i]);
    }
  }
  fclose(fp);
}

void Texture::Render( Coordinate position, int frame, int direction) {
  HGE* hge = hgeCreate( HGE_VERSION);
  if( frame >= frame_count_) {
    hge->System_Log( "%s doesn't have %d frames, the max is %d.", 
                   filename_.c_str(), frame, frame_count_);
    return;    ]
  }
  if( direction >= direction_count_) {
    hge->System_Log( "%s doesn't have %d directions, the max is %d", 
                   filename_.c_str(), direction, direction_count_);
    return;
  }
  if( tex_[frame][direction] == 0) {
    hge->System_Log( "%s doesn't in the memory.", filename_.c_str());
    return;
  }
  int width = hge->Texture_GetWidth( tex_[frame][direction]);
  int height = hge->Texture_GetHeight( tex_[frame][direction]);
  hgeSprite* spr = new hgeSprite( tex_[frame][direction], 0, 0,//the left-top of texture.
                                static_cast<float>(width),
                                static_cast<float>(height));
  spr->Render( position.GetX(), position.GetY());
  delete spr;
}

void Texture::RenderEx( Coordinate destination_position, Coordinate source_position,
                      int width, int height, int frame, int direction) {
  HGE* hge = hgeCreate( HGE_VERSION);
  if( frame >= frame_count_) {
    hge->System_Log( "%s doesn't have %d frames, the max is %d.", 
                   filename_.c_str(), frame, frame_count_);
    return;
  }
  if( direction >= direction_count_) {
    hge->System_Log( "%s doesn't have %d directions, the max is %d", 
                   filename_.c_str(), direction, direction_count_);
    return;
  }
  if( tex_[frame][direction] == 0) {
    hge->System_Log( "%s doesn't in the memory.", filename_.c_str());
    return;
  }
  hgeSprite* spr = new hgeSprite( tex_[frame][direction],
                                source_position.GetX(),
                                source_position.GetY(),
                                static_cast<float>(width), static_cast<float>(height));
  spr->Render( destination_position.GetX(), destination_position.GetY());
  delete spr;
}

uint32_t Texture::GetColor( int frame, int direction, Coordinate position) {
  HGE* hge = hgeCreate( HGE_VERSION);
  if( frame >= frame_count_) {
    hge->System_Log( "%s doesn't have %d frames, the max is %d.", 
                   filename_.c_str(), frame, frame_count_);
    return 0;
  }
  if( direction >= direction_count_) {
    hge->System_Log( "%s doesn't have %d directions, the max is %d", 
                   filename_.c_str(), direction, direction_count_);
    return 0;
  }
  if( tex_[frame][direction] == 0) {
    hge->System_Log( "%s doesn't in the memory.", filename_.c_str());
    return 0;
  }
  int width = hge->Texture_GetWidth( tex_[frame][direction]);
  int height = hge->Texture_GetHeight( tex_[frame][direction]);
  if( position.GetX() >= width || position.GetY() >= height) {
    hge->System_Log( "GetColor function error with %s", filename_.c_str());
    return 0;
  }
  uint32_t* color_buffer = reinterpret_cast<uint32_t*>(hge->Texture_Lock( tex_[frame][direction]));
  uint32_t color = color_buffer[static_cast<int>(position.GetY())*width+static_cast<int>(position.GetX())];
  hge->Texture_Unlock( tex_[frame][direction]);
  return color;
}