#include "Vec.hpp"
#include <vector>

#include <GL/glfw.h>

#include "Objeto.hpp"

void Objeto::UpdateAABB(){

}

void Objeto::Render(){

    // Guardamos la Matriz anterior
    glPushMatrix();

    //glMatrixMode(GL_MODELVIEW);
    glTranslatef(0, 0, 0);
    glTranslatef(Posicion.x, Posicion.y, Posicion.z);
    glRotatef(Angulo.x, 1,0,0);
    glRotatef(Angulo.y, 0,1,0);
    glRotatef(Angulo.z, 0,0,1);

    // Renderizamos el Modelo
    Modelo.Render();

    // Recuperamos la antigua Matriz
    glPopMatrix();
}
