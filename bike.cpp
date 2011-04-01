/*
 *  bike.cpp
 *  tron
 *
 *  Created by Daniel Beauchamp on 11-03-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "bike.h"

Bike::Bike(vec3f startPos, GLfloat speed, Grid& grid, vec4f color) : d_speed(speed), grid(grid), d_wall(1.0f, startPos), d_color(color) {
	d_position.x() = startPos.x();
	d_position.z() = startPos.z();
	d_cellPos.x() = startPos.x();
	d_cellPos.y() = startPos.z();
	d_direction.x() = 1;
	d_direction.y() = 0;
	// TODO: set wall on the starting pos
}
Bike::~Bike() {}

void Bike::init() {
	OBJMesh::init("bike.obj","bike.mtl");
}

void Bike::update(GLfloat timestep) {
	
	GLfloat distance = d_speed * timestep;
	if (d_partial + distance >= grid.d_cellWidth) {
		d_position.x() += d_direction.x() * (grid.d_cellWidth - d_partial);
		d_position.z() += d_direction.y() * (grid.d_cellWidth - d_partial);
		d_cellPos += d_direction;
		distance -= grid.d_cellWidth - d_partial;
		d_partial = 0;
		// If there is a turn.
		if (d_nextDirection.x() != 0 || d_nextDirection.y() != 0) {
			d_direction.x() = d_nextDirection.x();
			d_direction.y() = d_nextDirection.y();
			d_nextDirection.x() = 0;
			d_nextDirection.y() = 0;
			d_turnTravel = 0;
			d_wall.grow(d_position, true);
		}
		if (grid.cellType(d_cellPos) != 0) {
			d_speed = 0;
		}
	}
	while (distance >= grid.d_cellWidth) {
		d_position.x() += d_direction.x() * (grid.d_cellWidth);
		d_position.z() += d_direction.y() * (grid.d_cellWidth);
		d_cellPos += d_direction;
		distance -= grid.d_cellWidth;
		if (grid.cellType(d_cellPos) != 0) {
			d_speed = 0;
		}
	}
	d_partial += distance;
	d_turnTravel += distance;
	d_position.x() += d_direction.x() * distance;
	d_position.z() += d_direction.y() * distance;
	if(d_turnTravel > depth()) {
	   d_wall.grow(d_position, false);
	}
	
}

void Bike::draw() {
	glColor3f(d_color.r(), d_color.g(), d_color.b());
	d_wall.draw();
	glPushMatrix();
		glTranslatef(d_position.x(), d_position.y(), d_position.z() + 6);
		glScalef(0.2,0.2,0.2);
		OBJMesh::draw(true);
	glPopMatrix();
}