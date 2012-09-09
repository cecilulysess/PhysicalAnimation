//
//  objects.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/4/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef PhysicalAnimation_objects_h
#define PhysicalAnimation_objects_h

#include<limits>


namespace physical_objects {


//  Object abstract class that represent all objects in the physical world
//  the elasticity coeffcient should between (0.0, 1.0)
class Object {
public:
  enum ObjectType { UnAccess = 0, SphericalObject = 11, };
  
  Object();
  
  // a const identifier number of the object, every object should have one
  // permenant identifier 
  const long identifier() {
    return this->identifier_;
  }
  
  virtual float mass() = 0;
  // elasticity coeffieicnt 
  virtual float elasticity() = 0;
  
  static const ObjectType object_type_id_;
private:
  const long identifier_;
};


//  Particle objects that havs no shape and be totally rigid
class ParticleObject : Object {
public:
  virtual float mass() = 0;
  virtual float elasticity() = 0;
};

// An abstract class that represent shperical object with radius
class SphericalObject : public Object {
public:
  virtual float mass() = 0;
  virtual float elasticity() = 0;
  virtual float radius() = 0;
};


// A 2D ball
class Ball2D: public SphericalObject {
public:
  Ball2D(float mass, float elasticity, float radius ) ;
  
  float mass() {
    return this->mass_;
  }
  float elasticity() {
    return this->elasticity_;
  }
  
  float radius() {
    return this->radius_;
  }
  
  
  static const ObjectType object_type_id_;
private:
  float mass_, elasticity_, radius_;
  
};
  
//  An immortal object
class ImmortalObject: Object {
public:
  float mass() {
    return this->mass_;
  }
  float elasticity() {
    return this->elasticity_;
  }
  
private:
  float mass_ = std::numeric_limits<float>::max(),
        elasticity_ = 1.0f;
};

} //ns physical_objects



#endif
