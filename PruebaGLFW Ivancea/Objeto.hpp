#include "Vec.hpp"
#include <vector>

#include <GL/glfw.h>

#include "OBJ_Model.hpp"

#ifndef OBJETO_HPP
#define OBJETO_HPP

class Objeto{

    private:

        // Colocación
        Vec3d Posicion;
        Vec3d Angulo;

        // Colisiones
        bool Colision;

    public:

        OBJ_Model Modelo;
        _AABB AABB;
        _BB BB;

        // Funciones
        Objeto(){ Posicion.clear(); Angulo.clear(); Colision = false;}

        // Getters
        Vec3d GetPosicion(){ return Posicion;}
        Vec3d GetAngulo(){ return Angulo;}
        bool GetColision(){ return Colision;}

        void Move(Vec3d _Posicion){ Posicion = _Posicion;}
        void Move(double x, double y, double z){ Posicion = Vec3d(x,y,z);}

        void Rotate(Vec3d _Angulo){ Angulo = _Angulo;}
        void Rotate(double x,double y, double z){ Angulo = Vec3d(x,y,z);}

        void LoadModel(OBJ_Model _Modelo){Modelo = _Modelo;}

        void UpdateAABB();

        void Render();
};

#endif // OBJETO_HPP
