/*
 *  mesh.h
 *  tron
 *
 *  Created by Daniel Beauchamp on 11-03-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef MESH
#define MESH
#include "OpenGL/OpenGl.h"
#include "vec.h"
#include "math.h"

using namespace CSI4130;

class Mesh {
public:
	GLuint d_vbo;
	GLuint d_numVerts;
	GLuint d_numTris;
	
	vec3f d_dimensions;
	
	vec3f d_position;
	vec3f d_velocity;
	
	inline GLfloat getWidth() { return d_dimensions.x(); }
	
	inline GLfloat getHeight() { return d_dimensions.y(); }
	
	inline GLfloat getDepth() { return d_dimensions.z(); }
	
	// Sets up the vbo, loads the model, etc...
	void init();
	
	// Updates position, etc...
	void update(GLuint timestep);
	
	// Renders the scene
	void draw();
};
#endif