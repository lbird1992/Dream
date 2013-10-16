#ifndef _DREAM_GUIANIMATION_
#define _DREAM_GUIANIMATION_

#include "gui_base.h"

enum AnimationType {
  ONCE = 1,//只播放一次
  LOOP = 2//循环播放
};

const int kFrameFill = 8;

class GUIAnimation : public GUI {
public:
  GUIAnimation( const char* filename, const Coordinate position, const GUI* father,
                const AnimationType type);

  virtual void Render( const Coordinate position);

  inline int GetFrame() const {
    return frame_;
  }
  inline void SetFrame( const int frame) {
    frame_ = frame;
  }
  inline int GetDirection() const {
    return direction_;
  }
  inline void SetDirection( const int direction) {
    direction_ = direction;
  }
  virtual void EventMouseLeftUp() {
    //TODO: 加入对动画的点击信息
  }
  inline void SetEnd() {
    is_end_ = true;
  }
  inline bool IsEnd() const {
    return is_end_;
  }

  static float GetSpeed() const {
    return speed_;
  }

  static void SetSpeed(const float speed){
     speed_ = speed;
  }

protected:
  AnimationType animation_type_;
  bool is_end_;//此值只对type==LOOP 情况有效
  int frame_;
  float frame_fill_;
  int direction_;

  static float speed_;
};

#endif