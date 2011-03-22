// ==========================================================================
// $Id: shader.h,v 1.2 2011/01/10 20:52:46 jlang Exp $
// Helper Program for shader loading for CSI 4130 using openGL 2.0
// ==========================================================================
// (C)opyright:
//
//   Jochen Lang
//   SITE, University of Ottawa
//   800 King Edward Ave.
//   Ottawa, On., K1N 6N5
//   Canada.
//   http://www.site.uottawa.ca
//
// Creator: jlang (Jochen Lang)
// Email:   jlang@site.uottawa.ca
// ==========================================================================
// $Log: shader.h,v $
// Revision 1.2  2011/01/10 20:52:46  jlang
// Remove comments for wgl
//
// Revision 1.1  2011/01/08 04:36:24  jlang
// Lab 0 solution
//
//
// ==========================================================================
#ifndef CSI4130_SHADER_H_
#define CSI4130_SHADER_H_


#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#if WIN32
#include <gl/wglew.h>
#else
//#include <GL/glext.h>
#endif

using std::cerr;
using std::endl;
using std::vector;


void getGlVersion( int& major, int& minor );


inline static int _printOpenGLerrors(const char *file, int line) {
  GLenum glErrCode;
  int res=0;
  while (GL_NO_ERROR != (glErrCode = glGetError())) {
    cerr <<"glError in file: " << file << " line: " << line 
    << " -- " << gluErrorString(glErrCode);
    res = glErrCode;
  }
  return res;
}

inline int errorOut() {
  return _printOpenGLerrors(__FILE__, __LINE__);
}


class Shader {
  std::string d_vertShaderTxt;
  std::string d_fragShaderTxt;
  bool d_vertShaderRead;
  bool d_fragShaderRead;
 public:
  Shader() : d_vertShaderRead(false), d_fragShaderRead(false) {}

  /** All functions will return 0 on success */
  // Load a shader from file
  int load( std::string filename, GLuint shaderType );
  // Install a shader previously read
  int installShader( GLuint& handle, GLuint shaderType );

  static int compile( GLuint handle );
  static int installProgram(vector<GLuint> shaderHandles, GLuint& program); 
  static int validateProgram(GLuint program);
};

#endif
