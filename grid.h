/*
 *  grid.h
 *  tron
 *
 *  Created by Daniel Beauchamp on 11-03-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GRID
#define GRID
#include "OpenGL/OpenGL.h"
#include "mesh.h"
#include <vector>

using namespace std;

class Grid: public Mesh {
public:
	GLuint d_rows;
	GLuint d_cols;
	GLfloat d_cellWidth;
	GLfloat d_cellHeight;
	vector<GLuint> d_map;
	
	Grid(GLuint rows, GLuint cols, GLfloat cellWidth, GLfloat cellHeight);
	~Grid();
	void init();
	void draw();
	
	int cellType(vec2i coord);
	
	// Given a position on the map, return the coordinates for it.
	vec3f mapPosToCoords(int x, int y);
	// Given coordinates, return the position on the map.
	vec2f coordsToMapPos(vec3f coords);
	
	void reset();
};
#endif