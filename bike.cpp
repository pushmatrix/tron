/*
 *  bike.cpp
 *  tron
 *
 *  Created by Daniel Beauchamp on 11-03-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "bike.h"

Bike::Bike(vec3f startPos, vec2i startDirection, GLfloat speed, Grid& grid, vec4f color) : d_speed(speed), d_angle(90), grid(grid), d_wall(0.2f, startPos), d_color(color) {
	d_startPos = startPos;
	d_startSpeed = d_speed;
	d_startDirection.x() = startDirection.x();
	d_startDirection.y() = startDirection.y();
	d_cameraPos.x() = d_position.x();
	d_cameraPos.y() = d_position.z();
	reset();
}

Bike::~Bike() {}

void Bike::init() {
	OBJMesh::init("bike.obj","bike.mtl");
}

void Bike::update(GLfloat timestep) {
	
	updateCamera();
	GLfloat distance = d_speed * timestep;
	
	// Check for falling first
	if (outOfBounds(d_position.x(), d_position.z())) {
		d_freeFall += timestep; 
		d_position.y() += -0.045 * d_freeFall;
		d_wall.grow(d_position, true);
		d_nextDirection = d_direction;
		d_nextAngle = 0;
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
			d_angle += d_nextAngle;
			d_nextAngle = 0;
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
	if(d_turnTravel > 0.01) {
	   d_wall.grow(d_position, false);
	}
}

void Bike::draw() {
	glColor3f(d_color.r(), d_color.g(), d_color.b());
	d_wall.draw();
	glPushMatrix();

		glTranslatef(d_position.x(), d_position.y(), d_position.z());
		glScalef(0.013,0.013,0.013);
		glRotatef(d_angle, 0,1,0);
		OBJMesh::draw(true);
	glPopMatrix();
}

void Bike::reset() {
	d_position = d_startPos;
	d_cellPos.x() = d_startPos.x();
	d_cellPos.y() = d_startPos.z();
	d_direction = d_startDirection;
	d_vSpeed = 0;
	d_nextDirection = d_direction;
	d_speed = d_startSpeed;
	d_freeFall = 0;
	d_cameraPos.x() = d_position.x();
	d_cameraPos.y() = d_position.z();
	d_wall.reset(d_startPos);
	d_angle = 90;
}

void Bike::updateCamera() {
	if (!outOfBounds(d_position.x(), d_position.y())) {
		d_cameraPos.x() += (d_position.x() - d_cameraPos.x() - d_direction.x() )/5;
		d_cameraPos.y() += (d_position.z() - d_cameraPos.y() - d_direction.y() )/5;
	} else if (!outOfBounds(d_cameraPos.x(), d_cameraPos.y())) {
		d_cameraPos.x() += d_direction.x() * .01;
		d_cameraPos.y() += d_direction.y() * .01;
	}

	d_cameraEye.x() = d_position.x() + d_direction.x() * 1.5;
	d_cameraEye.y() = d_position.y();
	d_cameraEye.z() = d_position.z() + d_direction.y() * 1.5;
}

bool Bike::outOfBounds(GLfloat _x, GLfloat _z) {
	if ((_x < 0)||(_x > grid.d_rows)||(_z < 0)||(_z > grid.d_cols)) {
		return true;
	} else {
		return false;
	}
}

void Bike::turnLeft() {
	d_nextDirection.x() = d_direction.y();
	d_nextDirection.y() = -d_direction.x();
	d_nextAngle += 90;
}

void Bike::turnRight() {
	d_nextDirection.x() = -d_direction.y();
	d_nextDirection.y() = d_direction.x();
	d_nextAngle -= 90;
}