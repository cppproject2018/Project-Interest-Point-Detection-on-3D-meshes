#ifndef MESH_H
#define MESH_H
#include <vector>
#include <fstream>
#include "face.h"
#include "vertex.h"

using namespace std;

class mesh{

private:
//    vector<face> faces;
//    vector<vertex> vertices;

public:
    mesh();
    mesh(const char*);
    mesh(const char*, const char*);
    ~mesh();
    vector<face> faces;
    vector<vertex> vertices;


    bool readFile_Intialize(const char* ,const char*); //for tri, vert file
    bool loadFromFile(const char* ); //for OFF file

    void skipline(istream &);

};

#endif // MESH_H
