#ifndef MESH_H
#define MESH_H
#include <vector>
#include "face.h"
#include "vertex.h"

using namespace std;

class mesh{

private:
    vector<face> faces;
    vector<vertex> vertices;

public:
    mesh();
    mesh(const char*, const char*);
    ~mesh();
    void loadFromFile(const char* filename_vert, const char* filename_tri);

};

#endif // MESH_H
