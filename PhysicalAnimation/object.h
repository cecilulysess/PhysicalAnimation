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

// debug
#include<stdio.h>
//----------
namespace physical_objects{
  
  typedef void (*DrawObject_collision)(int, void*);
  
  template<class MotionVector>
  class ball {
  public:
    ball(float raidus, float mass, float elasticity,
         float drag_coeff,
         MotionVector init_v,
         MotionVector init_a,
         MotionVector g,
         MotionVector init_loc,
         MotionVector medium_speed):
         radius_(raidus), mass_(mass), drag_coeff_(drag_coeff),
         elasticity_(elasticity), g_(g), velocity_(init_v),
         accel_(init_a), location_(init_loc) ,
         medium_speed_(medium_speed) {
           current_time_ = 0.0f;
           time_step_ = 0.1f;
    }
    
    void move(DrawObject_collision disp_func, float time_step) {
      if (location_.x < 0 || location_.x > 1000
          || location_.y < 0 || location_.y > 1000 )
        return;
      this->time_step_ = time_step;
      
      MotionVector tmp_a, tmp_v, tmp_x;
      
      MotionVector tmp_f = (mass_ * g_ - drag_coeff_ * (velocity_ - medium_speed_));
      
      printf("\tRelative Speed: %f, %f \n",
             tmp_f.x, tmp_f.y);
      
      tmp_a =  tmp_f / mass_;
      printf("\ttmp_a: (%f, %f)\n", tmp_a.x, tmp_a.y);
      disp_func(0, this);
      
      tmp_v = velocity_ + accel_ * time_step;
      tmp_x = location_ + velocity_ * time_step;
      // if collison do sth
      
      
      
      printf("current time: %f, location: (%f, %f), V: (%f, %f), A: (%f, %f) \n",
             current_time_,
             location_.x, location_.y,
             velocity_.x, velocity_.y,
             accel_.x, accel_.y);
      
      velocity_ = tmp_v;
      location_ = tmp_x;
      accel_ = tmp_a;
      current_time_ += time_step;
    }
    
    float radius() {return this->radius_;}
    float mass() {return this->mass_;}
    float elasticity() {return this->elasticity_;}
    float drag_coeff() {return this->drag_coeff_;}
    MotionVector& g() {return this->g_;}
    MotionVector& velocity() {return this->velocity_;}
    MotionVector& acceleration() {return this->accel_;}
    MotionVector& location() {return this->location_;}
    
  private:
    float radius_, mass_, elasticity_, drag_coeff_;
    MotionVector g_, velocity_, accel_, location_, medium_speed_;
    float current_time_, time_step_;
    
    
  };
}//ns pobj

#endif /* defined(__PhysicalAnimation__object__) */
