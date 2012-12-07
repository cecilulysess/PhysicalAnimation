/*
 *  FrameGrabber.h
 *  
 *  Class to grab OpenGL frames and output to image files at HD resolution  
 *
 *  Created by Don House on 11/30/10.
 *  Copyright 2010 Clemson University. All rights reserved.
 *
 */

#ifndef __FRAMEGRABBER__
#define __FRAMEGRABBER__

#include <iostream>

class FrameGrabber{
private:
  int W, H;
  int Frame;
  int Textureid, FBid, DBid;
  std::string Filename;

  void Initialize();
  void writeimage();

public:
  FrameGrabber(const std::string &path, const std::string &filename, int w = 1920, int h = 1080, int f = 0);
  ~FrameGrabber();
  
  std::string filename(){return Filename;}
  void setfilename(const std::string &path, const std::string &filename);
  void setfilename(const std::string &filename);
  
  void setsize(int w, int h){W = w; H = h;}
  int width(){return W;}
  int height(){return H;}
  void setframe(int f){Frame = f;}
  int frame(){return Frame;}
  
  void recordimage(void (*drawingprocedure)());
};

#endif
