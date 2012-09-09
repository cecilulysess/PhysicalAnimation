//
//  MovementStrategy.cpp
//  PhysicalAnimation
//
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//  
//  Author: cosmobserver@gmail.com (Julian Wu )
//    9/7/12.

#include "MovementStrategy.h"
#include "objects.h"

namespace motion_strategies {
  template <class MotionVector, class DoFVector>
  StandardSphericalMovement<MotionVector, DoFVector>::
  StandardSphericalMovement(MotionVector V_obj_init,
    MotionVector A_obj_init, MotionVector X_obj_init,
    physical_objects::SphericalObject
                            <MotionVector, DoFVector>* object,
    physical_world::Medium<MotionVector>& medium ):
    V_obj_(V_obj_init),A_obj_(A_obj_init),
    X_obj_(X_obj_init), medium_(medium), object_(object)
  {
    
  }
  
  template <class MotionVector, class DoFVector>
  void StandardSphericalMovement<MotionVector, DoFVector>::moving(
    physical_world::World<MotionVector, DoFVector>& world,
    float start_time,
    float time_step) {
    
    float t = start_time;
    
    MotionVector tmp_a, tmp_v, tmp_x;
    
    tmp_a = world.g() - medium().drag_coefficient() / this->object_.mass() *
             (medium().speed() - V_obj_) ;
    //display(0, (physical_objects::SphericalObject<MotionVector, DoFVector>&)object2move);
    
    tmp_v = V_obj_ + A_obj_ * time_step;
    tmp_x = X_obj_ + V_obj_ * time_step;
    //if collison do sth
    
    
    V_obj_ = tmp_v;
    X_obj_ = tmp_x;
    A_obj_ = tmp_a;
    t += time_step;
    
  }
}