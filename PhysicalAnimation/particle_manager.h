//
//  particle_manager.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/23/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef __PhysicalAnimation__particle_manager__
#define __PhysicalAnimation__particle_manager__

#include <iostream>
#include <vector>
#include <queue>

#include "object.h"
#include "Vector.h"

#define PARTICLE_AGE 10.0f

namespace particle_manager {
  //particle manager, maintained particle_size particles showed at the same
  // time
  class ParticleManager {
  public:
    
    ParticleManager(int particle_size, Vector3d generation_plane_origin,
                    float generation_plane_width, float generation_plane_height,
                    Vector3d generation_direction,
                    double speed_mean,
                    double speed_var) :
      particle_size_(particle_size), speed_mean_(speed_mean),
      speed_var_(speed_var), gen_pl_origin_(generation_plane_origin),
      gen_pl_height_(generation_plane_height),
      gen_pl_width_(generation_plane_width) {
        
        this->generation_plane_[0] =
          generation_plane_origin;
        this->generation_plane_[1] =
          generation_plane_origin + Vector3d(0.0, generation_plane_height, 0.0);
        this->generation_plane_[2] =
        generation_plane_origin + Vector3d(generation_plane_width,
                                           generation_plane_height, 0.0);
        this->generation_plane_[3] =
        generation_plane_origin + Vector3d(generation_plane_width, 0.0, 0.0);

        Ux = Vector3d(1.0, 0.0, 0.0);
        Uy = Vector3d(0.0, 1.0, 0.0);
        
        init();
    }
    
    
    void move_particles(float time_step);
    
    Vector3d* generation_plane() {
      return this->generation_plane_;
    }
    int particle_size(){
      return this->particle_size_;
    }
    
    std::vector<physical_objects::Particle*>& particles(){
      return particles_;
    }
    
    
  private:
    // randomly generate particles
    void init();
    
    // internal func to calculate init location
    Vector3d get_init_loc(Vector3d& gen_pl_orig) ;
    
    int particle_size_;
    double speed_mean_, speed_var_;
    Vector3d gen_dir_;
    Vector3d generation_plane_[4];
    float gen_pl_width_, gen_pl_height_;
    Vector3d Ux, Uy;
    Vector3d gen_pl_origin_;
    std::vector<physical_objects::Particle*> particles_;
    std::queue<physical_objects::Particle*> dead_particles_;
  };
  
  
}// ns particle_manager

#endif /* defined(__PhysicalAnimation__particle_manager__) */
