#include "Stuff.hpp"
#include <GL/glfw.h>
#include "Vec.hpp"
#include <iostream>

GLuint LoadTexture(const char* TextureName){

   GLuint Texture;  //variable for texture
   glGenTextures(1,&Texture); //allocate the memory for texture
   glBindTexture(GL_TEXTURE_2D,Texture); //Binding the texture

   if(glfwLoadTexture2D(TextureName, GLFW_BUILD_MIPMAPS_BIT)){
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      return Texture;
   } else return -1;
}

void DrawAABB(Vec3d a, Vec3d b){


    glColor3f( 0.0,  0.0,  0.0 );
    //glTranslatef(0,0,b.z);

    glBegin(GL_LINE_LOOP);//Esto hay que cambiarlo. Todo lo del cubo
    glVertex3f(  b.x, a.y, b.z );
    glVertex3f(  b.x,  b.y, b.z );
    glVertex3f( a.x,  b.y, b.z );
    glVertex3f( a.x, a.y, b.z );
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f( b.x, a.y, a.z );
    glVertex3f( b.x,  b.y, a.z );
    glVertex3f( b.x,  b.y,  b.z );
    glVertex3f( b.x, a.y,  b.z );
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f( a.x, a.y,  b.z );
    glVertex3f( a.x,  b.y,  b.z );
    glVertex3f( a.x,  b.y, a.z );
    glVertex3f( a.x, a.y, a.z );
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(  b.x,  b.y,  b.z );
    glVertex3f(  b.x,  b.y, a.z );
    glVertex3f( a.x,  b.y, a.z );
    glVertex3f( a.x,  b.y,  b.z );
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(  b.x, a.y, a.z );
    glVertex3f(  b.x, a.y,  b.z );
    glVertex3f( a.x, a.y,  b.z );
    glVertex3f( a.x, a.y, a.z );
    glEnd();

}

void CoutVector(Vec3d v){
    std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
}
