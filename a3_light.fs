// ==========================================================================
// $Id: a3_light.fs,v 1.1 2011/02/27 03:56:20 jlang Exp $
// Basic Phong shading emulating fixed pipeline functionality
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
// $Log: a3_light.fs,v $
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
uniform vec4 specularMaterial;

void main() {
  vec3 NVec = normalize(normalVec);
  vec3 LVec = normalize(lightVec);
  vec3 EVec = normalize(eyeVec);

  float distanceLight = length(lightVec.xyz);

  float attenuation = 1.0 / 
    (gl_LightSource[0].constantAttenuation +
     gl_LightSource[0].linearAttenuation * distanceLight +
     gl_LightSource[0].quadraticAttenuation * distanceLight * distanceLight);

  // ambient term
  vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;

  // diffuse term
  float dotNL = max(0.0,dot(NVec,LVec));
  vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * dotNL;
  
  vec3 HVec = normalize(LVec+EVec);
  float dotNH = max(0.0,dot(NVec,HVec));
 // vec4 specular = gl_FrontMaterial.specular * gl_LightSource[0].specular 
  //     * pow(dotNH,gl_FrontMaterial.shininess);

  vec4 specular = specularMaterial * gl_LightSource[0].specular * pow(dotNH,gl_FrontMaterial.shininess);
  
  // spot light
  float spot_attenuation = 1.0;
  float dotSV = dot(-EVec,normalize(gl_LightSource[0].spotDirection));
  if ( dotSV < cos(radians(gl_LightSource[0].spotCutoff))) {
    spot_attenuation = 0.0;
  } else {
    spot_attenuation = pow(dotSV,gl_LightSource[0].spotExponent);
  }

  // color
  gl_FragColor = ambient + 
    attenuation * spot_attenuation * (diffuse + specular);
}
