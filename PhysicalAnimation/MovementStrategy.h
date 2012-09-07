//
//  MovementStrategy.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/7/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef PhysicalAnimation_IMovementStrategy_h
#define PhysicalAnimation_IMovementStrategy_h

#include "physical_world.h"
#include "objects.h"

namespace motion_strategies {
  //  define a func pointer that used for call back with parameter that
  //  tells whether collison
  typedef void (*DrawObjectIfCollision) (int);
  
  // strategy that responsible for movement, I avoid the approach that using
  // template as strategy so that avoid the additional complex
  template <class MotionVector>
  class IMovementStrategy {
  public:
    virtual void moving(physical_world::World world,
                        physical_objects::Object object2move,
                        DrawObjectIfCollision display,
                        float start_time,
                        float time_step, float end_time) = 0;
    
    virtual MotionVector V_obj() = 0;
    virtual MotionVector A_obj() = 0;
    virtual MotionVector X_obj() = 0;
    virtual physical_world::Air<MotionVector> medium() = 0;
  };
  
  template <class MotionVector>
  class StandardSphericalMovement {
  public:
    StandardSphericalMovement(MotionVector V_obj_init,
                              MotionVector A_obj_init, MotionVector X_obj_init,
                              physical_world::Air<MotionVector> medium );
    void moving(physical_world::World world,
                physical_objects::Object object2move,
                DrawObjectIfCollision display,
                float start_time,
                float time_step, float end_time);
    
    MotionVector V_obj() { return this->V_obj_; }
    MotionVector A_obj() { return this->A_obj_; }
    MotionVector X_obj() { return this->X_obj_; }
    physical_world::Air<MotionVector> medium() { return this->medium_; }
    
  private:
    MotionVector V_obj_, A_obj_, X_obj_;
    physical_world::Air<MotionVector> medium_;
  };
}// ns motion_strategies

#endif
