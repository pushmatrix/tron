/*
 *  wall.h
 *  tron
 *
 *  Created by Daniel Beauchamp on 11-03-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef WALL
#define WALL

#include "mesh.h"
#include <vector>

using namespace std;

class Wall : public Mesh {
public:
	vector<vec3f> d_verts;
	
	Wall(GLfloat height, vec3f startPos);
	~Wall();
	
	void grow(vec3f position, bool corner);
	void draw();
	void reset(vec3f startPos);
};
#endif
