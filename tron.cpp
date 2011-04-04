#define NOMINMAX
#include <GL/glew.h>
#include <GLUT/glut.h>

#include "vec.h"
#include "shader.h"

#include "obj_mesh.h"
#include "Bike.h"
#include "Grid.h"


using namespace std;

// Global time
GLuint elapsed = 0;

// For mouse interaction
int oldX = 0;
int oldY = 0;
int rotationX = 0;
int rotationY = 0;
//int rotationY = 85;

int deltaX = 0;
int deltaY = 0;

Grid grid(50, 50, 1, 1);

Bike player1(vec3f(30,0,25), vec2i(1,0), 11.0f, grid, vec4f(0.9f,0.4f,0.1f,1.0f));
Bike player2(vec3f(20,0,25), vec2i(-1,0), 11.0f, grid, vec4f(0.1f,0.6f,0.8f,1.0f));

int winWidth = 1280;
int winHeight = 752;



namespace CSI4130 {
	
	// Window dimensions
	struct WindowSize {
		GLfloat d_near;
		GLfloat d_far;
		GLint d_widthPixel;
		GLfloat d_width;
		GLint d_heightPixel;
		GLfloat d_height;
		WindowSize() : d_near(5.0f), d_far(17.5f),
		d_widthPixel(512), d_width(12.5f),
		d_heightPixel(512), d_height(12.5f)
		{}
	}; 
	
	struct LightSource {
		vec4f d_ambient;
		vec4f d_diffuse;
		vec4f d_specular;
		// switch between point and directional light source
		bool d_pointLight;
		// spot light
		// v is the vector to the vertex
		// if if dir*v < cos(cutoff) then (dir * v)^N 
		vec3f d_spot_direction;
		GLfloat d_spot_exponent;
		GLfloat d_spot_cutoff;
		// attentuation 1/(k_c + k_l r + k_q r^2) 
		// r is the distance of a vertex from the light source
		GLfloat d_constant_attenuation;
		GLfloat d_linear_attenuation;
		GLfloat d_quadratic_attenuation;
		
		LightSource() : d_ambient( 0.0f, 0.0f, 0.0f, 1.0f ), 
		d_diffuse( 1.0f, 1.0f, 1.0f, 1.0f ),
		d_specular( 1.0f, 1.0f, 1.0f, 1.0f ), 
		d_pointLight( true ),
		d_spot_direction( 0.0f, 0.0f, -1.0f ),
		d_spot_exponent(0),
		d_spot_cutoff(180.0f),
		d_constant_attenuation(1.0f),
		d_linear_attenuation(0.0f),
		d_quadratic_attenuation(0.0f) {}
		
		void setState(GLenum lightNo) {
			// Add a light source; use light source 0
			glLightfv(lightNo, GL_AMBIENT, d_ambient );
			glLightfv(lightNo, GL_DIFFUSE, d_diffuse );
			glLightfv(lightNo, GL_SPECULAR, d_specular );
			glLightfv(lightNo, GL_SPOT_DIRECTION, d_spot_direction );
			glLightf(lightNo, GL_SPOT_EXPONENT, d_spot_exponent );
			glLightf(lightNo, GL_SPOT_CUTOFF, d_spot_cutoff );
			glLightf(lightNo, GL_CONSTANT_ATTENUATION, d_constant_attenuation );
			glLightf(lightNo, GL_LINEAR_ATTENUATION, d_linear_attenuation );
			glLightf(lightNo, GL_QUADRATIC_ATTENUATION, d_quadratic_attenuation );
			// Make sure to enable light source
			glEnable( lightNo );
		}
		
		
	};
	
	
	GLfloat g_lightAngle = 0.0f;
	GLfloat g_camX = 0.0f, g_camY = 0.0f;
	WindowSize g_winSize;  
	LightSource g_light;
	Material g_material;
	
	GLuint g_lightProgram;
	GLuint g_glowHorizontalProgram;
	GLuint g_glowVerticalProgram;
	GLuint g_colorProgram;
	GLuint g_specularMaterial;
	GLint g_glowHorizontalMap;
	GLint g_glowVerticalMap;
	GLuint g_glowTex;
	GLuint fbo;
	GLuint renderbuffer;
	GLuint img;
	GLuint img2;

	
	
	void init() {
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glEnable( GL_DEPTH_TEST );
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		g_light.setState(GL_LIGHT0 );
		// Eable lighting -- mapped on key L
		//glEnable(GL_LIGHTING);
		// Make sure the normals are fine
		glEnable(GL_NORMALIZE);
		// adjust the materials
		g_material.d_ambient = vec4f(0.05f, 0.05f, 0.1f, 1.0f); 
		g_material.d_diffuse = vec4f(0.0f, 0.75f, 0.75f, 1.0f); 
		g_material.d_specular = vec4f(1.0f, 1.0f, 1.0f, 1.0f); 
		g_material.d_shininess = 32;
		
		// Set up stuff
		grid.init();
		player1.init();
		player2.init();
		
		
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer);
		
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 400, 400);
		
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		glGenTextures(1, &img);
		glBindTexture(GL_TEXTURE_2D, img);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  400, 400, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		
		glGenTextures(1, &img2);
		glBindTexture(GL_TEXTURE_2D, img2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  400, 400, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, img, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, img2, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
		
		
		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
			cerr << "FBO not loaded!" << endl;
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
		vector<GLuint> sHandles;
		GLuint handle;
		
		
		
		
		// Load blur shaders
		Shader glowHorizontalSh;
		cerr << "Loading shaders: " << endl;
		if ( !glowHorizontalSh.load("glow.vs", GL_VERTEX_SHADER )) {
			glowHorizontalSh.installShader( handle, GL_VERTEX_SHADER );
			Shader::compile( handle );
			sHandles.push_back( handle );
		}
		if ( !glowHorizontalSh.load("glow_horizontal.fs", GL_FRAGMENT_SHADER )) {
			glowHorizontalSh.installShader( handle, GL_FRAGMENT_SHADER ); 
			Shader::compile( handle );
			sHandles.push_back( handle );
		}
		cerr << "No of handles: " << sHandles.size() << endl;
		Shader::installProgram(sHandles, g_glowHorizontalProgram); 
		// allocate a texture name
		g_glowHorizontalMap = glGetUniformLocation(g_glowHorizontalProgram, "glowMap");
		
		sHandles.clear();
		Shader glowVerticalSh;
		if ( !glowVerticalSh.load("glow.vs", GL_VERTEX_SHADER )) {
			glowVerticalSh.installShader( handle, GL_VERTEX_SHADER );
			Shader::compile( handle );
			sHandles.push_back( handle );
		}
		if ( !glowVerticalSh.load("glow_vertical.fs", GL_FRAGMENT_SHADER )) {
			glowVerticalSh.installShader( handle, GL_FRAGMENT_SHADER ); 
			Shader::compile( handle );
			sHandles.push_back( handle );
		}
		cerr << "No of handles: " << sHandles.size() << endl;
		Shader::installProgram(sHandles, g_glowVerticalProgram); 
		// allocate a texture name
		g_glowVerticalMap = glGetUniformLocation(g_glowVerticalProgram, "glowMap");
		
		
		// Load color shaders
		sHandles.clear();
		Shader colorSh;
		cerr << "Loading shaders: " << endl;
		if ( !colorSh.load("a3_color.vs", GL_VERTEX_SHADER )) {
			colorSh.installShader( handle, GL_VERTEX_SHADER );
			Shader::compile( handle );
			sHandles.push_back( handle );
		}
		if ( !colorSh.load("a3_color.fs", GL_FRAGMENT_SHADER )) {
			colorSh.installShader( handle, GL_FRAGMENT_SHADER ); 
			Shader::compile( handle );
			sHandles.push_back( handle );
		}
		cerr << "No of handles: " << sHandles.size() << endl;
		Shader::installProgram(sHandles, g_colorProgram); 
				
		// Load light shaders
		sHandles.clear();
		Shader lightSh;
		cerr << "Loading shaders: " << endl;
		if ( !lightSh.load("a3_light.vs", GL_VERTEX_SHADER )) {
			lightSh.installShader( handle, GL_VERTEX_SHADER );
			Shader::compile( handle );
			sHandles.push_back( handle );
		}
		if ( !lightSh.load("a3_light.fs", GL_FRAGMENT_SHADER )) {
			lightSh.installShader( handle, GL_FRAGMENT_SHADER ); 
			Shader::compile( handle );
			sHandles.push_back( handle );
		}
		cerr << "No of handles: " << sHandles.size() << endl;
		Shader::installProgram(sHandles, g_lightProgram); 
		g_specularMaterial = glGetUniformLocation(g_lightProgram, "specularMaterial");
		return;
	}
	
	void update() {
		GLfloat timestep = (glutGet(GLUT_ELAPSED_TIME) - elapsed) * 0.001;
		elapsed = glutGet(GLUT_ELAPSED_TIME); 
		player1.update(timestep);
		player2.update(timestep);
	}
	/**
	 * Idle routine - update scene
	 */
	void myIdleFunc() {
		update();
		glutPostRedisplay();
	}
	// Handles mouse movement to rotate the scene
	void mouseMove(int x, int y){
		deltaX = x - oldX;
		oldX = x;
		rotationX += deltaX/2.0;
		
		deltaY = y - oldY;
		oldY = y;
		rotationY += deltaY/2.0;
	}
	
	void mousePress(int button, int state, int x, int y){
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
			oldX = x;
			oldY = y;
		}
	}
	
	void drawScene() {
		glColor3f(0.8,0.1,0.3);
		
		grid.draw();
		glUseProgram(g_lightProgram);
		player1.draw();
		player2.draw();
		
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,400, 400);
		
		player1.draw();
		player2.draw();
		
		glPopAttrib();
		
		glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,400, 400);
		
		glEnable( GL_TEXTURE_2D );
		glUseProgram(g_glowHorizontalProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, img);
		glBegin(GL_QUADS);
		glTexCoord2d(-1.0,0.0); glVertex2d(-1.0,-1.0);
		glTexCoord2d(1.0,0.0); glVertex2d(1.0,-1.0);
		glTexCoord2d(1.0,1.0); glVertex2d(1.0,1.0);
		glTexCoord2d(0.0,1.0); glVertex2d(-1.0,1.0);
		glEnd();
		glPopMatrix();
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_BLEND );
		
		
		glPopAttrib();
		
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
		
		glEnable( GL_TEXTURE_2D );
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);
		glUseProgram(g_glowVerticalProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, img2);
		glBegin(GL_QUADS);
		glTexCoord2d(-1.0,0.0); glVertex2d(-1.0,-1.0);
		glTexCoord2d(1.0,0.0); glVertex2d(1.0,-1.0);
		glTexCoord2d(1.0,1.0); glVertex2d(1.0,1.0);
		glTexCoord2d(0.0,1.0); glVertex2d(-1.0,1.0);
		glEnd();
		glPopMatrix();
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_BLEND );
		glUseProgram(0);
	}
	
	void display(void)
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		// Set the modelview matrix to I
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		
		//g_material.setState();
		// Place a light source at a radius from the camera
		glLightfv(GL_LIGHT0, GL_POSITION, 
				  vec4f( cos(g_lightAngle)*g_winSize.d_width, 
						sin(g_lightAngle)*g_winSize.d_width, 
						static_cast<GLfloat>( !g_light.d_pointLight ), 
						static_cast<GLfloat>( g_light.d_pointLight ))); 


		glMatrixMode(GL_PROJECTION);
		glViewport( 0, winHeight/2, winWidth, winHeight/2);
		glLoadIdentity();		
		gluPerspective(  60.0,
					   (float)winWidth/(winHeight/2),
					   0.1,
					   500.0);
		
		gluLookAt( player1.d_cameraPos.x(), 1, player1.d_cameraPos.y(), 
				  player1.d_cameraEye.x(), player1.d_cameraEye.y(), player1.d_cameraEye.z(),
				  0, 1, 0.0f);
		
		drawScene();
				
		glViewport( 0, 0, winWidth, winHeight/2);
		glLoadIdentity();		
		gluPerspective(  60.0,
									 (float)winWidth/(winHeight/2),
									 0.1,
									 500.0);
		gluLookAt( player2.d_cameraPos.x(), 1, player2.d_cameraPos.y(), 
							player2.d_cameraEye.x(), player2.d_cameraEye.y(), player2.d_cameraEye.z(),
							0, 1, 0.0f);
		
		drawScene();
		glutSwapBuffers();
		
	}
	
	
	/**
	 * OpenGL reshape function - main window
	 */
	void lightReshape( GLsizei _width, GLsizei _height ) {
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		GLfloat minDim = std::min(g_winSize.d_width,g_winSize.d_height);
		// adjust the view volume to the correct aspect ratio
		if ( _width > _height ) {
			g_winSize.d_width = minDim  * (GLfloat)_width/(GLfloat)_height;
			g_winSize.d_height = minDim;
		} else {
			g_winSize.d_width = minDim;
			g_winSize.d_height = minDim * (GLfloat)_height/(GLfloat)_width;
		}
		/*gluPerspective(  60.0,
					   (float)_width/(float)_height,
					   0.1,
					   500.0);*/
		g_winSize.d_widthPixel = _width;
		g_winSize.d_heightPixel = _height;
		// reshape our viewport
		//glViewport( _width/2, _height/2, _width/2, _height/2);
		
	}
	
	
	void specialkeys( int key, int x, int y )
	{
		switch (key) {
			case GLUT_KEY_LEFT:
				player1.turnLeft();
				break;
			case GLUT_KEY_RIGHT: 
				player1.turnRight();
				break;
			default: break;
		}
		glutPostRedisplay();
	}
	
	
	
	void keyboard (unsigned char key, int x, int y)
	{
		switch (key) {
			case 27:
				
			case 'q':
				exit(0);
				break;
			case 'r':
				grid.reset();
				player1.reset();
				player2.reset();
				break;
			case 'Z':
				// increase near plane
				g_winSize.d_near += 0.1f;
				g_winSize.d_far += 0.1f;
				lightReshape( g_winSize.d_widthPixel, g_winSize.d_heightPixel );
				break;
			case 'z':
				// decrease near plane
				if ( g_winSize.d_near > 0.1f ) {
					g_winSize.d_near -= 0.1f;
					g_winSize.d_far -= 0.1f;
				}
				lightReshape( g_winSize.d_widthPixel, g_winSize.d_heightPixel );
				break;
				// Lighting keys
			case 'L':
				glEnable(GL_LIGHTING);
				break;
			case 'l':
				glDisable(GL_LIGHTING);
				break;
			case 'a':
				player2.turnLeft();
				break;
			case 's':
				player2.turnRight();
				break;
			default:
				break;
		}
		glutPostRedisplay();
	}
	
	
}

using namespace CSI4130;

int main(int argc, char** argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (winWidth, winHeight); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow (argv[0]);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		cerr << "Error: " << glewGetErrorString(err) << endl;
		return -1;
	}
	cerr << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	
	init();
	
	glutReshapeFunc(lightReshape);
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeys);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mousePress);
	glutIdleFunc(myIdleFunc);
	glutMainLoop();
	return 0;
}
