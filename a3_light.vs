// ==========================================================================
// $Id: a3_light.vs,v 1.1 2011/02/27 03:56:20 jlang Exp $
// Basic Phong shading emulating fixed pipeline functionality
// ===================================================================
// (C)opyright:
//
//   Jochen Lang
//   SITE, University of Ottawa
//   800 King Edward Ave.
//   Ottawa, On., K1N 6N5
//   Canada.
//   http://www.site.uottawa.ca
//
// Creator: jlang (Jochen Lang)
// Email:   jlang@site.uottawa.ca
// ==========================================================================
// $Log: a3_light.vs,v $
// Revision 1.1  2011/02/27 03:56:20  jlang
// Created assign 3 skeleton
//
// Revision 1.2  2011/02/26 22:12:25  jlang
// Completed Lab 6
//
//
// ==========================================================================
#version 110
varying vec3 normalVec, lightVec, eyeVec;

void main() {
  // map the vertex position into clipping space 
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  // map the vertex position in camera coordinates
  vec4 posVec =  gl_ModelViewMatrix * gl_Vertex; 
  
  // camera coordinate normal
  normalVec = gl_NormalMatrix * gl_Normal;
  
  // light vector in camera coordinates
  // Check for directional lighting	
  if ( abs(gl_LightSource[0].position.w) > 0.0 ) {
    lightVec = gl_LightSource[0].position.xyz - posVec.xyz;
  } else {
    lightVec = gl_LightSource[0].position.xyz;
  }

  // eyeVec
  eyeVec = -posVec.xyz;

  gl_FrontColor = gl_Color;
}
