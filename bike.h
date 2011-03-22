/*
 *  bike.h
 *  tron
 *
 *  Created by Daniel Beauchamp on 11-03-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "mesh.h"
#include "grid.h"
#include <GLUT/glut.h>

class Bike: public Mesh {
public:
	vec2i d_direction;
	vec2i d_cellPos;
	GLfloat d_speed;
	GLfloat d_partial;
	
	// Reference to the grid
	Grid& grid;
	
	vec2i d_nextDirection;
	
	Bike(GLfloat speed, Grid& grid);
	~Bike();
	
	void init();
	void draw();
	void update(GLfloat timestep);
};