//
//  physical_world_controller.h
//  PhysicalAnimation
//
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//  
//  Author: cosmobserver@gmail.com (Julian Wu )
//    9/9/12.

#ifndef __PhysicalAnimation__physical_world_controller__
#define __PhysicalAnimation__physical_world_controller__

#define CAL_EPS 0.00000001f

#include "physical_world.h"
#include "MovementStrategy.h"

namespace physical_world {
  
  template <class LocationVector, class MotionVector>
  class BallMovementController {
  public:
    BallMovementController(World<LocationVector> world,
                           LocationVector init_loc,
                           MotionVector init_speed,
                           MotionVector init_accel,
                           Medium<MotionVector> medium,
                           float time_step) :
        movement_strategy(init_speed, init_accel, init_loc, medium),
        time_step(time_step),
        world(world)
    {
      time = 0.0f;
    }
    // get the next step of all objects in this world
    void next_step();
  private:
    physical_world::World<LocationVector> world;
    motion_strategies::StandardSphericalMovement<MotionVector, LocationVector>
      movement_strategy;
    float time, time_step;
  };
} // ns physical_world


#endif /* defined(__PhysicalAnimation__physical_world_controller__) */
