/*
 *  wall.cpp
 *  tron
 *
 *  Created by Daniel Beauchamp on 11-03-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "wall.h"

Wall::Wall(GLfloat height, vec3f startPos) {
	d_dimensions.y() = height;
	d_verts.push_back(startPos);
	d_verts.push_back(startPos);
}


Wall::~Wall() {}


void Wall::grow(vec3f position, bool corner = false) {
	if(!corner) {
		d_verts.back() = position;
	} else {
		d_verts.push_back(position);
	}
}

void Wall::draw() {
	glBegin(GL_QUAD_STRIP);
		for (int i=0; i < d_verts.size(); i++) {
			glVertex3f(d_verts[i].x(), d_verts[i].y(), d_verts[i].z());
			glVertex3f(d_verts[i].x(), d_verts[i].y() + getHeight(), d_verts[i].z());
		}
	glEnd();
}

void Wall::reset(vec3f startPos) {
	///
	d_verts.clear();
	d_verts.push_back(startPos);
	d_verts.push_back(startPos);
}

