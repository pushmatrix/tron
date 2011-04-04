// ==========================================================================
// $Id: obj_mesh.h,v 1.1 2011/02/27 03:56:21 jlang Exp $
// Basic obj mesh loader and renderer
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
// Creator: Jochen Lang
// Email:   jlang@site.uottawa.ca
// ==========================================================================
// $Log: obj_mesh.h,v $
// Revision 1.1  2011/02/27 03:56:21  jlang
// Created assign 3 skeleton
//
//
// ==========================================================================
#ifndef CSI4130_OBJ_MESH_H
#define CSI4130_OBJ_MESH_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLUT/glut.h>

#include "vec.h"
#include "Material.h"

// for hashmaps
using namespace __gnu_cxx;

namespace CSI4130 {
	
	// Multiple faces can be grouped together under a common material.
	struct FaceGrouping {
		std::vector<vec3us> d_faceArrVec;
		std::string d_materialName;
		GLuint d_nTri;
		GLuint d_iboID;
	};
	
class OBJMesh {
  std::string d_fileName;
  bool d_init;
  std::vector<vec3f> d_vertArrVec;
  std::vector<vec3f> d_normalArrVec;
	std::map<const std::string,Material> d_materials;
  // bounds
  GLfloat d_minX;
  GLfloat d_minY;
  GLfloat d_minZ;
  GLfloat d_maxX;
  GLfloat d_maxY;
  GLfloat d_maxZ;
  // You can assume that you will always get triangles
  // Each grouping of faces optionally has a material associated to it.
	std::map<std::string,FaceGrouping> d_groups;
  GLuint d_nTri; 
  GLuint d_nGroups; 
  GLuint d_bufferObject;
  GLuint d_normalObject;
	

public:
	vec3f d_position;
	vec3f d_velocity;
  OBJMesh();
  ~OBJMesh();
	bool init(const std::string& _fileName, const std::string& _mtlFileName);
	inline GLfloat minX() {return d_minX;};
	inline GLfloat minY() {return d_minY;};
	inline GLfloat minZ() {return d_minZ;};
	inline GLfloat maxX() {return d_maxX;};
	inline GLfloat maxY() {return d_maxY;};
	inline GLfloat maxZ() {return d_maxZ;};
	inline GLfloat width() {return d_maxX-d_minX;};
	inline GLfloat height() {return d_maxY-d_minY;};
	inline GLfloat depth() {return d_maxZ-d_minZ;};
  void draw(bool lightOn);  
 protected:
  bool read(const std::string& _fileName);
  bool readMTL(const std::string& _fileName);
  void updateBoundaries(const vec3f &vert);
};

}

#endif
