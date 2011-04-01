// ==========================================================================
// $Id: obj_mesh.cpp,v 1.1 2011/02/27 03:56:21 jlang Exp $
// Helper class to draw a plane with an indexed VBO
// ==========================================================================
// (C)opyright:
//
//   Jochen Lang
//   SITE, University of Ottawa
//   800 King Edward Ave.
//   Ottawa, On., K1N 6N5
//   Canada.
//   http://www.site.uottawa.ca
//
// Creator: Jochen Lang
// Email:   jlang@site.uottawa.ca
// ==========================================================================
// $Log: obj_mesh.cpp,v $
// Revision 1.1  2011/02/27 03:56:21  jlang
// Created assign 3 skeleton
//
// Revision 1.1  2011/02/26 22:12:25  jlang
// Completed Lab 6
//
// ==========================================================================
#include <iostream>
#include <cassert>

#include "obj_mesh.h"
#include "vec.h"
#include "shader.h"

#define DEBUG_OBJ_MESH

using namespace std;

namespace CSI4130 {
	
	OBJMesh::OBJMesh() : d_init(false),d_minX(0), d_minY(0), d_minZ(0), d_maxX(0), d_maxY(0), d_maxZ(0)
	{
	}
	
	// String splitting provided by http://stackoverflow.com/questions/236129/how-to-split-a-string
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while(std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}
	
	
	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		return split(s, delim, elems);
	}
	
	bool OBJMesh::readMTL(const std::string& _fileName){
		ifstream file(_fileName.c_str());
		
		if (!file.is_open()){
			cerr << "Error: " << _fileName << " does not exist" << endl;
			return false;
		}
		
		// Parse one line at a time
		string line, token;
		Material* currentMaterial = &d_materials["default"];;
		cerr << "Reading mat file..." << endl;
		
		while (getline(file, line)) {
			istringstream streamLine(line);
			// //cerr << "Got: " << line << endl;
			streamLine >> token;
			// Process key
			//  //cerr << token << " ";
			switch ( token[0] ) {
				case 'n':
					// check if a new material is being defined
					if ( token.compare("newmtl")==0 ) {
						std::string materialName;
						streamLine >> materialName;
						currentMaterial = &d_materials[materialName.c_str()];
					} 
					break;
				case 'K':{
					if ( token.compare("Ka")==0 ) {
						vec3f property;
						streamLine >> property;
						currentMaterial->d_ambient.x() = property.x();
						currentMaterial->d_ambient.y() = property.y();
						currentMaterial->d_ambient.z() = property.z();
						currentMaterial->d_ambient.w() = 1;
					} else if(token.compare("Kd")==0){
						vec4f property;
						streamLine >> property;
						currentMaterial->d_diffuse.x() = property.x();
						currentMaterial->d_diffuse.y() = property.y();
						currentMaterial->d_diffuse.z() = property.z();
						currentMaterial->d_diffuse.w() = property.w();
					} else if(token.compare("Ks")==0){
						vec3f property;
						streamLine >> property;
						currentMaterial->d_specular.x() = property.x();
						currentMaterial->d_specular.y() = property.y();
						currentMaterial->d_specular.z() = property.z();
						currentMaterial->d_specular.w() = 1;
					}
					break;
				}
				case 'T':
				{
					if ( token.compare("Tr")==0 ) {
						GLfloat alpha;
						streamLine >> alpha;
						currentMaterial->d_ambient.w() = alpha;
						currentMaterial->d_diffuse.w() = alpha;
						currentMaterial->d_specular.w() = alpha;
					}
					
					break;
				}
				case 'i':
				{
					if ( token.compare("illum")==0 ) {
						int type;
						streamLine >> type;
					}

					break;
				}
				case 'N':
				{
					if ( token.compare("Ns")==0 ) {
						GLfloat shininess;
						streamLine >> shininess;
						currentMaterial->d_shininess = shininess;
					}
					break;
				}
				default:
					break;
			}
		}
		return true;
	}
	
	bool OBJMesh::read(const std::string& _fileName) {
			
		
		ifstream file(_fileName.c_str());
		if (!file.is_open()){
			//cerr << "Error: " << _fileName << " does not exist" << endl;
			return false;
		}
		
		const vec3us one(1,1,1);
		// Parse one line at a time
		string line, token;
			
		// default group
		FaceGrouping defaultGroup;
		d_groups["default"] = defaultGroup;
			
		FaceGrouping* currentGroup = &d_groups["default"];
			
		// //cerr << "Reading ..." << endl;
		while (getline(file, line)) {
			istringstream streamLine(line);
			// //cerr << "Got: " << line << endl;
			streamLine >> token;
			// Process key
			//  //cerr << token << " ";
			switch ( token[0] ) {
				case 'v':
					// check for vertex vs. vertex normal
					if ( token.compare("v")==0 ) {
						vec3f vert;
						streamLine >> vert;
						d_vertArrVec.push_back(vert);
						// update the max/min boundaries of this mesh
						updateBoundaries(vert);
						////cerr << vert << endl;
						if ( streamLine >> token ) {
							//  //cerr << "Extra coordinates: " << token << endl;
							while ( streamLine >> token ) {
								// Drain extra coordinates
								//   //cerr << " " << token << endl;
							}
						}
					} else if ( token.compare("vn")==0 ) {
						vec3f normal;
						streamLine >> normal;
						d_normalArrVec.push_back(normal);
						if ( streamLine >> token ) {
							//  //cerr << "Extra coordinates: " << token << endl;
							while ( streamLine >> token ) {
								// Drain extra coordinates
								//   //cerr << " " << token << endl;
							}
						}
					}
					break;
				case 'f':
					// check for face vs. face normal
					if ( token.compare("f")==0 ) {
						unsigned int indices[3];
						
						//face -= one;
						
						for(int i=0; i < 3 && streamLine >> token; i++ ){
							vector<string> x =split(token, '/');
							std::stringstream ss(x[0]);
							int val;
							ss >> val;
							indices[i] = val - 1;
						}
						vec3us face(indices[0], indices[1], indices[2]);
						currentGroup->d_faceArrVec.push_back(face);
						

						if ( streamLine >> token ) {
							//cerr << "Extra indices: " << token << endl;
							while ( streamLine >> token ) {
								// Drain extra indices
								//cerr << " " << token << endl;
							}
						}
					}
					break;
				case 'u': 
					if ( token.compare("usemtl")==0 ) {
						string materialName;
						streamLine >> materialName;
						currentGroup = &d_groups[materialName];
						currentGroup->d_materialName = materialName;
					}
					break;
				default:
					break;
			}
		}
			
		d_nGroups = 0;
		// Update polycounts
			for(map<std::string,FaceGrouping>::iterator it = d_groups.begin(); it != d_groups.end(); it++) {
				it->second.d_nTri = it->second.d_faceArrVec.size();
				d_nTri += it->second.d_nTri;
				d_nGroups++;
			}

			
		/*
		 
		 Overall Procedure for Normal Generation (if needed)
		 
		 We create 2 vertex arrays:
			- normalSums: contains the sum of normals for each face adjacent to vertices
			- adjacentCounts: contains the number of faces that are adjacent to each vertex
		 
		 1) For each face grouping:
			  For each face in the grouping:
				1) Determine the normal by using the cross product
				2) Add the normal to the sum array for each vertex making up the face
				3) Increase the number of adjacent faces by 1 for each vertex making up the face
		 
		 2) For a given vertex, since we know the sum of all adjacent faces AND the number of adjacent faces,
			we can calculate the average.
		 
		 3) Place the average normal for each vertex into the normals array.
		 
		*/
		
		if (d_normalArrVec.size() == 0) {
			int numVertices = d_vertArrVec.size();
			vector<vec3f*> normalSums(numVertices, NULL);
			vector<int> adjacentCounts(numVertices, 0);
			
			for(map<std::string,FaceGrouping>::iterator it = d_groups.begin(); it != d_groups.end(); it++) {
				int tricount = it->second.d_faceArrVec.size();
				for (int i = 0; i < tricount; i++) {
					GLuint vertexIndices[3] = {0};
					
					vertexIndices[0] = it->second.d_faceArrVec[i].x();
					vertexIndices[1] = it->second.d_faceArrVec[i].y();
					vertexIndices[2] = it->second.d_faceArrVec[i].z();
					
					// Get vertices from the face in order to compute normal (using cross product)
					vec3f v1 = d_vertArrVec[vertexIndices[0]];
					vec3f v2 = d_vertArrVec[vertexIndices[1]];
					vec3f v3 = d_vertArrVec[vertexIndices[2]];
					
					vec3f delta1 = v3-=v2;
					vec3f delta2 = v1-=v2;
					
					vec3f normal = delta1.cross(delta2);
					normal.normalize();
					
					// Create the normal sums if they don't exist already.
					if(!normalSums[vertexIndices[0]]){
						normalSums[vertexIndices[0]] = new vec3f(0, 0, 0);
					}
					if(!normalSums[vertexIndices[1]]){
						normalSums[vertexIndices[1]] = new vec3f(0, 0, 0);
					}
					if(!normalSums[vertexIndices[2]]){
						normalSums[vertexIndices[2]] = new vec3f(0, 0, 0);
					}
					
					// add this normal to each of the vertices for this face.
					normalSums[vertexIndices[0]]->x() += normal.x();
					normalSums[vertexIndices[0]]->y() += normal.y();
					normalSums[vertexIndices[0]]->z() += normal.z();
					
					normalSums[vertexIndices[1]]->x() += normal.x();
					normalSums[vertexIndices[1]]->y() += normal.y();
					normalSums[vertexIndices[1]]->z() += normal.z();
					
					normalSums[vertexIndices[2]]->x() += normal.x();
					normalSums[vertexIndices[2]]->y() += normal.y();
					normalSums[vertexIndices[0]]->z() += normal.z();
					
					// increment the # of adjacent faces for each of the vertices of this face.
					adjacentCounts[vertexIndices[0]] += 1;
					adjacentCounts[vertexIndices[1]] += 1;
					adjacentCounts[vertexIndices[2]] += 1;
				}
			}
			
			for (int i = 0;  i < numVertices; i++) {
				if(normalSums[i]){
					int adjacentFaceCount = adjacentCounts[i];
					normalSums[i]->x() /= adjacentFaceCount;
					normalSums[i]->y() /= adjacentFaceCount;
					normalSums[i]->z() /= adjacentFaceCount;
					
					normalSums[i]->normalize();
					d_normalArrVec.push_back(*normalSums[i]);
				} else{
					d_normalArrVec.push_back(vec3f(0,0,1));
				}
			}
			normalSums.clear();
		}
		
		file.close();
		return true;
	};
	
	/*
	 Overall Procedure:
	 
	 If a material file is specified, then it is loaded and parsed according to the .mtl spec. Each material
	 is stored in a map with a key on the material name. 
	 
	 Obj files can contain groups which are essentially groupings of faces. Each group is represented by the FaceGrouping struct.
	 A FaceGrouping not only contains the faces for that group, but also the name of the associated material. Each FaceGrouping is also
	 stored in a map, with a key on the group name (in this case the group name is the same as the material name specified). The map allows
	 groupings to be referenced by any arbitrary string, rather than by a sequential number (if it was in an array).
	 
	 The rendering process goes as follows:
		1) Set the VBO
		2) Bind the normal array
		3) For each group:
			a) Use the group's material name as a key to fetch the corresponding material in the Material Map.
			b) Set the material
			c) Bind the group's index buffer object
			d) Draw the elements in the IBO
	 
	 ** For details on normal generation, see the comments inside the read function
	 
	*/
	
	
	
	bool OBJMesh::init(const std::string& _fileName, const std::string& _mtlFileName) {
		// If a material was specified
		readMTL(_mtlFileName);
		
		cerr << "Opening: " << _fileName << endl;
		if ( !read(_fileName)) return false;
		d_init = true;
		// Generate a VBO
		glGenBuffers( 1, &d_bufferObject );
		errorOut();
		glBindBuffer(GL_ARRAY_BUFFER, d_bufferObject );
		glBufferData(GL_ARRAY_BUFFER, 
					 sizeof(vec3f)*d_vertArrVec.size(),
					 &d_vertArrVec[0], GL_STATIC_DRAW);
		
		// One IBO for each group
		for(map<std::string,FaceGrouping>::iterator it = d_groups.begin(); it != d_groups.end(); it++) {
			glGenBuffers(1, &it->second.d_iboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->second.d_iboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
						 sizeof(vec3us) * it->second.d_nTri,
						 &it->second.d_faceArrVec[0], GL_STATIC_DRAW);
		}
		glGenBuffers(1, &d_normalObject);
		
		 glBindBuffer(GL_ARRAY_BUFFER, d_normalObject);
		 glBufferData(GL_ARRAY_BUFFER, 
		 sizeof(vec3f)*d_normalArrVec.size(),
		 &d_normalArrVec[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
		
		glBindBuffer(GL_ARRAY_BUFFER, 0 );
		glBindBuffer(GL_NORMAL_ARRAY, 0 );
		
		errorOut();
		// no longer needed now OpenGL stores it
		d_vertArrVec.clear();
		d_normalArrVec.clear();
		cerr << ".obj loaded successfully!" <<endl;
		return true;
	}
	
	
	void OBJMesh::draw(bool lightOn = true) {
		// uses the default normal in z
		if (!d_init) {
			cerr << "OBJMesh was not initialized!" << endl;
			return;
		}
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_INDEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		// Use VBO for drawing
		glBindBuffer(GL_ARRAY_BUFFER, d_bufferObject);
		glVertexPointer(3, GL_FLOAT, 0, 0 );
		glBindBuffer(GL_ARRAY_BUFFER, d_normalObject);
		glNormalPointer(GL_FLOAT, 0, 0 );
		
		// Render each group
		for(map<std::string,FaceGrouping>::iterator it = d_groups.begin(); it != d_groups.end(); it++) {
			// Set up the material properties
			
			std::string matName = it->second.d_materialName;
			Material& g_material = d_materials[matName.c_str()];
			if(lightOn){
				g_material.setState();
			} else{
				glColor3f(g_material.d_diffuse.x(),g_material.d_diffuse.y(),g_material.d_diffuse.z());
			}
			
			// Draw the object
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->second.d_iboID);
			glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);
			glDrawElements(GL_TRIANGLES, 3*it->second.d_nTri, GL_UNSIGNED_SHORT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
		glBindBuffer(GL_ARRAY_BUFFER, 0 );
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	
	OBJMesh::~OBJMesh() {
		if (d_init) {
			glDeleteBuffers(1, &d_bufferObject);
			for(map<std::string,FaceGrouping>::iterator it = d_groups.begin(); it != d_groups.end(); it++) {
				glDeleteBuffers(1, &it->second.d_iboID);
			}
			glDeleteBuffers(1, &d_normalObject);
		}
	}
	
	void OBJMesh::updateBoundaries(const vec3f &vert){
	    if (vert.x() > d_maxX) {
		    d_maxX = vert.x();
	    }
		if (vert.y() > d_maxY) {
			d_maxY = vert.y();
		}
		if (vert.z() > d_maxZ) {
			d_maxZ = vert.z();
		}
		
		if (vert.x() < d_minX) {
		    d_minX = vert.x();
	    }
		if (vert.y() < d_minY) {
			d_minY = vert.y();
		}
		if (vert.z() < d_minZ) {
			d_minZ = vert.z();
		}
	}
	
}  // end namespace
