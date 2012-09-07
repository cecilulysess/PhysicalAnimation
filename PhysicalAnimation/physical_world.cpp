//
//  physical_world.cpp
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/4/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#include "physical_world.h"

namespace physical_world {

// world detail of Standard World
  
StandardWorld* StandardWorld::unique_instance_ = 0;
const float StandardWorld::g_ = 9.8;

StandardWorld* StandardWorld::Instance() {
  if (unique_instance_ == 0 ) {
    unique_instance_ = new StandardWorld();
  }
  return unique_instance_;
  
}

// ------------------------------
  template<class MotionVector>
  Air<MotionVector>::Air(float drag_coefficient, MotionVector speed) :
    drag_coefficient_(drag_coefficient), speed_(speed) {
      //do nothing
  }
  

} //ns physical_world