//////////////////////////////////////////////////////////////////////
// imageManipulationPBO.cpp
//
// This program modifies image manipulation.cpp to store pixel data in
// a pixel buffer object.
//
// Interaction:
// Click and drag the mouse to move the texture image.
//
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
//////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#define TextureWidth 128  // Width of texture image in pixels.
#define TextureHeight 128  // Height of texture image in pixels.
#define BUFFER_OFFSET(bytes) ((char*)0 + (bytes)) // Macro to calculate offset.

#include "getbmp.h"

using namespace std;

static int width, height; // OpenGL window size.

static int rasterX = 10; // Raster x-coordinate
static int rasterY = 10; // Raster y-coordinate

static unsigned int pixelBuffer; // Pixel buffer id.

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   
   // Load the texture image into a BitMapFile.
   BitMapFile *image[1];
   image[0] = getbmp("../../Textures/number1.bmp");

   // Copy the texture image to local storage.
   static unsigned char textureImage[TextureWidth][TextureHeight][4]; 
   memcpy(textureImage, image[0]->data, TextureWidth*TextureHeight*4);

   glGenBuffers(1, &pixelBuffer); // Generate a buffer id.
   glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBuffer); // Activate a buffer for drawing.

   // Initialize buffer with pixel data from textureImage
   glBufferData(GL_PIXEL_UNPACK_BUFFER, TextureWidth*TextureHeight*4, textureImage, GL_STATIC_DRAW); 
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);

   // Current raster position.
   glRasterPos2i(rasterX, rasterY); 

   // 2D pixel array contained in the pixel buffer is drawn at current raster position.
   glDrawPixels(TextureWidth, TextureHeight, GL_RGBA, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0)); 

   glFlush(); 
}

// Mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      // Current raster position is set at the mouse click.
      rasterX = x; 
      rasterY = height - y;
   }
 
   glutPostRedisplay();
}

// Mouse motion callback routine.
void mouseMotion(int x, int y)
{
   // Current raster position is set at the mouse position.
   rasterX = x; 
   rasterY = height - y; 
   
   glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

   // Pass the size of the OpenGL window to globals.
   width = w; 
   height = h; 

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Click and drag the mouse to move the texture image." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{  
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 
   
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("imageManipulationPBO.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMouseFunc(mouseControl); 
   glutMotionFunc(mouseMotion);
  
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();  
}
