#include <vector>
#include <GL/glfw.h>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

#include <cstdlib>

using namespace std;

#include "Stuff.hpp"

#include "Vec.hpp"
#include "OBJ_Model.hpp"

OBJ_Model::OBJ_Model():Escala(1),_isAABBStarted(false){};

void OBJ_Model::clear(){
    Faces.clear();
    Texturas.clear();
    AABB.Max.clear();
    AABB.Min.clear();
    Escala = 1;
}

OBJ_Model::OBJ_Model(const OBJ_Model& m){
    Faces = m.GetFaces();
    Texturas = m.GetTexturas();
    AABB = m.GetAABB();
    _isAABBStarted = m.isAABBStarted();
    Escala = m.GetEscala();
};

void OBJ_Model::LoadFromFile(std::string filename){

    vector<_Material> Materiales;
    _Material CurrentMaterial;

    vector<Vec3d> v;
    vector<Vec2d> vt;
    vector<Vec3d> vn;

    // Reseteamos el modelo anterior, si existe
    clear();

    ifstream file;
    file.open(filename);

    string linea;

    if(!file.is_open()) return;

    while(!file.eof()){

        getline(file,linea);

        // Trimeamos espacios por delante
        while(linea[0] == ' ' && linea.size() > 0){
            linea.erase(0,1);
        }

        // Trimeamos espacios por detras
        while(linea[linea.size()-1] == ' ' && linea.size() > 0){
            linea.erase(linea.size()-1,1);
        }

        if(linea.substr(0,7) == "mtllib "){   // Archivo MTL
            linea.erase(0,7);
            string ruta = filename;
            while(ruta[ruta.size()-1] != '/' && ruta.size() > 0){
                ruta.erase(ruta.size()-1,1);
            }
            ruta += linea;

            LoadMTLFromFile(ruta, Materiales);
        }

        if(linea.substr(0,7) == "usemtl "){ // Si cambiamos de Material
            linea.erase(0,7);
            // linea = nombre_del_material
            for(auto it:Materiales){
                if(it.Name == linea) CurrentMaterial = it;
            }
        }

        if(linea.substr(0,2) == "v "){   // Vertices
            parseV(linea, v);
        }

        if(linea.substr(0,3) == "vt "){  // PuntosTextura
            parseVT(linea, vt);
        }

        if(linea.substr(0,3) == "vn "){  // Normales
            parseVN(linea, vn);
        }

        if(linea.substr(0,2) == "f "){   // Caras
            parseFace(linea,CurrentMaterial,v,vt,vn,Faces);
        }
    }

    // Centramos el objeto como a Isc le gusta

    for(auto it = Faces.begin(); it!=Faces.end(); it++)
    for(auto it2 = it->Vertices.begin(); it2!=it->Vertices.end(); it2++){
        it2->x -= (AABB.Max.x+AABB.Min.x)/2;
        it2->y -= AABB.Min.y;
        it2->z -= (AABB.Max.z+AABB.Min.z)/2;
    }

    /*Ahora procesamos LAS CARAS
    for(int i=0; i<Caras.size(); i++){

        string Temp = Caras[i];

        if (Temp.substr(0,7) == "usemtl "){

                Temp.erase(0,7);
                //Buscamos entre los materiales uno con el nombre == Temp
                for(int j=0; j<Materiales.size(); j++)
                    if(Materiales[j].Name == Temp)
                        CurrentMaterial = Materiales[j];

        }else{

            Temp.erase(0,2); //Quitamos el "f "

            vector<string> Numeros;

            // Vamos a sacar el formato

            // Saco el primer trozo hasta un espacio
            int Cantidad_Numeros = 1;
            string Formato = Temp;

            Formato = Formato.substr(0,Formato.find(" "));
            // Vamos a contar las "/"
            for(int i=0; i<Formato.size(); i++)
                if(Formato[i] == '/') Cantidad_Numeros++;

            // Cantidad_Numeros = 1 -> v
            // Cantidad_Numeros = 2 -> v/vt
            // Cantidad_Numeros = 3 -> v/vt/vn
            cout << "Numeros: " << Cantidad_Numeros << endl;

            //Vamos a separar los puntos por "/"
            while(Temp.size() > 0 || Temp.find("/") != string::npos){

                if(Temp.size() > 0 && Temp.find("/") == string::npos){
                    //Recogemos el ultimo numero
                    Numeros.push_back(Temp);
                    break;
                }

                string Temp2, Temp3;
                Temp2 = Temp.substr(0,Temp.find("/"));

                if(Temp2.find(" ") != string::npos){
                    //Separamos en 2
                    Temp3 = Temp2.substr(0,Temp2.find(" "));
                    Numeros.push_back(Temp3);
                    Temp2.erase(0,Temp2.find(" ")+1);
                }

                if(Temp2 == ""){Temp2 = "0";}

                Numeros.push_back(Temp2);
                Temp.erase(0,Temp.find("/")+1);
            }

            // Con todos los datos, hacemos la MAGIA
            // std::atof(std::string)

            _Face Face_temp;

            // Vertices
            for(int j=0; j<Numeros.size(); j+=3){
                Vec3d Vec3d_temp;
                string vert_temp = Vertices[atof(Numeros[j].c_str())-1];

                // Quitamos el "v " del principio
                vert_temp.erase(0,2);

                Vec3d_temp.x = atof(vert_temp.substr(0,vert_temp.find(" ")).c_str());
                vert_temp.erase(0,vert_temp.find(" ")+1);
                Vec3d_temp.y = atof(vert_temp.substr(0,vert_temp.find(" ")).c_str());
                vert_temp.erase(0,vert_temp.find(" ")+1);
                Vec3d_temp.z = atof(vert_temp.c_str());

                if(Vec3d_temp.x<AABB.Min.x) AABB.Min.x = Vec3d_temp.x;
                if(Vec3d_temp.y<AABB.Min.y) AABB.Min.y = Vec3d_temp.y;
                if(Vec3d_temp.z<AABB.Min.z) AABB.Min.z = Vec3d_temp.z;

                if(Vec3d_temp.x>AABB.Max.x) AABB.Max.x = Vec3d_temp.x;
                if(Vec3d_temp.y>AABB.Max.y) AABB.Max.y = Vec3d_temp.y;
                if(Vec3d_temp.z>AABB.Max.z) AABB.Max.z = Vec3d_temp.z;

                Face_temp.Vertices.push_back(Vec3d_temp);
            }

            // PuntosTextura
            if(Cantidad_Numeros < 2) Face_temp.PuntosTextura.push_back(Vec2d(0,0));
            else
            for(int j=1; j<Numeros.size(); j+=3){
                Vec2d Vec2d_temp;
                string ptex_temp = PuntosTextura[atof(Numeros[j].c_str())-1];

                // Quitamos el "vt " del principio
                ptex_temp.erase(0,3);

                Vec2d_temp.x = atof(ptex_temp.substr(0,ptex_temp.find(" ")).c_str());
                ptex_temp.erase(0,ptex_temp.find(" ")+1);
                Vec2d_temp.y = atof(ptex_temp.c_str());

                Face_temp.PuntosTextura.push_back(Vec2d_temp);
            }

            // Normales
            if(Cantidad_Numeros < 3) Face_temp.Normales.push_back(Vec3d(0,0,0));
            else
            for(int j=2; j<Numeros.size(); j+=3){
                Vec3d Vec3d_temp;
                string norm_temp = Normales[atof(Numeros[j].c_str())-1];

                // Quitamos el "vn " del principio
                norm_temp.erase(0,3);

                Vec3d_temp.x = atof(norm_temp.substr(0,norm_temp.find(" ")).c_str());
                norm_temp.erase(0,norm_temp.find(" ")+1);
                Vec3d_temp.y = atof(norm_temp.substr(0,norm_temp.find(" ")).c_str());
                norm_temp.erase(0,norm_temp.find(" ")+1);
                Vec3d_temp.z = atof(norm_temp.c_str());

                Face_temp.Normales.push_back(Vec3d_temp);
            }

            // Material
            Face_temp.Material = CurrentMaterial;

            Faces.push_back(Face_temp);
        }
    }

        for(auto it = Faces.begin(); it!=Faces.end(); it++)
        for(auto it2 = it->Vertices.begin(); it2!=it->Vertices.end(); it2++){
            it2->x -= (AABB.Max.x+AABB.Min.x)/2;
            it2->y -= AABB.Min.y;
            it2->z -= (AABB.Max.z+AABB.Min.z)/2;
        }

    AABB.Max.x -= (AABB.Max.x+AABB.Min.x)/2;
    AABB.Max.y -= AABB.Min.y;
    AABB.Max.z -= (AABB.Max.z+AABB.Min.z)/2;
    AABB.Max = (AABB.Max-AABB.Min)/2;
    AABB.Max.y *= 2;
    AABB.Min = -AABB.Max;
    AABB.Min.y = 0;*/

    //DebugFaces();
}

void OBJ_Model::parseV(string line, vector<Vec3d> &v){
    v.push_back(Vec3d());
    sscanf(line.c_str(), "v %lf %lf %lf %lf", &v.back().x, &v.back().y, &v.back().z);

    if(!_isAABBStarted){
        _isAABBStarted = true;
        AABB.Min = AABB.Max = v.back();
        return;
    }
    if(v.back().x<AABB.Min.x) AABB.Min.x = v.back().x;
    if(v.back().y<AABB.Min.y) AABB.Min.y = v.back().y;
    if(v.back().z<AABB.Min.z) AABB.Min.z = v.back().z;

    if(v.back().x>AABB.Max.x) AABB.Max.x = v.back().x;
    if(v.back().y>AABB.Max.y) AABB.Max.y = v.back().y;
    if(v.back().z>AABB.Max.z) AABB.Max.z = v.back().z;
}

void OBJ_Model::parseVT(string line, vector<Vec2d> &vt){
    vt.push_back(Vec2d());
    sscanf(line.c_str(), "vt %lf %lf %lf", &vt.back().x, &vt.back().y);
}

void OBJ_Model::parseVN(string line, vector<Vec3d> &vn){
    vn.push_back(Vec3d());
    sscanf(line.c_str(), "vn %lf %lf %lf %lf", &vn.back().x, &vn.back().y, &vn.back().z);
}

void OBJ_Model::parseFace(string line, _Material Material, vector<Vec3d> &v, vector<Vec2d> &vt, vector<Vec3d> &vn, vector<_Face> &f){

    // Quitamos "f " del principio
    line.erase(0,2);

    // Creamos un vector cortando por los " "
    vector<string> Bloque;

    while(line.find(" ") != string::npos){
        //cout << line.substr(0,line.find(" ")) << endl;
        Bloque.push_back(line.substr(0,line.find(" ")));
        line.erase(0, line.find(" ")+1);
    }
    //cout << line << endl;
    Bloque.push_back(line);

    //cout << endl;

    /*for(int i = 0; i < Bloque.size(); i++){
        cout << Bloque[i] << endl;
    }
    cout << endl;*/

    int Estilo = 0;

    /* Miramos que Estilo tiene
       1 - v
       2 - v/vt
       3 - v//vn
       4 - v/vt/vn
    */

    float Basura = 0;
    int Resultado = 0;

    Resultado = sscanf(Bloque[0].c_str(), "%f/%f/%f", &Basura, &Basura, &Basura);

    if(Resultado == 3) Estilo = 4; //     v/vt/vn
    if(Resultado == 2) Estilo = 2; //     v/vt
    if(Resultado == 1){ // Puede ser o "v" o "v//vn"
        Resultado = sscanf(Bloque[0].c_str(), "%f//%f", &Basura, &Basura);
        if(Resultado == 2) Estilo = 3; //     v//vn
        if(Resultado == 1) Estilo = 1; //     v
    }

    /*cout << "Tipo: " << Estilo << endl;
    cout << endl << endl;*/

    // Hasta aqui funciona perfecto

    vector<int> Numeros;

    switch(Estilo){
        case 1:
            for(int i = 0; i < Bloque.size(); i++){
                Numeros.push_back(atoi(Bloque[i].c_str()));
            }
            break;
        case 2:
            for(int i = 0; i < Bloque.size(); i++){
                int temp[2];
                sscanf(Bloque[i].c_str(),"%i/%i",&temp[0], &temp[1]);
                Numeros.push_back(temp[0]);
                Numeros.push_back(temp[1]);
            }
            break;
        case 3:
            for(int i = 0; i < Bloque.size(); i++){
                int temp[2];
                sscanf(Bloque[i].c_str(),"%i//%i",&temp[0], &temp[1]);
                Numeros.push_back(temp[0]);
                Numeros.push_back(temp[1]);
            }
            break;
        case 4:
            for(int i = 0; i < Bloque.size(); i++){
                int temp[3];
                sscanf(Bloque[i].c_str(),"%i/%i/%i",&temp[0], &temp[1], &temp[2]);
                Numeros.push_back(temp[0]);
                Numeros.push_back(temp[1]);
                Numeros.push_back(temp[2]);
            }
            break;
        default:
            break;
    }

    /* Comprobamos los numeros
    for(int i = 0; i < Numeros.size(); i++){
        cout << Numeros[i] << " ";
    }
    cout << endl;*/

    // Hasta aqui va perfecto

    // Ahora tenemos los id de los Vertices a juntar
    // Rellenamos la Face_Temp

    _Face Face_Temp;

    Face_Temp.Estilo = Estilo;

    switch(Estilo){
        case 1:
            for(int i = 0; i < Numeros.size(); i++)
                Face_Temp.Vertices.push_back(v[Numeros[i]-1]);
            break;
        case 2:
            for(int i = 0; i < Numeros.size(); i+=2){
                Face_Temp.Vertices.push_back(v[Numeros[i]-1]);
                Face_Temp.PuntosTextura.push_back(vt[Numeros[i+1]-1]);
            }
            break;
        case 3:
            for(int i = 0; i < Numeros.size(); i+=2){
                Face_Temp.Vertices.push_back(v[Numeros[i]-1]);
                Face_Temp.Normales.push_back(vn[Numeros[i+1]-1]);
            }
            break;
        case 4:
            for(int i = 0; i < Numeros.size(); i+=3){
                //cout << "V[" << v[Numeros[i]-1].x << ", " << v[Numeros[i]-1].y << ", " << v[Numeros[i]-1].z << "]" << endl;
                Face_Temp.Vertices.push_back(v[Numeros[i]-1]);
                Face_Temp.PuntosTextura.push_back(vt[Numeros[i+1]-1]);
                Face_Temp.Normales.push_back(vn[Numeros[i+2]-1]);
            }
            break;
        default:
            break;
    }

    //cout << endl;

    Face_Temp.Material = Material;

    f.push_back(Face_Temp);

}

void OBJ_Model::LoadMTLFromFile(std::string filename, vector<_Material> &Materiales){

    // Rellenar esta struct

    /*
    struct _Textura{
        string Ruta;
        GLuint Textura;
    };

    struct _Material{
        string Name;
        _Textura *Textura;
        GLfloat Ambiente[4];
        GLfloat Difusa[4];
        GLfloat Especular[4];
    };*/

    // Name = Nombre del material
    // Textura.Ruta = RUta de la textura del material
    // Ambiente[4] = (x), (y), (z), (1.0)
    // Difusa[4] = (x), (y), (z), (1.0)
    // Especular[4] = (x), (y), (z), (1.0)

    // Cuando leemos un nuevo nombre de textura, lo añadimos al vector<_Textura> Texturas
    // Pero antes de añadirlo, comprobamos que no haya otra textura con la misma ruta

    // Una vez tengamos todos los materiales apuntando a _Textura del vector, las cargamos
    // Se podrá acceder a la textura usando _Face._Material._Textura.Textura

    // Suerte, que hoy tengo musica hasta las 19:00

    ifstream file;
    file.open(filename);

    string linea;
    _Material Mat_Temp;

    // Inicializamos los Boolean
    Mat_Temp.HayAmbiente = false;
    Mat_Temp.HayDifusa = false;
    Mat_Temp.HayEspecular = false;
    Mat_Temp.HayTextura = false;

    if(!file.is_open()) return;

    while(!file.eof()){

        getline(file,linea);

        // Trimeamos espacios por delante
        while((linea[0] == ' ' || linea[0] == '\t') && linea.size() > 0){
            linea.erase(0,1);
        }

       // Trimeamos espacios por detras
        while((linea[linea.size()-1] == ' ' || linea[linea.size()-1] == '\t') && linea.size() > 0){
            linea.erase(linea.size()-1,1);
        }

        if(linea.substr(0,3) == "Ka "){   // Ambiente

            linea.erase(0,3);

            Mat_Temp.Ambiente[0] = (float)atof(linea.substr(0,linea.find(" ")).c_str());
            linea.erase(0,linea.find(" ")+1);
            Mat_Temp.Ambiente[1] = (float)atof(linea.substr(0,linea.find(" ")).c_str());
            linea.erase(0,linea.find(" ")+1);
            Mat_Temp.Ambiente[2] = (float)atof(linea.c_str());
            Mat_Temp.Ambiente[3] = 1.0;

            Mat_Temp.HayAmbiente = true;
        }

        if(linea.substr(0,3) == "Kd "){   // Difusa

            linea.erase(0,3);

            Mat_Temp.Difusa[0] = (float)atof(linea.substr(0,linea.find(" ")).c_str());
            linea.erase(0,linea.find(" ")+1);
            Mat_Temp.Difusa[1] = (float)atof(linea.substr(0,linea.find(" ")).c_str());
            linea.erase(0,linea.find(" ")+1);
            Mat_Temp.Difusa[2] = (float)atof(linea.c_str());
            Mat_Temp.Difusa[3] = 1.0;

            Mat_Temp.HayDifusa = true;
        }

        if(linea.substr(0,3) == "Ks "){   // Especular

            linea.erase(0,3);

            Mat_Temp.Especular[0] = (float)atof(linea.substr(0,linea.find(" ")).c_str());
            linea.erase(0,linea.find(" ")+1);
            Mat_Temp.Especular[1] = (float)atof(linea.substr(0,linea.find(" ")).c_str());
            linea.erase(0,linea.find(" ")+1);
            Mat_Temp.Especular[2] = (float)atof(linea.c_str());
            Mat_Temp.Especular[3] = 1.0;

            Mat_Temp.HayEspecular = true;
        }

        if(linea.substr(0,7) == "map_Kd "){   // Textura

            linea.erase(0,7);
            string ruta = filename;
            while(ruta[ruta.size()-1] != '/' && ruta.size() > 0){
                ruta.erase(ruta.size()-1,1);
            }
            ruta += linea;

            Mat_Temp.Textura = _LoadTexture(ruta);
            Mat_Temp.HayTextura = true;
        }

        if(linea.substr(0,7) == "newmtl "){   // Nuevo Material

            // Si habia un material anterior, lo guardamos
            if(Mat_Temp.Name != "")
                    Materiales.push_back(Mat_Temp);

            // Reseteamos el Mat_Temp
            for(int i=0; i<4; i++){
                Mat_Temp.Ambiente[i] = 0;
                Mat_Temp.Difusa[i] = 0;
                Mat_Temp.Especular[i] = 0;
            }

            Mat_Temp.HayAmbiente = false;
            Mat_Temp.HayDifusa = false;
            Mat_Temp.HayEspecular = false;
            Mat_Temp.HayTextura = false;

            // Aplicamos el nombre al material
            linea.erase(0,7);
            Mat_Temp.Name = linea;
        }
    }

    // Añadimos el ultimo material
    Materiales.push_back(Mat_Temp);

    /*for(int i=0; i<Materiales.size(); i++){

        cout << "-----------------------------------" << endl;

        cout << " Material: " << Materiales[i].Name << endl;

        cout << " Ka ";
        for(int i=0; i<4; i++)
            cout << Materiales[i].Ambiente[i] << " , ";
        cout << endl;

        cout << " Kd ";
        for(int i=0; i<4; i++)
            cout << Materiales[i].Difusa[i] << " , ";
        cout << endl;

        cout << " Ks ";
        for(int i=0; i<4; i++)
            cout << Materiales[i].Especular[i] << " , ";
        cout << endl;

        cout << " Ruta: " << Materiales[i].Textura.Ruta << endl;
    }*/

}

void OBJ_Model::DebugFaces()const{

    cout << "-----------------------------------" << endl;
    cout << "        Cantidad de Caras: " << Faces.size() << endl;

    for(int i=0; i<Faces.size(); i++){

        cout << "-----------------------------------" << endl;
        cout << "              Cara " << i << endl;
        cout << "-----------------------------------" << endl;

        for(int j=0; j<Faces[i].Vertices.size(); j++){
            cout << " v" << j << "  >> ";
            cout << Faces[i].Vertices[j].x << " , ";
            cout << Faces[i].Vertices[j].y << " , ";
            cout << Faces[i].Vertices[j].z << endl;
        }
        cout << "-----------------------------------" << endl;

        for(int j=0; j<Faces[i].PuntosTextura.size(); j++){
            cout << " vt" << j << " >> ";
            cout << Faces[i].PuntosTextura[j].x << " , ";
            cout << Faces[i].PuntosTextura[j].y << endl;
        }
        cout << "-----------------------------------" << endl;


        for(int j=0; j<Faces[i].Normales.size(); j++){
            cout << " vn" << j << " >> ";
            cout << Faces[i].Normales[j].x << " , ";
            cout << Faces[i].Normales[j].y << " , ";
            cout << Faces[i].Normales[j].z << endl;
        }
        cout << "-----------------------------------" << endl;

        cout << " Material: " << Faces[i].Material.Name << endl;

        cout << " Ka ";
        for(int j=0; j<4; j++)
            cout << Faces[i].Material.Ambiente[j] << " , ";
        cout << endl;

        cout << " Kd ";
        for(int j=0; j<4; j++)
            cout << Faces[i].Material.Difusa[j] << " , ";
        cout << endl;

        cout << " Ks ";
        for(int j=0; j<4; j++)
            cout << Faces[i].Material.Especular[j] << " , ";
        cout << endl;

    }

}

void OBJ_Model::Render()const{

    //srand(time(NULL));

    for(int i=0; i<Faces.size(); i++){

        //glEnable(GL_LIGHTING);
        glColor3f(255,255,255);

        //glEnable(GL_LIGHTING);
        if(Faces[i].Material.HayAmbiente)
            glMaterialfv(GL_FRONT, GL_AMBIENT, Faces[i].Material.Ambiente);
        if(Faces[i].Material.HayDifusa)
            glMaterialfv(GL_FRONT, GL_DIFFUSE, Faces[i].Material.Difusa);
        if(Faces[i].Material.HayEspecular)
            glMaterialfv(GL_FRONT, GL_SPECULAR, Faces[i].Material.Especular);
        // El brillo especular pequeño (0 - 255).
        glMaterialf(GL_FRONT, GL_SHININESS, 20);

        if(Faces[i].Material.HayTextura){
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, Faces[i].Material.Textura);
        }else{
            glColor3f(Faces[i].Material.Difusa[0], Faces[i].Material.Difusa[1], Faces[i].Material.Difusa[2]);
        }

        glBegin(GL_POLYGON);
        for(int j=0; j<Faces[i].Vertices.size(); j++){
            // Texturas
            if(Faces[i].Estilo == 2 || Faces[i].Estilo == 4)
                glTexCoord2f(Faces[i].PuntosTextura[j].x, Faces[i].PuntosTextura[j].y);
            // Normales
            if(Faces[i].Estilo == 3 || Faces[i].Estilo == 4)
                glNormal3d(Faces[i].Normales[j].x, Faces[i].Normales[j].y, Faces[i].Normales[j].z);
            // Vertices
            glVertex3d(Faces[i].Vertices[j].x, Faces[i].Vertices[j].y, Faces[i].Vertices[j].z);
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);

        //glDisable(GL_LIGHTING);

        /*glColor3f(0,255,0);
        glLineWidth(3);
        glBegin(GL_LINE_LOOP);
        for(int j=0; j<Faces[i].Vertices.size(); j++){
            // Texturas
            if(Faces[i].Estilo == 2 || Faces[i].Estilo == 4)
                glTexCoord2f(Faces[i].PuntosTextura[j].x, Faces[i].PuntosTextura[j].y);
            // Normales
            if(Faces[i].Estilo == 3 || Faces[i].Estilo == 4)
                glNormal3d(Faces[i].Normales[j].x, Faces[i].Normales[j].y, Faces[i].Normales[j].z);
            // Vertices
            glVertex3d(Faces[i].Vertices[j].x, Faces[i].Vertices[j].y, Faces[i].Vertices[j].z);
        }
        glEnd();
        glLineWidth(1);*/
    }
}

GLuint OBJ_Model::_LoadTexture(std::string TextureName){

    if(TextureName != ""){


        if(Texturas.find(TextureName) == Texturas.end()){
            // No encontrado
            Texturas[TextureName] = LoadTexture(TextureName.c_str());
        }

        //cout << Texturas[TextureName] << endl;
        return Texturas[TextureName];

    }else{
        return -1;
    }
}

void OBJ_Model::InvertirX(){
    double t = AABB.GetCenter().x;
    for(auto it0=Faces.begin(); it0!=Faces.end(); it0++)
        for(auto it=it0->Vertices.begin(); it!=it0->Vertices.end(); it++)
            it->x = 2*t - it->x;
}

void OBJ_Model::InvertirY(){
    double t = AABB.GetCenter().y;
    for(auto it0=Faces.begin(); it0!=Faces.end(); it0++)
        for(auto it=it0->Vertices.begin(); it!=it0->Vertices.end(); it++)
            it->y = 2*t - it->y;
}

void OBJ_Model::InvertirZ(){
    double t = AABB.GetCenter().z;
    for(auto it0=Faces.begin(); it0!=Faces.end(); it0++)
        for(auto it=it0->Vertices.begin(); it!=it0->Vertices.end(); it++)
            it->z = 2*t - it->z;
}

void OBJ_Model::SetEscala(double esc){
    double t = esc/Escala;
    for(auto it0=Faces.begin(); it0!=Faces.end(); it0++)
        for(auto it=it0->Vertices.begin(); it!=it0->Vertices.end(); it++)
            *it *=t;
    Escala = esc;
    AABB.Max *=t;
    AABB.Min *=t;
}

Vec3d OBJ_Model::GetCenter()const{
    Vec3d t = (AABB.Max+AABB.Min)/2;
    t.y = GetAABB().Min.y;
    return t;
}

bool OBJ_Model::isAABBStarted()const{
    return _isAABBStarted;
}
