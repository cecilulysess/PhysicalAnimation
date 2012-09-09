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
  template <class LocationVector, class MotionVector>
  void BallMovementController<LocationVector, MotionVector>::next_step(){
    if ( time - 0.0f < CAL_EPS ) {
      movement_strategy
    }
  }
}