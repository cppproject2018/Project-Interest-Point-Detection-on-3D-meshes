#include <iostream>
#include <fstream>
#include <string>
#include "mesh.h"

mesh::mesh(){

}
mesh::mesh(const char* filename){

       //load vertices and faces
       loadFromFile(filename);

       cout<<"Size of vertices:" <<vertices.size()<<endl;
       cout<<"Size of faces:" <<faces.size()<<endl;
       cout<<faces[0].getv3()<<" vs M"<<endl;
       cout<<faces[100].getv2()<<" vs M"<<endl;
       cout<<vertices[0].getx()<<" vs M"<<endl;
       cout<<vertices[100].getx()<<" vs M"<<endl;

}
mesh::mesh(const char* filename_vert, const char* filename_tri){

       //load vertices and faces
       readFile_Intialize(filename_vert,filename_tri);

//       cout<<"Size of vertices:" <<vertices.size()<<endl;
//       cout<<"Size of faces:" <<faces.size()<<endl;
//       cout<<faces[0].getv1()<<" vs M"<<endl;
//       cout<<faces[100].getv1()<<" vs M"<<endl;
//       cout<<vertices[0].getx()<<" vs M"<<endl;
//       cout<<vertices[100].getx()<<" vs M"<<endl;

}
mesh::~mesh(){

}
bool mesh::readFile_Intialize(const char* filevert,const char* filetri)
{


    ifstream file_vert;
    file_vert.open (filevert);
    ifstream file_tri;
    file_tri.open (filetri);

    if (!(file_vert.is_open() && file_tri.is_open())) {
        cout<<"Unable to open file"<<endl;
        return false;
    }

    //load vertices first

    double xc,yc,zc;
    int idV=0;
    while (file_vert >> xc)
    {
        vertex v;
        file_vert>>yc>>zc;
        v.vIndex=idV;
        v.setxyz(xc,yc,zc);

        vertices.push_back(v);

        ++idV;

    }


    //then load faces

    int vt1,vt2,vt3;
    int idF=0;
    while (file_tri >> vt1)
    {
        face f;
        file_tri>>vt2>>vt3;

        f.fIndex=idF;
        f.addvertices(vt1,vt2,vt3);

        //create 1st ring neighbourhood, credit to source code
        vertices[vt1].addNeighbour(vt2);    vertices[vt1].addNeighbour(vt3);
        vertices[vt2].addNeighbour(vt1);    vertices[vt2].addNeighbour(vt3);
        vertices[vt3].addNeighbour(vt2);    vertices[vt3].addNeighbour(vt2);

        faces.push_back(f);

        ++idF;

    }

    file_vert.close();
    file_tri.close();

    return true;

}

//reading OFF file, credit to source code
bool mesh::loadFromFile(const char* filename){

    int numEdges, numVertices, numFaces;;

    ifstream in(filename);

    if (!(in.is_open())) {
        cout<<"Unable to open file"<<endl;
        return false;
    }

    string format;
    getline(in, format);

    in>>numVertices>>numFaces>>numEdges;
    skipline(in);


    for(int i = 0; i < numVertices; i++){
        double xc, yc, zc;
        in>>xc>>yc>>zc;
        skipline(in);
        vertex v;
        v.vIndex=i;
        v.setxyz(xc,yc,zc);

        vertices.push_back(v);
    }


    for(int i = 0;  i < numFaces; i++){
        int numVert;
        in >> numVert;

        int vt1, vt2, vt3;

        in>>vt1>>vt2>>vt3;
        skipline(in);

        face f;
        f.fIndex=i;
        f.addvertices(vt1,vt2,vt3);

        //create 1st ring neighbourhood, credit to source code
        vertices[vt1].addNeighbour(vt2);    vertices[vt1].addNeighbour(vt3);
        vertices[vt2].addNeighbour(vt1);    vertices[vt2].addNeighbour(vt3);
        vertices[vt3].addNeighbour(vt2);    vertices[vt3].addNeighbour(vt2);

        faces.push_back(f);

    }

    in.close();
    return true;

}

void mesh::skipline(istream &in){
    char c;
    while(in>>noskipws>>c && c!='\n');
    in>>skipws;
}
