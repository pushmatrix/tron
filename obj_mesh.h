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

// for hashmaps
using namespace __gnu_cxx;

namespace CSI4130 {
	
struct Material {
	vec4f d_ambient;
	vec4f d_diffuse;
	vec4f d_specular;
	GLfloat d_shininess;
	vec4f d_emission;
	Material() : d_ambient( 0.2f, 0.2f, 0.2f, 1.0f ), 
	d_diffuse( 0.8f, 0.8f, 0.8f, 1.0f ), 
	d_specular( 0.0f, 0.0f, 0.0f, 1.0f ),
	d_shininess(  ),
	d_emission(0.0, 0.0, 0.0, 1.0) {}
	
	// sets the current opengl state to the current material 
	void setState() {
		// only use front lighting
		glMaterialfv(GL_FRONT, GL_AMBIENT, d_ambient );
		glMaterialfv(GL_FRONT, GL_DIFFUSE, d_diffuse );
		glMaterialfv(GL_FRONT, GL_SPECULAR, d_specular );
		glMaterialf(GL_FRONT, GL_SHININESS, d_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, d_emission );
	}
};
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