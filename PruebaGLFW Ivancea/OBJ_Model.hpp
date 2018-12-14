#include <iostream>
#include <vector>
#include <map>
#include <GL/glfw.h>

using namespace std;

#include "Vec.hpp"
#include "BoundingBox.hpp"

#ifndef OBJ_MODEL_HPP
#define OBJ_MODEL_HPP

struct _Material{
    string Name;
    bool HayTextura;
    GLuint Textura;

    bool HayAmbiente;
    GLfloat Ambiente[4];
    bool HayDifusa;
    GLfloat Difusa[4];
    bool HayEspecular;
    GLfloat Especular[4];
};

struct _Face{
    vector<Vec3d> Vertices;
    vector<Vec3d> Normales;
    vector<Vec2d> PuntosTextura;

    int Estilo;
    /*
       1 - v
       2 - v/vt
       3 - v//vn
       4 - v/vt/vn
    */

    _Material Material;
};

class OBJ_Model{

    private:

        vector<_Face> Faces;
        map<string, GLuint> Texturas;
        _AABB AABB;
        bool _isAABBStarted;
        double Escala;

        GLuint _LoadTexture(string);
        void LoadMTLFromFile(string, vector<_Material>&);

        void parseV(string, vector<Vec3d> &v);
        void parseVT(string, vector<Vec2d> &vt);
        void parseVN(string, vector<Vec3d> &vn);
        void parseFace(string, _Material material, vector<Vec3d> &v, vector<Vec2d> &vt, vector<Vec3d> &vn, vector<_Face> &f);

    public:

        OBJ_Model();
        OBJ_Model(const OBJ_Model&);

        void LoadFromFile(string);
        void DebugFaces()const;
        void Render()const;
        void clear();

        void InvertirX();
        void InvertirY();
        void InvertirZ();
        _AABB GetAABB()const{ return AABB;}
        Vec3d GetCenter()const;
        double GetEscala()const{ return Escala;}
        bool isAABBStarted()const;
        void SetEscala(double);
        const vector<_Face> GetFaces()const{ return Faces;}
        const map<string, GLuint> GetTexturas()const{ return Texturas;}
};

#endif // OBJ_MODEL_HPP
