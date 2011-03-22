// ==========================================================================
// $Id: a3_color.fs,v 1.1 2011/02/27 03:56:20 jlang Exp $
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
// $Log: a3_color.fs,v $
// Revision 1.1  2011/02/27 03:56:20  jlang
// Created assign 3 skeleton
//
// Revision 1.1  2011/01/08 04:36:11  jlang
// Lab 0 solution
//
//
// ==========================================================================
#version 110

void main() {
  // color the fragment 
  gl_FragColor = 0.5 * gl_Color;
}