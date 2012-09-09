//
//  physical_world_controller.cpp
//  PhysicalAnimation
//
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//  
//  Author: cosmobserver@gmail.com (Julian Wu )
//    9/9/12.

#include "physical_world_controller.h"

namespace physical_world {
  inline bool is_ball(const physical_objects::Object& obj) {
    return obj.object_type_id_ ==
              physical_objects::Object::ObjectType::SphericalObject;
  }
  
  template <class DoFVector, class MotionVector>
  void StandardWorldController<DoFVector, MotionVector>::next_step(){
    
    for (int i = 0; i < world.active_objects().size(); ++i ) {
      physical_objects::Object& curr_obj = this->world.active_objects().at(i);
      if ( is_ball( curr_obj ) ) {
        ball_moving()
      }
    }
    
  }
  
  template <class DoFVector, class MotionVector>
  void StandardWorldController<DoFVector, MotionVector>::
      ball_moving(physical_objects::SphericalObject& ball){
    float time_step = this->time_step;
    MotionVector 
  }
  
} // ns
