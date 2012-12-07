/*
 *  FrameGrabber.cpp
 *  
 *  Class to grab OpenGL frames and output to image files at HD resolution  
 *
 *  Created by Don House on 11/30/10.
 *  Copyright 2010 Clemson University. All rights reserved.
 *
 */

#include "FrameGrabber.h"

#include <Magick++.h>
#include <cstdlib>
#include <iostream>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES 1
#  include <GL/glut.h>
#endif

using namespace std;
using namespace Magick;

//
// Helper function to convert a frame number to a 4 digit string
//
string makeframenumber(int f){
  string fnum;
  int n;
  
  for(int i = 0; i < 4; i++){
    n = f % 10;
    fnum = fnum.insert(0, 1, '0' + n);
    f = f / 10;
  }

  return fnum;
}

FrameGrabber::FrameGrabber(const string &path, const string &filename, int w, int h, int f){
  setsize(w, h);
  setframe(f);
  setfilename(path, filename);
  FBid = Textureid = DBid = 0;
}

FrameGrabber::~FrameGrabber(){
}

void FrameGrabber::setfilename(const string &path, const string &filename){
  Filename = path;
  if(Filename.size() > 0){
    if(Filename[Filename.size() - 1] != '/')
      Filename.push_back('/');
    Filename = Filename + filename;
  }
  else
    Filename = filename;
}

void FrameGrabber::setfilename(const string &filename){
  Filename = filename;
}

//
// Initialize the framegrabber by creating a texture image for the image,
// a renderbuffer for the depth buffer, and a frame buffer object that holds
// the image and the depth buffer. Code modified from that in:
// http://www.songho.ca/opengl/gl_fbo.html
//
void FrameGrabber::Initialize(){
  // create a texture image for the frame buffer object (FBO) to draw to
  glGenTextures(1, (GLuint *)&Textureid);
  glBindTexture(GL_TEXTURE_2D, Textureid);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // create a renderbuffer object to use as the depth buffer
  GLuint rboId;
  glGenRenderbuffersEXT(1, (GLuint *)&DBid);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, DBid);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, W, H);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
  
  // create the FBO and attach the texture image to it
  glGenFramebuffersEXT(1, (GLuint *)&FBid);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBid);
  
  // attach the texture and the depth buffer to the FBO
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			    GL_TEXTURE_2D, Textureid, 0);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
			       GL_RENDERBUFFER_EXT, DBid);
  
  // If the FBO status is bad, warn user and ignore FBO
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT){
    cout << "bad framebuffer!" << endl;
    FBid = 0;
  }
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FrameGrabber::recordimage(void (*drawingprocedure)()){
  int savevp[4];

  // only create texture and frame buffer objects once
  if(FBid == 0)
    Initialize();

  // save viewport, and set focus to FBO, with full screen viewport
  glGetIntegerv(GL_VIEWPORT, savevp);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBid);
  glViewport(0, 0, W, H);

  // draw the scene offscreen to the texture in the FBO
  drawingprocedure();
  glFlush();

  // capture the image and write the next animation frame file
  writeimage();

  // restore the OpenGL framebuffer and viewport
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glViewport(savevp[0], savevp[1], savevp[2], savevp[3]);
}

/*
  Routine to write the current framebuffer to an image file
*/
void FrameGrabber::writeimage(){
  string framenumber, outfilename;
  unsigned char pixmap[4 * W * H];
  
  // add the frame number to the file name and .png
  framenumber = makeframenumber(Frame);
  outfilename = Filename + "." + framenumber + ".png";
  cout << outfilename << endl;
  
  // fetch the current frame from the texture object into a pixmap
  glBindTexture(GL_TEXTURE_2D, Textureid);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixmap);
  glBindTexture(GL_TEXTURE_2D, 0);

  // create an ImageMagick image from the pixmap
  // and flip it or it will be upside down in the image file
  Image image(W, H, "RGBA", CharPixel, (void *)pixmap);
  image.flip();
  
  // output the frame as an image file 
  image.write(outfilename);

  // increment the frame number
  Frame++;
}
