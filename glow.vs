// ==========================================================================
// $Id: a3_color.vs,v 1.1 2011/02/27 03:56:20 jlang Exp $
// Basic shader emulating fixed pipeline functionality
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
// Creator: jlang (Jochen Lang)
// Email:   jlang@site.uottawa.ca
// ==========================================================================
// $Log: a3_color.vs,v $
// Revision 1.1  2011/02/27 03:56:20  jlang
// Created assign 3 skeleton
//
// Revision 1.1  2011/01/08 04:36:11  jlang
// Lab 0 solution
//
//
// ==========================================================================
#version 110

varying vec2 vTexCoord;
 
// remember that you should draw a screen aligned quad
void main(void)
{
   gl_Position = ftransform();;
  
   // Clean up inaccuracies
   vec2 Pos;
   Pos = sign(gl_Vertex.xy);
 
   gl_Position = vec4(Pos, 0.0, 1.0);
   // Image-space
   vTexCoord = Pos * 0.5 + 0.5;
   //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   //gl_FrontColor = gl_Color;
}
