//
//  object.cpp
//  PhysicalAnimation
//
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//  
//  Author: cosmobserver@gmail.com (Julian Wu )
//    9/9/12.

#include "object.h"

namespace physical_objects{

  // external definition of template is unsupported
//  template<class MotionVector>
//  void ball<MotionVector>::move(DrawObject_collision disp_func,
//                                float time_step) {
//    this->time_step_ = time_step;
//    
//    MotionVector tmp_a, tmp_v, tmp_x;
//    
//    tmp_a = g() - drag_coeff() / mass() *
//            medium_speed_ - velocity_ ;
//    disp_func(0, this);
//    
//    tmp_v = velocity_ + accel_ * time_step;
//    tmp_x = location_ + velocity_ * time_step;
//    //if collison do sth
//    
//    
//    velocity_ = tmp_v;
//    location_ = tmp_x;
//    accel_ = tmp_a;
//    current_time_ += time_step;
//  }

} //ns