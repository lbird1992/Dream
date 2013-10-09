#ifndef _DREAM_HGEINCLUDE_H_
#define _DREAM_HGEINCLUDE_H_


#include "hgeLib\include\hge.h"
#include "hgeLib\include\hgeanim.h"
#include "hgeLib\include\hgecolor.h"
#include "hgeLib\include\hgedistort.h"
#include "hgeLib\include\hgefont.h"
#include "hgeLib\include\hgegui.h"
#include "hgeLib\include\hgeguictrls.h"
#include "hgeLib\include\hgeparticle.h"
#include "hgeLib\include\hgerect.h"
#include "hgeLib\include\hgeresource.h"
#include "hgeLib\include\hgesprite.h"
#include "hgeLib\include\hgestrings.h"
#include "hgeLib\include\hgevector.h"

class Coordinate{
public:
  Coordinate() : x_(0), y_(0) {}
  Coordinate(float x, float y) : x_(x), y_(y) {}

  inline float GetX() {
    return x_;
  }
  inline float GetY() {
    return y_;
  }
  Coordinate Plus(Coordinate c) {
    return Coordinate( x_+c.GetX(), y_+c.GetY()); 
  }
  Coordinate Minus(Coordinate c) {
    return Coordinate( x_-c.GetX(), y_-c.GetY());
  }

private:
  float x_;
  float y_;
};

#endif //_DREAM_HGEINCLUDE_H_