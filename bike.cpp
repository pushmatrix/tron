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
	d_startPos = startPos;
	d_startSpeed = d_speed;
	reset();
}

Bike::~Bike() {}

void Bike::update(GLfloat timestep) {
	
	updateCamera();
	GLfloat distance = d_speed * timestep;
	
	// Check for falling first
	if (outOfBounds(d_position.x(), d_position.z())) {
		d_freeFall += timestep; 
		d_position.y() += -0.008 * d_freeFall;
		d_wall.grow(d_position, true);
		d_nextDirection = d_direction;
	}
	
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
		if (grid.cellType(d_cellPos) == 1) {
			d_speed = 0;
		}
	}
	while (distance >= grid.d_cellWidth) {
		d_position.x() += d_direction.x() * (grid.d_cellWidth);
		d_position.z() += d_direction.y() * (grid.d_cellWidth);
		d_cellPos += d_direction;
		distance -= grid.d_cellWidth;
		if (grid.cellType(d_cellPos) == 1) {
			d_speed = 0;
		}
	}
	d_partial += distance;
	d_turnTravel += distance;
	d_position.x() += d_direction.x() * distance;
	d_position.z() += d_direction.y() * distance;
	if(d_turnTravel > getDepth()) {
	   d_wall.grow(d_position, false);
	}
}

void Bike::draw() {
	glColor3f(d_color.r(), d_color.g(), d_color.b());
	d_wall.draw();
	glPushMatrix();
	glTranslatef(d_position.x(), d_position.y(), d_position.z());
		glutSolidSphere(0.5f, 10.0f, 10.0f);
	glPopMatrix();
}

void Bike::reset() {
	d_position = d_startPos;
	d_cellPos.x() = d_startPos.x();
	d_cellPos.y() = d_startPos.z();
	d_direction.x() = 1;
	d_direction.y() = 0;
	d_vSpeed = 0;
	d_speed = d_startSpeed;
	d_wall.reset(d_startPos);
}

void Bike::updateCamera() {
	if (!outOfBounds(d_position.x(), d_position.y())) {
		d_cameraPos.x() += (d_position.x() - d_cameraPos.x() - d_direction.x() * 7)/20;
		d_cameraPos.y() += (d_position.z() - d_cameraPos.y() - d_direction.y() * 7)/20;
	} else if (!outOfBounds(d_cameraPos.x(), d_cameraPos.y())) {
		d_cameraPos.x() += d_direction.x() * .0013;
		d_cameraPos.y() += d_direction.y() * .0013;
	}


	d_cameraEye.x() = d_position.x() + d_direction.x() * 6.5;
	d_cameraEye.y() = d_position.y();
	d_cameraEye.z() = d_position.z() + d_direction.y() * 6.5;
}

bool Bike::outOfBounds(GLfloat _x, GLfloat _z) {
	if ((_x < 0)||(_x > grid.d_rows)||(_z < 0)||(_z > grid.d_cols)) {
		return true;
	} else {
		return false;
	}

}