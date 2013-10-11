//
//The resource manage part of Dream Game
//		Code By L.Bird @ 2013-10-2
//
#ifndef _DREAM_RESOURCEMANAGE_H_
#define _DREAM_RESOURCEMANAGE_H_

#include <stdint.h>
#include <map>
#include <string>
#include "hge_include.h"

const int kMaxFrame = 20;//Max frames of all the was files.

//A group of HTEXTURE.
//Make it possible to get specific graphic resource handle with frame and direction.
class Texture {
public:

  Texture();
  ~Texture();

  void LoadFile( const char* filename);//For files in normal format.
  void LoadWasFile( const char* filename);//For files in was format.
  HTEXTURE GetTexture( const int frame, const int direction) {
    if( frame_count_ <= frame)
      return 0;
    if( direction_count_ <= direction)
      return 0;
    return tex_[frame][direction];
  }
  inline int16_t GetFrameCount() const{
    return frame_count_ + 1;
  }
  inline int16_t GetDirectionCount() const{
    return direction_count_ + 1;
  }
  inline int16_t GetKeyPointX() const {
    return key_point_x_;
  }
  inline int16_t GetKeyPointY() const {
    return key_point_y_;
  }
  inline int GetWidth( const int frame, const int direction) const {
    HGE* hge = hgeCreate( HGE_VERSION);
    return hge->Texture_GetWidth( tex_[frame][direction]);
  }
  inline int GetHeight( const int frame, const int direction) const {
    HGE* hge = hgeCreate( HGE_VERSION);
    return hge->Texture_GetHeight( tex_[frame][direction]);
  }
  void Release();

  void Render( const Coordinate position, const int frame, const int direction);
  void RenderEx( const Coordinate destination_position, 
               const Coordinate source_position, 
               const int width, const int height, 
               const int frame, const int direction);
  uint32_t GetColor( const int frame, const int direction, const Coordinate position);

private:
  std::string filename_;
  HTEXTURE tex_[kMaxFrame][8];
  int16_t frame_count_;
  int16_t direction_count_;
  int16_t key_point_x_;
  int16_t key_point_y_;
};

class ResourceManage {
public:
  void DeleteTex( const char* filename);
  Texture GetTex( const char* filename);
  
private:
  std::map< std::string, Texture> resource_map_; 
};

//Some struct which file format *.was may use.

//the pixel format which was files use.
struct WPixel {  
    union {  
		uint16_t color;
        struct {								// 16bit 565ģʽ
            uint16_t blue:5;                        
            uint16_t green:6;                       
            uint16_t red:5;                         
        };  
        struct {   
            uint8_t l;   
            uint8_t h;   
        };  
    };  
    WPixel() {}  
    WPixel(uint32_t c) {
      color=(uint16_t)((c>>8&0xf800)|(c>>5&0x7e0)|(c>>3&0x1f));
    }  
    WPixel(int r,int g,int b) : red(r>>3),green(g>>2),blue(b>>3) {}  
    operator uint32_t() const {
      return (color<<5&0x7fc00)|(color<<8&0xf80000)|((color<<3|(color&7))&0x3ff);
    }
};

//the 32 bit format of pixel.
//Use this to transcode from the 16-bit format pixel 
//to the format which HGE can use.
struct Pixel32 {
  uint8_t alpha;
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  Pixel32() {}
  Pixel32( WPixel p) {
    alpha = 255;
    red = p.red * 255 / 31;
    green = p.green * 255 / 63;
    blue = p.blue * 255 / 31;
  }
  Pixel32( uint8_t a, WPixel p) : alpha(a) {
    red = p.red * 255 / 31;
    green = p.green * 255 / 63;
    blue = p.blue * 255 / 31;
  }
  Pixel32( uint8_t a, uint8_t r, uint8_t g, uint8_t b) : alpha(a), red(r), green(g), blue(b) {}

  inline uint32_t GetColor() {
    return ARGB(alpha, red, green, blue);
  }
  inline uint8_t GetAlpha() {
    return alpha;
  }
  inline uint8_t GetRed() {
    return red;
  }
  inline uint8_t GetGreen() {
    return green;
  }
  inline uint8_t GetBlue() {
    return blue;
  }
};

extern ResourceManage* g_resource_manage;

#endif //_DREAM_RESOURCEMANAGE_H_