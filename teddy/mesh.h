#ifndef MESH_H
#define MESH_H
#include <vector>
#include "face.h"
#include "vertex.h"

using namespace std;

class mesh{

private:
//    vector<face> faces;
//    vector<vertex> vertices;

public:
    mesh();
    mesh(const char*, const char*);
    ~mesh();
    vector<face> faces;
    vector<vertex> vertices;

    bool readFile_Intialize(const char* ,const char*);

};

#endif // MESH_H
