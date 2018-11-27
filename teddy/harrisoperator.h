#ifndef HARRISOPERATOR_H
#define HARRISOPERATOR_H
#include <iostream>
#include <vector>
#include <set>
#include "face.h"
#include "vertex.h"
#include "mesh.h"

using namespace std;


class harrisoperator{
private:
    int ringSize;
    vector<int> harrisRPoints;// save the harrisresponse of each vertex
    mesh meshes;
public:
    harrisoperator();
    harrisoperator(mesh, int );
    ~harrisoperator();

    set<int> calculateNeighbourhood(int, vector<face>);
    set<int> calculateRingNeighbourhood(int, vector<face>, set<int> *); //neighbourhood considering k rings
    void calculateHarrisResponse();

};

#endif // HARRISOPERATOR_H

//calculate centroid of the neighbourhood Vk(v)
//translate the set of points to origin
//compute the best fitting plane to the translated points using PCA
//rotate the points to make normal of fitting plane in Z-axis
//fit quadratic surface to the transformed points
