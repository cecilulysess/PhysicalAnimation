//
//  physical_object.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 11/25/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef __PhysicalAnimation__physical_object__
#define __PhysicalAnimation__physical_object__

#include <vector>
#include "Vector.h"


namespace physical_objects{
  
  // StateVector store value as Vector3d or 4d in its own terms.
  // This is collaboration with the NumericalIntegrator
  class StateVector {
    
    
  };

  // a particle in the particle system
  typedef struct Particle {
    Particle(float x, float y, float z,
             float vx, float vy, float vz,
             float m, bool is_pivot){
      this->x = Vector3d(x, y, z);
      this->v = Vector3d(vx, vy, vz);
      this->m = m;
      this->f = Vector3d(0,0,0);
      this->is_pivot = is_pivot;
      N = 0;
      connected_particles = std::vector<Particle*>();
    }
    
    //locaiton in space
    Vector3d x;
    // speed in space
    Vector3d v;
    // force applied to this particle
    Vector3d f;
    // mass;
    float    m;
    // is static particle that never applied any force
    bool     is_pivot;
    
    std::vector<Particle*> connected_particles;
    // number of particles
    int N;
    
  } Particle;
  
  // BouncingMesh is a bouncing object that bounce everything from its surface
  class BouncingMesh {
  public:
    // construct a new bouncing mesh with upper-left point (x, y, z) and
    // normal as (0, 1, 0);
    BouncingMesh(float x, float y, float z,
                 float width, float height, int division,
                 float mass);
    ~BouncingMesh();
    
    // clear the force for all particle
    void clear_force();
    
    //get the state vector of this object
    StateVector& state_vector();
    
    const std::vector<Particle>& mesh_particles();
    
  private:
    //create springs when get all particle setted
    void create_springs();
    StateVector state_vector_;
    std::vector<Particle> mesh_particles_;
    // number of particles
    int N;
    // array_width
    int array_width;
    
  };
  
  class NumericalIntegrator {
  public:
    void Integrate(StateVector& sv, float deltaT);
  };

}//ns physical_objects
#endif /* defined(__PhysicalAnimation__physical_object__) */
