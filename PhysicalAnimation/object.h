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
#include<vector>
#include"Vector.h"

//----------
namespace physical_objects{
  // a box that is the obstacle in this scene
  template<class VerticeVector>
  class box {
  public:
    box(std::vector<VerticeVector>& vertices):vertices_(vertices) {}
    
    const std::vector<VerticeVector>& vertices() {
      return this->vertices_;
    }
    
    
  private:
    std::vector<VerticeVector>& vertices_;
  };

  
  
  //----------------------------------------------------------------
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
           rest = false;
    }
    
    // responsible for the motion of this ball
    void move(DrawObject_collision disp_func, float time_step,
              box<MotionVector> in_box) {
//      if (location_.x < 0 || location_.x > 1000
//          || location_.y < 0 || location_.y > 1000 )
//        return;
      if (rest) return;
      this->time_step_ = time_step;
      
      MotionVector tmp_a, tmp_v, tmp_x, norm;
      
      MotionVector tmp_f = (mass_ * g_ - drag_coeff_ * (velocity_ - medium_speed_));
      
      
      tmp_a =  tmp_f / mass_;
      tmp_v = velocity_ + accel_ * time_step;
      tmp_x = location_ + velocity_ * time_step;
      if (is_collision(location_, tmp_x, in_box, norm)) {
        // if the time_step is small enough, then there is no need to
        // calculate the collision time fraction
        
        velocity_ = reflect(velocity_, norm, elasticity_);
        tmp_f = (mass_ * g_ - drag_coeff_ * (velocity_ - medium_speed_));
        tmp_a = tmp_f /mass_;
        location_ = location_ + velocity_ * time_step;
        if ( velocity_.norm() < 0.9 && location_.y + 1 - radius_< 0.01){
          
          velocity_.x = 0.0f;
          velocity_.y = 0.0f;
          rest = true;
        }
        disp_func(1, this);
      } else {
        velocity_ = tmp_v;
        location_ = tmp_x;
        accel_ = tmp_a;
        current_time_ += time_step;
        disp_func(0, this);
      }
            
      
      
      printf("current time: %f, location: (%f, %f), V: (%f, %f), A: (%f, %f) \n",
             current_time_,
             location_.x, location_.y,
             velocity_.x, velocity_.y,
             accel_.x, accel_.y);
      
      
    }
    
    // return whether the movement result in collison with the in_box
    bool is_collision(MotionVector& from, MotionVector& to,
                      box<MotionVector> in_box , MotionVector& collision_plane_norm) {
      //fake it to static box
      float max_x = 0.0f, min_x = 98654321.0f,
            max_y = 0.0f, min_y = 98654321.0f,
            max_z = 0.0f, min_z = 98654321.0f;
      
      for ( int i = 0 ; i < in_box.vertices().size() ; ++i ) {
        if ( in_box.vertices()[i].x > max_x ) {
          max_x = in_box.vertices()[i].x;
        }
        if ( in_box.vertices()[i].y > max_y ) {
          max_y = in_box.vertices()[i].y;
        }
        if ( in_box.vertices()[i].z > max_z ) {
          max_z = in_box.vertices()[i].z;
        }
        if ( in_box.vertices()[i].x < min_x ) {
          min_x = in_box.vertices()[i].x;
        }
        if ( in_box.vertices()[i].y < min_y ) {
          min_y = in_box.vertices()[i].y;
        }
        if ( in_box.vertices()[i].z < min_z ) {
          min_z = in_box.vertices()[i].z;
        }
      }
      max_x -= radius_;
      max_y -= radius_;
      max_z -= radius_;
      min_x += radius_;
      min_y += radius_;
      min_z += radius_;
      
      if ( to.x < max_x && to.x > min_x && to.y <max_y && to.y > min_y &&
          to.z > min_x && to.z < max_x ){
        return false;
      } else {
        if (to.x < min_x) {
          collision_plane_norm.x = 1;
          collision_plane_norm.y = 0;
          collision_plane_norm.z = 0;
        }
        if (to.x > max_x) {
          collision_plane_norm.x = -1;
          collision_plane_norm.y = 0;
          collision_plane_norm.z = 0;
        }
        if (to.y < min_y) {
          collision_plane_norm.x = 0;
          collision_plane_norm.y = 1;
          collision_plane_norm.z = 0;
        }
        if (to.y > max_y) {
          collision_plane_norm.x = 0;
          collision_plane_norm.y = -1;
          collision_plane_norm.z = 0;
        }
        if (to.z < min_z ) {
          collision_plane_norm.x = 0;
          collision_plane_norm.y = 0;
          collision_plane_norm.z = 1;
        }
        if (to.z > max_y ) {
          collision_plane_norm.x = 0;
          collision_plane_norm.y = 0;
          collision_plane_norm.z = -1.0;
        }
        return true;
      }
    }
    
    MotionVector reflect(MotionVector speed,
                         MotionVector plane_norm,
                         float elasticity_coefficient) {
      //printf("\t test: (%f, %f)", (speed%plane_norm).x, (speed%plane_norm).y);
      MotionVector b = - (speed * plane_norm) * plane_norm;
      printf( "\t Reflect: speed(%f, %f, %f), norm(%f, %f, %f), b(%f, %f, %f)\n",
             speed.x, speed.y, plane_norm.x, plane_norm.y, plane_norm.z, b.x, b.y, b.z);
      return speed + (1 + elasticity()) * b;
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
    bool rest;
    
  };
  
  
  //-------------------Particle system elements---------------------------------
  class Particle {
  public:
    
    Particle(const Vector4d& color, Vector3d loc, Vector3d speed,
             float max_age ) :
      color_(color), location_(loc), velocity_(speed) , max_age_(max_age){
        is_alive = true;
        mass_ = 0.1;
        g_ = Vector3d(0.0, -0.98, 0.0);
        drag_coeff_ = 1.0;
        elasticity_ = 1.0;
        medium_speed_  = Vector3d(0.0, 0.0, 0.0);
        age_ = 0.0f;
    }
    
    // responsible for the motion of this ball
    void move( float time_step, Vector3d& center, float obsrad
              ) {
      if (is_alive) {
        age_ += time_step;
        if ( age_ < 0.0f ) {
          return;
        }
        if ( age_ > max_age_ ) {
          is_alive = false;
        }
        this->time_step_ = time_step;
        
        Vector3d tmp_a, tmp_v, tmp_x, norm;
        
        Vector3d tmp_f =(mass_ * g_);
        if ( mass() < 0.1 ) {
          tmp_f = Vector3d(0);
        }
        //(mass_ * g_ - drag_coeff_ * (velocity_ - medium_speed_));
        
//        printf("F: %f, %f, %f", tmp_f.x, tmp_f.y, tmp_f.z);
        tmp_a =  tmp_f / mass_;
        tmp_v = velocity_ + tmp_a * time_step;
        tmp_x = location_ + velocity_ * time_step;
        
        // collision with a sphere
        if ( (tmp_x - center).norm() <  obsrad ) {
//          printf("collision\n");
          tmp_v = reflect_sphere(tmp_x, tmp_v, center, obsrad);
          if ( (rand() % 1000) < 100 ) {
            mass_ = 0.01;
            tmp_v = tmp_v / 1.5 ;
          }
        }
        
        velocity_ = tmp_v;
        location_ = tmp_x;
//        printf("th part vol: %f, %f, %f \n",
//               this->velocity().x,
//               this->velocity().y,
//               this->velocity().z);
      }
      
    }
    
    // change the speed of this particle
    Vector3d& reflect(Vector3d speed,
                     Vector3d plane_norm,
                     float elasticity_coefficient) {
      return velocity();
    }
    
    //return next_v after reflected and modify next_x
    Vector3d reflect_sphere(Vector3d& next_x, Vector3d& next_v,
                             Vector3d& center, float radius) {
      Vector3d xr_dir = (next_x - center) / (next_x - center).norm();
      
      next_x = center + radius * xr_dir;
      return xr_dir * next_v.norm() * 0.5;
    }
    
    
    //color can be changed
    Vector4d& get_color() { return color_; }
    float mass() {return this->mass_;}
    void set_mass(float mass) { this->mass_ = mass; }
    float elasticity() {return this->elasticity_;}
    float drag_coeff() {return this->drag_coeff_;}
    Vector3d& g() {return this->g_;}
    Vector3d& velocity() {return this->velocity_;}
    Vector3d& location() {return this->location_;}
    // control whether this particle is still alive
    bool is_alive;
    
    float age_;
  private:
    Vector4d color_;
    Vector3d location_;
    float mass_, elasticity_, drag_coeff_;
    Vector3d g_, velocity_,  medium_speed_;
    bool rest;
    float current_time_, time_step_;
    float max_age_;
  };
  
  // verticles that stands for interaction vertices as struts
  class Struts_Vertice {
  public:
    Struts_Vertice(float mass_, Vector3d force_, Vector3d location_,
                   Vector3d velocity_) :
      mass(mass_), location(location_), velocity(velocity_), force(force_){
      
    }
    float mass;    Vector3d location;
    Vector3d velocity;
    Vector3d force;
    
    
  };
  
  class Strut {
  public:
    Strut(float spring, float damper, float original_len) :
      K(spring), D(damper), L0(original_len) {
    
    }
    float K, D, L0; //spring, damper, L_0
  };
  
  // a surface that works as struts inside
  class surface {
  public:
    int getIdx(int i, int j) {
      return i * ( subdividion_no + 2 ) + j;
    }

    // subdivide conform to blender subdivide of triangled plane
    // 0 is a two triangle plane
    surface(int width, int height, Vector3d Center, int subdivide,
            float spring, float damper, float mass) {
      int wstep = width / (subdivide + 1), hstep = height / (subdivide + 1);
      int vertice_no = (subdivide + 2) * (subdivide + 2);
      //int strut_no = (subdivide + 1) * (3 * subdivide + 5);
      int strut_no = (subdivide + 1) * (4 * subdivide + 6); // for all connection
      subdividion_no = subdivide;
      
//      for ( int i = 0 ; i < subdivide * 4; ++i ) {
//        vertices.push_back(Struts_Vertice(
//            1.0, //mass = 1.0 kg
//            Vector3d(0.0, 0.0, 0.0), //force 0, 0, 0
//            Vector3d(Center.x - width / 2 + (int) i * wstep,  Center.y, Center.z),
//            Vector3d(0.0, 0.0, 0.0),
//            
//            ) );
//      }
      for(int i = 0 ; i < subdivide + 2; ++i) {
        for(int j = 0; j < subdivide + 2; ++j) {
          vertices.push_back(
            Struts_Vertice(
                           mass, // mass
                           Vector3d(0.0, 0.0, 0.0), // force
                           Vector3d(Center.x - width / 2 + (int) (wstep * j),
                                    0.0,
                                    Center.z - hstep / 2 + (int) (hstep * i)
                                    ), //location
                           Vector3d(0.0, 0.0, 0.0) // velocity
                           
            )
          );
        }
      }
      
      Vector3d& current_loc = vertices[0].location;
      for(int i = 0 ; i < subdivide + 2; ++i) {
        for(int j = 0; j < subdivide + 2; ++j) {
          current_loc = vertices[i * (subdivide + 2) + j].location;
          // right i, j+1
          struts.push_back(
            Strut(spring, damper,
                  (vertices[i * ( subdivide + 2 ) + j].location - current_loc).norm()
                  )
          );
        }
      }
      
    }
    std::vector<Struts_Vertice> vertices;
    std::vector<Strut> struts;
    int subdividion_no;
    
  };
}//ns pobj

#endif /* defined(__PhysicalAnimation__object__) */
