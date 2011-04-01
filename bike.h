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
#include "wall.h"
#include <GLUT/glut.h>

class Bike: public Mesh {
public:
	Wall d_wall;
	
	vec4f d_color;
	vec2i d_direction;
	vec2i d_cellPos;
	vec3f d_startPos;
	GLfloat d_speed;
	GLfloat d_vSpeed;
	GLfloat d_freeFall;
	GLfloat d_startSpeed;
	GLfloat d_partial;
	GLfloat d_angle;
	GLfloat d_nextAngle;
	
	vec2f d_cameraPos;
	vec3f d_cameraEye;
	
	// How far you've travelled since your last turn.
	GLfloat d_turnTravel; 
	
	// Reference to the grid
	Grid& grid;
	
	vec2i d_nextDirection;
	
	Bike(vec3f startPos, GLfloat speed, Grid& grid, vec4f color);
	~Bike();
	
	void init();
	void draw();
	void update(GLfloat timestep);
	void reset();
	void updateCamera();
	bool outOfBounds(GLfloat _x, GLfloat _z);
	void turnLeft();
	void turnRight();
};