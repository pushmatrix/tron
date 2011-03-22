// ==========================================================================
// $Id: vec.h,v 1.8 2011/02/27 03:56:21 jlang Exp $
// Simple vec helper class
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
// $Log: vec.h,v $
// Revision 1.8  2011/02/27 03:56:21  jlang
// Created assign 3 skeleton
//
// Revision 1.7  2011/02/26 22:12:41  jlang
// Completed Lab 6
//
// Revision 1.6  2011/02/25 16:49:25  jlang
// Fixed include order of gl headers
//
// Revision 1.5  2011/02/14 12:53:37  jlang
// lab5
//
// Revision 1.4  2011/02/11 01:57:05  jlang
// Upgrade for assignment 2
//
// Revision 1.3  2011/02/09 14:35:34  jlang
// Update vcproj and viewsol
//
// Revision 1.2  2011/01/24 05:10:52  jlang
// Added line.h
//
// Revision 1.1  2011/01/08 04:36:25  jlang
// Lab 0 solution
//
//
// ==========================================================================
#ifndef CSI4130_VEC_H_
#define CSI4130_VEC_H_

#include <iostream>
#include <cmath>

namespace CSI4130 {


template <class T, const int NUM> class vec {
  T d_comp[NUM];
 public:
  inline vec( T* _in );
  inline vec();
  inline T& operator()(int i) { return d_comp[i]; }
  inline T operator()(int i) const { return d_comp[i]; }
  operator T* () { return d_comp; } 
  operator const T* () { return d_comp; } 
};


template <class T, const int NUM>
  vec<T,NUM>::vec(T* _in) {
  for (int i=0;i<NUM; ++i) {
    d_comp[i] = _in[i];
  }
}


template <class T, const int NUM>
vec<T,NUM>::vec() {
  for (int i=0;i<NUM; ++i) {
    d_comp[i] = 0;
  }
}


template <class T> class vec<T,2> {
  T d_comp[2];
 public:
  inline vec( const T _in[2] );
  inline vec( const T _x,const T _y );
  inline vec();
  inline T& x() { return d_comp[0]; }
  inline T& y() { return d_comp[1]; }
  inline T x() const { return d_comp[0]; }
  inline T y() const { return d_comp[1]; }
  operator T* () { return d_comp; }   
  operator const T* () { return d_comp; } 
  friend std::ostream& operator<<( std::ostream& _out, const vec<T,2>& _pt) {
    _out << _pt.d_comp[0] << " " << _pt.d_comp[1] << " ";
    return _out;
  };
};


template <class T>
vec<T,2>::vec(const T _in[2]) {
  d_comp[0] = _in[0];
  d_comp[1] = _in[1];
}


template <class T>
vec<T,2>::vec( const T _x,const T _y ) {
  d_comp[0] = _x;
  d_comp[1] = _y;
}

template <class T>
vec<T,2>::vec() {
  d_comp[0] = 0;
  d_comp[1] = 0;
}


template <class T> class vec<T,3> {
  T d_comp[3];
 public:
  inline vec( const T _in[3] );
  inline vec( const T _x, const T _y, const T _z );
  inline vec();
  inline T& x() { return d_comp[0]; }
  inline T& y() { return d_comp[1]; }
  inline T& z() { return d_comp[2]; }
  inline T x() const { return d_comp[0]; }
  inline T y() const { return d_comp[1]; }
  inline T z() const { return d_comp[2]; }
  inline T& r() { return d_comp[0]; }
  inline T& g() { return d_comp[1]; }
  inline T& b() { return d_comp[2]; }
  inline T r() const { return d_comp[0]; }
  inline T g() const { return d_comp[1]; }
  inline T b() const { return d_comp[2]; }
  inline vec<T,3>& operator+=( const vec<T,3>& _pt );
  inline vec<T,3>& operator-=( const vec<T,3>& _pt );
  inline vec<T,3>& operator*=( const T& _scalar );
	
	// Helper methods to take the dot product, cross product, and normalization of vectors.
	inline T dot(vec<T,3>& other) { return d_comp[0]*other.x()+d_comp[1]*other.y()+d_comp[2]*other.z(); }
	inline vec<T,3> cross(vec<T,3>& other) {
		return vec<T,3>(d_comp[1]*other.z() -d_comp[2]*other.y(),d_comp[2]*other.x() -d_comp[0]*other.z(),d_comp[0]*other.y() -d_comp[1]*other.x()); 
	}
	
	inline void normalize() {
		double len = sqrt(d_comp[0]*d_comp[0]+d_comp[1]*d_comp[1]+d_comp[2]*d_comp[2]);
		d_comp[0] /= len;
		d_comp[1] /= len;
		d_comp[2] /= len;
	}
  
  operator T* () { return d_comp; } 
  operator const T* () { return d_comp; } 
  friend std::ostream& operator<<( std::ostream& _out, const vec<T,3>& _pt ) {
    _out << _pt.d_comp[0] << " " << _pt.d_comp[1] << " " 
	 << _pt.d_comp[2] << " ";
    return _out;
  };
  friend std::istream& operator>>( std::istream& _in, vec<T,3>& _pt ) {
    _in >> _pt.d_comp[0] >> _pt.d_comp[1] >> _pt.d_comp[2];
    if ( !_in ) _pt=vec();
    return _in;
  };
};


template <class T>
  vec<T,3>::vec(const T _in[3]) {
  d_comp[0] = _in[0];
  d_comp[1] = _in[1];
  d_comp[2] = _in[2];
 }

template <class T>
  vec<T,3>::vec( const T _x,const T _y, const T _z ) {
  d_comp[0] = _x;
  d_comp[1] = _y;
  d_comp[2] = _z;
 }

template <class T>
  vec<T,3>::vec() {
  for (int i=0;i<3; ++i) {
    d_comp[i] = 0;
  }
 }

template <class T>
  vec<T,3>& vec<T,3>::operator+=( const vec<T,3>& _pt ) {
  for ( int i=0; i<3; ++i ) {
    d_comp[i] += _pt.d_comp[i];
  }
  return *this;
 }

template <class T>
  vec<T,3>& vec<T,3>::operator-=( const vec<T,3>& _pt ) {
  for ( int i=0; i<3; ++i ) {
    d_comp[i] -= _pt.d_comp[i];
  }
  return *this;
 }

template <class T>
  vec<T,3>& vec<T,3>::operator*=( const T& scalar  ) {
  for ( int i=0; i<3; ++i ) {
    d_comp[i] *= scalar;
  }
  return *this;
 }


template <class T> class vec<T,4> {
  T d_comp[4];
 public:
  inline vec( const T _in[4] );
  inline vec( const T _x, const T _y, const T _z, const T _w );
  inline vec();
  inline T& x() { return d_comp[0]; }
  inline T& y() { return d_comp[1]; }
  inline T& z() { return d_comp[2]; }
  inline T& w() { return d_comp[3]; }
  inline T x() const { return d_comp[0]; }
  inline T y() const { return d_comp[1]; }
  inline T z() const { return d_comp[2]; }
  inline T w() const { return d_comp[3]; }
  inline T& r() { return d_comp[0]; }
  inline T& g() { return d_comp[1]; }
  inline T& b() { return d_comp[2]; }
  inline T& a() { return d_comp[3]; }
  inline T r() const { return d_comp[0]; }
  inline T g() const { return d_comp[1]; }
  inline T b() const { return d_comp[2]; }
  inline T a() const { return d_comp[3]; }
  inline vec<T,4>& operator+=( const vec<T,4>& _pt );
  inline vec<T,4>& operator-=( const vec<T,4>& _pt );
  inline vec<T,4>& operator*=( const T& _scalar );
  operator T* () { return d_comp; } 
  operator const T* () { return d_comp; } 
  friend std::ostream& operator<<( std::ostream& _out, const vec<T,4>& _pt ) {
    _out << _pt.d_comp[0] << " " << _pt.d_comp[1] << " " 
	 << _pt.d_comp[2] << " " << _pt.d_comp[3] << " ";
    return _out;
  };
	friend std::istream& operator>>( std::istream& _in, vec<T,4>& _pt ) {
		_in >> _pt.d_comp[0] >> _pt.d_comp[1] >> _pt.d_comp[2] >> _pt.d_comp[3];
		if ( !_in ) _pt=vec();
		return _in;
	};
};


template <class T>
  vec<T,4>::vec(const T _in[4]) {
  d_comp[0] = _in[0];
  d_comp[1] = _in[1];
  d_comp[2] = _in[2];
  d_comp[3] = _in[3];
 }

template <class T>
  vec<T,4>::vec( const T _x,const T _y, const T _z , const T _w ) {
  d_comp[0] = _x;
  d_comp[1] = _y;
  d_comp[2] = _z;
  d_comp[3] = _w;
 }

template <class T>
  vec<T,4>::vec() {
  for (int i=0;i<4; ++i) {
    d_comp[i] = 0;
  }
 }

template <class T>
  vec<T,4>& vec<T,4>::operator+=( const vec<T,4>& _pt ) {
  for ( int i=0; i<4; ++i ) {
    d_comp[i] += _pt.d_comp[i];
  }
  return *this;
 }

template <class T>
  vec<T,4>& vec<T,4>::operator-=( const vec<T,4>& _pt ) {
  for ( int i=0; i<4; ++i ) {
    d_comp[i] -= _pt.d_comp[i];
  }
  return *this;
 }

template <class T>
  vec<T,4>& vec<T,4>::operator*=( const T& scalar  ) {
  for ( int i=0; i<4; ++i ) {
    d_comp[i] *= scalar;
  }
  return *this;
 }

#define vec2d vec<GLdouble,2>
#define vec2f vec<GLfloat,2>
#define vec2i vec<GLint,2>
#define vec2ub vec<GLubyte,2>
#define vec3d vec<GLdouble,3>
#define vec3f vec<GLfloat,3>
#define vec3i vec<GLint,3>
#define vec3ub vec<GLubyte,3>
#define vec3us vec<GLushort,3>
#define vec4d vec<GLdouble,4>
#define vec4f vec<GLfloat,4>
#define vec4i vec<GLint,4>
#define vec4ub vec<GLubyte,4>

} // end namespace

#endif


// Module test
#ifdef MOD_TEST_VEC

using namespace CSI4130;
using std::cerr;
using std::endl;

int main () {
  double data[] = { 0.0, 1.0, 2.0, 3.0 };
  // 2D template
  vec2d v2dA;
  cerr << v2dA << endl;
  vec<double,2> v2dB(data);
  cerr << v2dB << endl;
  vec<double,2> v2dC(data[2],data[3]);
  cerr << v2dC << endl;
  v2dA = v2dC;
  cerr << v2dA << endl;
  vec2d v2dD(v2dC);
  cerr << v2dD << endl;
  // 3D template
  vec3d v3dA;
  vec<double,3> v3dB(data);
  vec<double,3> v3dC(data[0],data[1],data[2]);
  // Generic template
  vec<double,4> v4dA;
  vec<double,4> v4dB(data);
}
#endif

