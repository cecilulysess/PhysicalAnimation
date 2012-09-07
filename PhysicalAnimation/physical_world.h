//
//  physical_world.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/4/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef PhysicalAnimation_physical_world_h
#define PhysicalAnimation_physical_world_h

#include<map>

#include"objects.h"

namespace physical_world {

//  The abstract class represent the world, DoFVector
//  is the vector used to store the Degree of Freedom of objects
//  in this world. In this stage, it would be Vector3D
template <class DoFVector>
class World {
public:
  // return the g value in this world
  virtual float g() = 0;
  virtual const std::map<physical_objects::Object, DoFVector> objects() = 0;
};
  
//  Standard World is the stand world that in the earth with g = 9.8
template <class DoFVector>
class StandardWorld : public World<DoFVector>{
public:
  static StandardWorld* Instance();
  float g() {
    return this->g_;
  }
  const std::map<physical_objects::Object, DoFVector> objects() {
    return objects_;
  }
private:
  static StandardWorld* unique_instance_;
  static const float g_;
  static const std::map<physical_objects::Object, DoFVector> objects_;
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
