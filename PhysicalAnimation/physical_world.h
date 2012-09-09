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
#include<vector>
#include<stdexcept>

#include"objects.h"

namespace physical_world {

//  The abstract class represent the world, DoFVector
//  is the vector used to store the Degree of Freedom of objects
//  in this world. In this stage, it would be Vector3D
template <class MotionVector,class DoFVector>
class World {
public:
  // return the g value in this world
  virtual float g() = 0;
  // a map of objects that store it's identifier() and location in the world
  const std::map<long, DoFVector>& objects_location() {
    return this->objects_location_ ;
  }
  std::vector<physical_objects::Object<MotionVector, DoFVector>>&
    active_objects() {
    return this->active_objects_;
  }
  const std::vector<physical_objects::Object<MotionVector, DoFVector>>&
     immortal_objects() {
    return this->immortal_objects_;
  }
  
  
  void add_object_location(long identifier, DoFVector dof_vector) {
    this->objects_location_[identifier] = dof_vector;
  }
  
  DoFVector& get_object_location(long identifier) {
    typename std::map<long, DoFVector>::iterator it =
      this->objects_location_.find(identifier);
    
    if (it == objects_location_.end() ){
      throw std::invalid_argument("Object not found in this world");
    } else {
      return it->second;
    }
  }
private:
  std::map<long, DoFVector> objects_location_;
  std::vector<physical_objects::Object<MotionVector, DoFVector>>
    active_objects_, immortal_objects_;
};
  
//  Standard World is the stand world that in the earth with g = 9.8
template <class MotionVector,class DoFVector>
class StandardWorld : public World<MotionVector, DoFVector>{
public:
  static StandardWorld* Instance();
  float g() {
    return g_;
  }
private:
  static StandardWorld* unique_instance_;
  static const float g_ = 9.8f;
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
class Air : public Medium<MotionVector> {
public:
  Air(float drag_coefficient, MotionVector speed) :
    drag_coefficient_(drag_coefficient), speed_(speed) {
    //do nothing
  }
  float drag_coefficient() {return this->drag_coefficient_;}
  MotionVector speed() {return this->speed_;}
private:
  float drag_coefficient_;
  MotionVector speed_;
};
  
} //ns physical_world

#endif
