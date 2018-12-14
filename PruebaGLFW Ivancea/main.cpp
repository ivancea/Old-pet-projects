#include <iostream>
#include <string>
#include <vector>

#include <GL/glfw.h>      // Incluimos GLFW

#include "Stuff.hpp"

#include "Camara.hpp"       // Incluimos la Camara
#include "FpsManager.hpp" // Incluimos el FpsManager
#include "Vec.hpp"       // Incluimos Vec
#include "Objeto.hpp"
#include "OBJ_Model.hpp"

using namespace std;

vector<GLuint> Texturas; // Array de Texturas
GLuint SkyboxTexture; // Textura del SkyBox
void CargarTexturas(); // Funcion de carga de Texturas

vector<OBJ_Model> Modelos;

Camera *cam;

string windowTitle = "Sensual";

// Limita a 60 FPS y cambia el Titulo de la ventana cada 3s
FpsManager fpsManager(60.0);

GLint windowWidth   = 800;              // Width of our window
GLint windowHeight  = 600;              // Heightof our window

GLint midWindowX    = windowWidth  / 2; // Middle of the window horizontally
GLint midWindowY    = windowHeight / 2; // Middle of the window vertically

GLfloat fieldOfView = 45.0f;            // Define our field of view (i.e. how quickly foreshortening occurs)
GLfloat near        = 0.01f;             // The near (Z Axis) point of our viewing frustum (default 2.0f)
GLfloat far         = 1500.0f;          // The far  (Z Axis) point of our viewing frustum (default 1500.0f)

// Función que maneja los eventos del TECLADO
void handleKeypress(int theKey, int theAction){

	// If a key is pressed, toggle the relevant key-press flag
	if (theAction == GLFW_PRESS){

		switch (theKey){

		case 'W':
			cam->holdingForward = true;
			break;
		case 'S':
			cam->holdingBackward = true;
			break;
		case 'A':
			cam->holdingLeftStrafe = true;
			break;
		case 'D':
			cam->holdingRightStrafe = true;
			break;
		case '-':
			fpsManager.setTargetFps(fpsManager.getTargetFps() - 10);
			break;
		case '+':
			fpsManager.setTargetFps(fpsManager.getTargetFps() + 10);
			break;
		default:
			// Do nothing...
			break;
		}

	} else { // If a key is released, toggle the relevant key-release flag

		switch (theKey){

		case 'W':
			cam->holdingForward = false;
			break;
		case 'S':
			cam->holdingBackward = false;
			break;
		case 'A':
			cam->holdingLeftStrafe = false;
			break;
		case 'D':
			cam->holdingRightStrafe = false;
			break;
		default:
			// Do nothing...
			break;
		}
	}
}

// Función que maneja los eventos del RATON
void handleMouseMove(int mouseX, int mouseY){

    // Le pasamos las coordenadas a la Cámara
	cam->handleMouseMove(mouseX, mouseY);
}

void initGL(){

	// ----- Opciones GLFW -----

	glfwDisable(GLFW_MOUSE_CURSOR); // Escondemos el cursor

	//glfwSwapInterval(0);            // Disable vsync

	// ----- Window and Projection Settings -----

	// Set the window title
	glfwSetWindowTitle(windowTitle.c_str());

	// Setup our viewport to be the entire size of the window
	glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

	// Change to the projection matrix, reset the matrix and set up our projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat aspectRatio = (windowWidth > windowHeight)? float(windowWidth)/float(windowHeight) : float(windowHeight)/float(windowWidth);
	GLfloat fH = tan( float(fieldOfView / 360.0f * 3.14159f) ) * near;
	GLfloat fW = fH * aspectRatio;
	glFrustum(-fW, fW, -fH, fH, near, far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ----- OpenGL settings -----

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);              // Set out clear colour to black, full alpha
	glEnable(GL_DEPTH_TEST);                           // Enable the depth buffer
	glClearDepth(1.0f);                                // Clear the entire depth of the depth buffer
	glDepthFunc(GL_LEQUAL);		                       // Set our depth function to overwrite if new value less than or equal to current value
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);    // Renderizado suave
}

// Function to draw a grid of lines
void drawGround(float groundLevel){

    GLfloat extent      = 300.0f; // How far on the Z-Axis and X-Axis the ground extends
	GLfloat stepSize    = 1.0f;  // The size of the separation between points

	// Draw our ground grid
	glBegin(GL_LINES);
	for (GLint loop = -extent; loop < extent; loop += stepSize)
	{
		// Draw lines along Z-Axis
		glVertex3f(loop, groundLevel,  extent);
		glVertex3f(loop, groundLevel, -extent);

		// Draw lines across X-Axis
		glVertex3f(-extent, groundLevel, loop);
		glVertex3f( extent, groundLevel, loop);
	}
	glEnd();
}

void Draw_Skybox(float x, float y, float z, float width, float height, float length){

	// Centramos la caja respecto a las coordenadas
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, SkyboxTexture);

	// FRENTE
	glBegin(GL_QUADS);
		glTexCoord2f(1/2.f, 1/3.f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1/2.f, 2/3.f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(1/4.f, 2/3.f); glVertex3f(x+width, y+height, z+length);
		glTexCoord2f(1/4.f, 1/3.f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// ATRAS
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1/3.f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 2/3.f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(3/4.f, 2/3.f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(3/4.f, 1/3.f); glVertex3f(x,		  y,		z);
	glEnd();

	// IZQUIERDA
	glBegin(GL_QUADS);
		glTexCoord2f(3/4.f, 2/3.f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(2/4.f, 2/3.f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(2/4.f, 1/3.f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(3/4.f, 1/3.f); glVertex3f(x,		  y,		z);
	glEnd();

	// DERECHA
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1/3.f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1/4.f, 1/3.f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1/4.f, 2/3.f); glVertex3f(x+width, y+height,	z+length);
		glTexCoord2f(0.0f, 2/3.f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// ARRIBA
    glBegin(GL_QUADS);
		glTexCoord2f(1/4.f, 1.f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1/4.f, 2/3.f); glVertex3f(x+width, y+height, z+length);
		glTexCoord2f(2/4.f, 2/3.f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(2/4.f, 1.f); glVertex3f(x,		  y+height,	z);
	glEnd();

    // ABAJO
	glBegin(GL_QUADS);
		glTexCoord2f(2/4.f, 0.f); glVertex3f(x,		  y,		z);
		glTexCoord2f(2/4.f, 1/3.f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1/4.f, 1/3.f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1/4.f, 0.f); glVertex3f(x+width, y,		z);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

// Pintamos nuestra escena
void drawScene(){

	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Disable lighting
    glDisable(GL_LIGHTING);

    glAccum(GL_RETURN, 0.55f); /** EFECTO MOTION-BLUR **/
    glClear(GL_ACCUM_BUFFER_BIT);

	// Reset the matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/** ROTAR Y POSICIONAR CÁMARA **/
	glRotatef(cam->getXRot(), 1.0f, 0.0f, 0.0f); // Rotate our camera on the x-axis (looking up and down)
	glRotatef(cam->getYRot(), 0.0f, 1.0f, 0.0f); // Rotate our camera on the  y-axis (looking left and right)
	glTranslatef( -cam->getXPos(), -cam->getYPos(), -cam->getZPos() );
	/** FIN CÁMARA **/


	/** 3D **/

	// SkyBox
	Draw_Skybox(cam->getXPos(),cam->getYPos(),cam->getZPos(),50,50,50);

    //glColor3f(255,255,255);
	///drawGround(0.0f); // Draw lower ground grid
	//drawGround(100.0f);  // Draw upper ground grid
	//glColor3f(255,255,255);

    //glTranslatef(0, 0, -100);

    // Bind the texture
    /*glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textura[0]);

	GLfloat cube[] =    {
        // positions   // texture coordinates
        -20,  0, -20,  0, 0,
        -20, 40, -20,  1, 0,
        -20,  0,  20,  0, 1,
        -20,  0,  20,  0, 1,
        -20, 40, -20,  1, 0,
        -20, 40,  20,  1, 1,

         20, 0, -20,  0, 0,
         20,  40, -20,  1, 0,
         20, 00,  20,  0, 1,
         20, 00,  20,  0, 1,
         20,  40, -20,  1, 0,
         20,  40,  20,  1, 1,

        -20, 00, -20,  0, 0,
         20, 00, -20,  1, 0,
        -20, 00,  20,  0, 1,
        -20, 00,  20,  0, 1,
         20, 00, -20,  1, 0,
         20, 00,  20,  1, 1,

        -20,  40, -20,  0, 0,
         20,  40, -20,  1, 0,
        -20,  40,  20,  0, 1,
        -20,  40,  20,  0, 1,
         20,  40, -20,  1, 0,
         20,  40,  20,  1, 1,

        -20, 00, -20,  0, 0,
         20, 00, -20,  1, 0,
        -20,  40, -20,  0, 1,
        -20,  40, -20,  0, 1,
         20, 00, -20,  1, 0,
         20,  40, -20,  1, 1,

        -20, 00,  20,  0, 0,
         20, 00,  20,  1, 0,
        -20,  40,  20,  0, 1,
        -20,  40,  20,  0, 1,
         20, 00,  20,  1, 0,
         20,  40,  20,  1, 1
    };

    // Enable position and texture coordinates vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), cube);
    glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), cube + 3);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisable(GL_TEXTURE_2D);*/

    glColor3ub(0,0,0);

    glTranslatef(0, 0, 0);
    Modelos[0].Render();
    DrawAABB(Modelos[0].GetAABB().Min, Modelos[0].GetAABB().Max);

    glTranslatef(0, 0, -20);
    Modelos[1].Render();
    DrawAABB(Modelos[1].GetAABB().Min, Modelos[1].GetAABB().Max);

    /** FINAL 3D **/



    /** 2D **/

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
    glOrtho(0,windowWidth,windowHeight,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Pintar en 2D

	/*glColor4f(0,0,0,0.3);

	glBegin(GL_POLYGON);
		glVertex3f(midWindowX, midWindowY-midWindowY/2, -1);
		glVertex3f(midWindowX+midWindowX/2, midWindowY, -1);
		glVertex3f(midWindowX, midWindowY+midWindowY/2, -1);
		glVertex3f(midWindowX-midWindowX/2, midWindowY, -1);
    glEnd();

    glColor3f(255,255,255);

	glBegin(GL_LINE_LOOP);
		glVertex3f(midWindowX, midWindowY-midWindowY/2, 0);
		glVertex3f(midWindowX+midWindowX/2, midWindowY, 0);
		glVertex3f(midWindowX, midWindowY+midWindowY/2, 0);
		glVertex3f(midWindowX-midWindowX/2, midWindowY, 0);
	glEnd();*/

	glPointSize(5.0);
	glColor3f(255,0,0);
	glBegin(GL_POINTS);
        glVertex3f(midWindowX, midWindowY, 0);
	glEnd();

    // Quitar proyeccion ortogonal
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	/** FINAL 2D **/

	gluLookAt(cam->getXPos(),
 	cam->getYPos(),
 	cam->getZPos(),
 	Modelos[0].GetAABB().GetCenter().x,
    Modelos[0].GetAABB().GetCenter().y,
 	Modelos[0].GetAABB().GetCenter().z,
 	0,
 	0,
 	0);

    glColor3f(255,255,255); //Sin efectos de colores
	glfwSwapBuffers(); // Swap the buffers to display the scene (so we don't have to watch it being drawn!)
	//glAccum(GL_ACCUM, 0.5f); /** EFECTO MOTION-BLUR **/
}

int main(int argc, char **argv){

	//cout << "Controls: Use WSAD and the mouse to move around!" << endl;

	// Frame counter and window settings variables
	int redBits    = 8, greenBits = 8,    blueBits    = 8;
	int alphaBits  = 8, depthBits = 24,   stencilBits = 0;

	// Flag to keep our main loop running
	bool running = true;

	// ----- Intialiase GLFW -----

	// Initialise GLFW
	if (!glfwInit() )
	{
		std::cout << "Failed to initialise GLFW!" << endl;
		glfwTerminate();
		return -1;
	}

	// Create a window
	if( !glfwOpenWindow(windowWidth, windowHeight, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, GLFW_WINDOW))
	{
		std::cout << "Failed to open window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Call our initGL function to set up our OpenGL options
	initGL();

	// Instantiate our pointer to a Camera object providing it the size of the window
	cam = new Camera(windowWidth, windowHeight);
	cam->move(0,2,0);
	cam->changeSpeed(20);

	// Set the mouse cursor to the centre of our window
	glfwSetMousePos(midWindowX, midWindowY);

	// Specify the function which should execute when a key is pressed or released
	glfwSetKeyCallback(handleKeypress);

	// Specify the function which should execute when the mouse is moved
	glfwSetMousePosCallback(handleMouseMove);

	//Cargamos las texturas en la memoria
	CargarTexturas();

	// The deltaTime variable keeps track of how much time has elapsed between one frame and the next.
	// This allows us to perform framerate independent movement i.e. the camera will move at the same
	// overall speed regardless of whether the app's running at (for example) 6fps, 60fps or 600fps!
	double deltaTime = 0.0;

	while (running)
	{
		// Calculate our camera movement
		cam->move(deltaTime);

		// Draw our scene
		drawScene();

		// exit if ESC was pressed or window was closed
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

		// Call our fpsManager to limit the FPS and get the frame duration to pass to the cam->move method
		deltaTime = fpsManager.enforceFPS();
	}

	// Clean up GLFW and exit
	glfwTerminate();

	delete cam; // Delete our pointer to the camera object

	return 0;
}

void CargarTexturas(){

    //Texturas.push_back(LoadTexture("resources/madera.tga"));
    SkyboxTexture = LoadTexture("resources/skybox.tga");

    Modelos.resize(10);
    Modelos[0].LoadFromFile("resources/katarina.obj");
    Modelos[1].LoadFromFile("resources/LD_HorseRtime02.obj");
    //Objetos.push_back(Objeto(Modelos[0]));
}
