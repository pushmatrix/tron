/*
 *  grid.cpp
 *  tron
 *
 *  Created by Daniel Beauchamp on 11-03-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "grid.h"

Grid::Grid(GLuint rows = 10, GLuint cols = 10, GLfloat cellWidth = 1, GLfloat cellHeight = 1): d_rows(rows), d_cols(cols), d_cellWidth(cellWidth), d_cellHeight(cellHeight), d_map(rows * cols) {
	d_dimensions.x() = rows * cellHeight;
	d_dimensions.z() = cols * cellWidth;
}

Grid::~Grid() {}

void Grid::init() {
	// Generate the grid
	vec3f vertices[d_rows * d_cols * 2];
	int index = 0;
	
	for (int i = 0; i < d_rows -1; i++) {
		for (int j = 0; j < d_cols; j++) {
			vertices[index++] = vec3f(i * d_cellHeight, 0.0, j * d_cellWidth);
			vertices[index++] = vec3f((i + 1) * d_cellHeight, 0, j * d_cellWidth);
		}
	}
	
	// Set up the vbo
	glGenBuffers(1, &d_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, d_vbo );
	glBufferData(GL_ARRAY_BUFFER, 
				 sizeof(vec3f) * d_rows * d_cols * 2,
				 &vertices[0], GL_STATIC_DRAW);
	
}

void Grid::draw() {
	// Set to wireframe mode
	glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE ) ;
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, d_vbo);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	for (int i = 0; i < d_rows; i++) {
		glDrawArrays(GL_QUAD_STRIP, i * d_cols * 2, d_cols * 2);
	}
	glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL ) ;
}

int Grid::cellType(vec2i coord) {
	if( coord.y() > d_cols || coord.x() > d_rows || coord.x() < 0 || coord.y() < 0) {
		return -1; // out of bounds
	}
	int index = coord.y() * d_cols + coord.x();
	if (d_map[index]) {
		return 1;
	} else {
		d_map[index] = 1;
		return 0;
	}
}

vec3f Grid::mapPosToCoords(int x, int y) {
	return vec3f(x * d_cellWidth, 0, y * d_cellHeight);
}

vec2f Grid::coordsToMapPos(vec3f coords) {
	GLfloat width = d_cellWidth * d_rows;
	GLfloat height = d_cellHeight * d_cols;
	
	GLfloat x = floor(coords.x() / width);
	GLfloat y = floor(coords.z() / height);
	
	// Clamp the values into the bounds
	if (x > d_rows) {
		x = d_rows;
	} else if (x < 0) {
		x = 0;
	}
	
	if (y > d_cols) {
		y = d_cols;
	} else if (y < 0) {
		y = 0;
	}
	
	return vec2f(x, y);
}