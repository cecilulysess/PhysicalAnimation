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
  template <class MotionVector, class DoFVector>
  inline bool is_ball(
    const physical_objects::Object<MotionVector, DoFVector>& obj) {
    return obj.object_type_id_ ==
              physical_objects::Object<MotionVector, DoFVector>::
              ObjectType::SphericalObject;
  }
  
  template <class MotionVector, class DoFVector>
  void StandardWorldController<MotionVector, DoFVector>::next_step(){
    
    for (int i = 0; i < world.active_objects().size(); ++i ) {
      physical_objects::Object<MotionVector, DoFVector>& curr_obj = this->world.active_objects().at(i);
      if ( is_ball( curr_obj ) ) {
//        ball_moving();
      }
    }
    
  }
  
  template <class MotionVector, class DoFVector>
  void StandardWorldController<MotionVector, DoFVector>::
      ball_moving(
        physical_objects::SphericalObject<MotionVector, DoFVector>& ball){
    float time_step = this->time_step;
//    MotionVector 
  }
  
} // ns
