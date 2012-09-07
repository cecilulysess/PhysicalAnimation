//
//  physical_world.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/4/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef PhysicalAnimation_physical_world_h
#define PhysicalAnimation_physical_world_h

namespace physical_world {

//  The abstract class represent the world
class World {
public:
  // return the g value in this world
  virtual float g() = 0;
};
  
//  Standard World is the stand world that in the earth with g = 9.8
class StandardWorld : World{
public:
  static StandardWorld* Instance();
  float g() {
    return this->g_;
  }
  
private:
  static StandardWorld* unique_instance_;
  static const float g_;
};
  
// An abstract class represent the medium that filling some part of the world
template <class MotionVector>
class Medium{
public:
  virtual float drag_coefficient() = 0;
  virtual MotionVector speed() = 0;
};

  // Normal Air with drag_coefficient and speed that represent wind
template <class MotionVector>
class Air : Medium<MotionVector> {
public:
  Air(float drag_coefficient, MotionVector speed);
  float drag_coefficient() = 0;
  MotionVector speed() = 0;
private:
  float drag_coefficient_;
  MotionVector speed_;
};
  
} //ns physical_world

#endif
