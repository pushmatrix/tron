/*
 *  bike.cpp
 *  tron
 *
 *  Created by Daniel Beauchamp on 11-03-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "bike.h"

Bike::Bike(GLfloat speed, Grid& grid) : d_speed(speed), grid(grid) {
	d_direction.x() = 1;
	d_direction.y() = 0;
}
Bike::~Bike() {}

void Bike::update(GLfloat timestep) {
	
	GLfloat distance = d_speed * timestep;
	if (d_partial + distance >= grid.d_cellWidth) {
		d_position.x() += d_direction.x() * (grid.d_cellWidth - d_partial);
		d_position.z() += d_direction.y() * (grid.d_cellWidth - d_partial);
		distance -= grid.d_cellWidth - d_partial;
		d_partial = 0;
		if (d_nextDirection.x() != 0 || d_nextDirection.y() != 0) {
			d_direction.x() = d_nextDirection.x();
			d_direction.y() = d_nextDirection.y();
			d_nextDirection.x() = 0;
			d_nextDirection.y() = 0;
		}
		// check for collision
	}
	while (distance >= grid.d_cellWidth) {
		d_position.x() += d_direction.x() * (grid.d_cellWidth);
		d_position.z() += d_direction.y() * (grid.d_cellWidth);
		distance -= grid.d_cellWidth;
		// check for collision
	}
	d_partial += distance;
	d_position.x() += d_direction.x() * distance;
	d_position.z() += d_direction.y() * distance;
}

void Bike::draw() {
	glTranslatef(d_position.x(), d_position.y(), d_position.z());
	glutSolidSphere(0.5f, 10.0f, 10.0f);
}