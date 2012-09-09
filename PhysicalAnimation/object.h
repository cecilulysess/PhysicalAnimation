//
//  object.h
//  PhysicalAnimation
//
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//  
//  Author: cosmobserver@gmail.com (Julian Wu )
//    9/9/12.

#ifndef __PhysicalAnimation__object__
#define __PhysicalAnimation__object__

namespace physical_objects{
  
  
  template<class MotionVector>
  class ball {
    ball(float raidus, float mass, float elasticity, float g,
         float drag_coeff, MotionVector init_v, MotionVector init_a,
         MotionVector init_loc): radius_(radius), mass_(mass),
         elasticity_(elasticity), g_(g), velocity_(init_v),
         accel_(init_a), location_(init_loc) {
           
    }
    
    void move()
    
    float radius() {return this->radius_;}
    float mass() {return this->mass_;}
    float elasticity() {return this->elasticity_;}
    float g() {return this->g_;}
    float drag_coeff() {return this->drag_coeff_;}
    MotionVector velocity() {return this->velocity_;}
    MotionVector acceleration() {return this->accel_;}
    MotionVector location() {return this->location_;}
    
  private:
    float radius_, mass_, elasticity_, g_, drag_coeff_;
    MotionVector velocity_, accel_, location_;
    
    
  };
}//ns pobj

#endif /* defined(__PhysicalAnimation__object__) */
