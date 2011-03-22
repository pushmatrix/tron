// ==========================================================================
// $Id: shader.cpp,v 1.1 2011/01/08 04:36:24 jlang Exp $
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
// $Log: shader.cpp,v $
// Revision 1.1  2011/01/08 04:36:24  jlang
// Lab 0 solution
//
//
// ==========================================================================
#include <fstream>
#include <sstream>
#include "shader.h"




void getGlVersion( int& major, int& minor )
{
  char dot;
  std::string verStr = std::string((const char*)glGetString( GL_VERSION ));
  std::istringstream converter(verStr);
  converter >> major;
  converter >> dot;
  if ( dot == '.' ) {
    converter >> minor;
  } else {
    minor = 0;
  }
  return;
}




int Shader::load( std::string filename, GLuint shaderType ) {
  std::ostringstream os;
  std::string line;
  std::ifstream inFile( filename.c_str());
  if ( !inFile ) {
    cerr << "Error: unable to open: " << filename << endl;
    return -1;
  }
  // Read the shader file but keep all characters (including ws and cr)
  while (getline(inFile, line)) {
    os << line << endl;
    if ( inFile.fail() ) {
      cerr << "Read error: " << filename << endl;
      inFile.close();
      return -1;
    }
  }
  inFile.close();
  switch (shaderType) {
  case GL_VERTEX_SHADER:
    d_vertShaderTxt=os.str();
    d_vertShaderRead=true;
    break;
  case GL_FRAGMENT_SHADER:
    d_fragShaderTxt=os.str();
    d_fragShaderRead=true;
    break;
  default:
    cerr << "Invalid shader type: " <<  shaderType << endl;
    return -2;
  }
  return errorOut();
}
  

int Shader::installShader( GLuint& handle, GLuint shaderType ) {
  handle = glCreateShader( shaderType );
  if ( handle == GL_INVALID_OPERATION ) {
    cerr << "Invalid operation: " << shaderType << endl;
    return -1;
  } else if ( handle == GL_INVALID_ENUM ) {
    cerr << "Invalid Enum: " << endl;
    return -1;
  }
  const GLchar* shaderTxt;
  switch (shaderType) {
  case GL_VERTEX_SHADER:
    if ( d_vertShaderRead ) {
      // cerr << d_vertShaderTxt << endl;
      shaderTxt = static_cast<const GLchar*>(d_vertShaderTxt.c_str());
    } else { 
      return -2;
    }
    break;
  case GL_FRAGMENT_SHADER:
    if ( d_fragShaderRead ) {
      // cerr << d_fragShaderTxt << endl;
      shaderTxt = static_cast<const GLchar*>(d_fragShaderTxt.c_str());
    } else { 
      return -2;
    }
    break;
  default:
    cerr << "Invalid shader type: " <<  shaderType << endl;
    return -2;
  }
  glShaderSource( handle, 1, &shaderTxt, NULL );
  return errorOut();
}

int Shader::compile( GLuint handle ) {
  glCompileShader(handle);
  GLint success;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success );
  if ( !success ) { 
    // get and print the shader info log
    int bSize;
    GLchar *infoLog;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &bSize);
    if (bSize > 0) {
      infoLog = new GLchar[bSize+1];
      glGetShaderInfoLog(handle, bSize, NULL, infoLog);
      cerr << "Compile error: (" << handle << ") :" << infoLog << endl;
      delete [] infoLog;
    }
    return -1;
  }
  return errorOut();
}


int Shader::installProgram(vector<GLuint> shaderHandles,
			   GLuint& program) 
{
  program = glCreateProgram();
  for ( vector<GLuint>::const_iterator iter=shaderHandles.begin(); 
	iter != shaderHandles.end(); ++iter ) { 
    glAttachShader(program, *iter);
  }
  glLinkProgram(program);
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success );
  if ( !success ) {
    int bSize;
    GLchar *infoLog;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bSize);
    if (bSize > 0) {
      infoLog = new GLchar[bSize+1];
      glGetProgramInfoLog(program, bSize, NULL, infoLog);
      cerr << "Link error (" << program << ") :" << infoLog << endl;
      delete [] infoLog;
    }
    return -1;
  }
  return errorOut();
}

 
int Shader::validateProgram(GLuint program) {
  glValidateProgram(program);
  GLint success;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &success );
  if ( !success ) {
    int bSize;
    GLchar *infoLog;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bSize);
    if (bSize > 0) {
      infoLog = new GLchar[bSize+1];
      glGetProgramInfoLog(program, bSize, NULL, infoLog);
      cerr << "Validate error: (" << program << ") :" << infoLog << endl;
      delete [] infoLog;
    }
    return -1;
  }
  return errorOut();
}

