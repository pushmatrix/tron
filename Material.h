#ifndef MATERIAL
#define MATERIAL
namespace CSI4130 {
struct Material {
	vec4f d_ambient;
	vec4f d_diffuse;
	vec4f d_specular;
	GLfloat d_shininess;
	vec4f d_emission;
	Material() : d_ambient( 0.2f, 0.2f, 0.2f, 1.0f ), 
	d_diffuse( 0.8f, 0.8f, 0.8f, 1.0f ), 
	d_specular( 0.0f, 0.0f, 0.0f, 1.0f ),
	d_shininess(  ),
	d_emission(0.0, 0.0, 0.0, 1.0) {}
	
	// sets the current opengl state to the current material 
	void setState() {
		// only use front lighting
		glMaterialfv(GL_FRONT, GL_AMBIENT, d_ambient );
		glMaterialfv(GL_FRONT, GL_DIFFUSE, d_diffuse );
		glMaterialfv(GL_FRONT, GL_SPECULAR, d_specular );
		glMaterialf(GL_FRONT, GL_SHININESS, d_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, d_emission );
	}
};
}
#endif