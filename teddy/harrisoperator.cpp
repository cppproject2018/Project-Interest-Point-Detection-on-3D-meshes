#include <iostream>
#include <set>
#include <iterator>
#include <algorithm> //for difference
#include <numeric> //for sum
#include "harrisoperator.h"

using namespace std;

harrisoperator::harrisoperator(){
//    calculateHarrisResponse();
}
harrisoperator::harrisoperator(mesh msh, int rSize){
    meshes = msh;
    ringSize = rSize;
}
harrisoperator::~harrisoperator(){

}
//calculate the neighbourhood at ring 1
set<int> harrisoperator::calculateNeighbourhood(int indexVertex, vector<face> fc){

    set<int> neighbour;
    unsigned int fcSize = fc.size(); //getting the size of the face

    //checking if the face has the vertex
    for(unsigned int i=0; i < fcSize; i++){

        if (fc[i].getv1() == indexVertex){

            neighbour.insert(fc[i].getv2());
            neighbour.insert(fc[i].getv3());

        }if (fc[i].getv2() == indexVertex){

            neighbour.insert(fc[i].getv1());
            neighbour.insert(fc[i].getv3());

        }if (fc[i].getv3() == indexVertex){

            neighbour.insert(fc[i].getv1());
            neighbour.insert(fc[i].getv2());

        }

    }
    return neighbour;
}
//calculate the neighbourhood at ring N
set<int> harrisoperator::calculateRingNeighbourhood(int indexVertex, vector<face> fc, set<int> *nbhood){

    set<int> s_prev, s_current, newring, s_total, s_ring, temp;

    s_prev.insert( indexVertex); //insert the index of the vertex
    s_current.insert(nbhood[indexVertex-1].begin(),nbhood[indexVertex-1].end()); //insert the neighbourhood at ring 1

    s_total.insert(nbhood[indexVertex-1].begin(),nbhood[indexVertex-1].end()); //set of all neighbours of the vertex

    //store neighbours at each ring
    set<int> *nhd = new set<int>[ringSize];
    nhd[0].insert(nbhood[indexVertex-1].begin(), nbhood[indexVertex-1].end()); // at ring 1

    for(int j=2;j<=ringSize;j++){
        set<int>::iterator itr;
        for(itr=nhd[j-2].begin(); itr!=nhd[j-2].end(); ++itr){
            s_ring.insert(nbhood[(*itr)-1].begin(),nbhood[(*itr)-1].end()); //add neighbours of each vertex at ring j-1
            cout<<"Ring "<<j<<" Iter:"<<*itr<<" : "<<(*itr)-1<<endl;
        }
        cout<<endl;
        //calculating the difference s_ring - s_current
        set_difference(s_ring.begin(), s_ring.end(), s_current.begin(), s_current.end(),inserter(temp, temp.end()));
        //calculating the difference (s_ring - s_current) - s_prev
        set_difference(temp.begin(), temp.end(), s_prev.begin(), s_prev.end(),inserter(newring, newring.end()));

        s_prev.clear();
        s_prev.insert(s_current.begin(), s_current.end());

        s_current.clear();
        s_current.insert(s_ring.begin(), s_ring.end());

        // add it to the array of rings
        s_total.insert(newring.begin(), newring.end());
        nhd[j-1].insert(newring.begin(), newring.end());
    }

    delete[] nhd;

    return s_total;
}

void harrisoperator::calculateHarrisResponse(){



    unsigned int vertexSize = meshes.vertices.size();
    vector<double> x_coord, y_coord, z_coord;    

    //first calculate ring 1 of each vertices and store them  and
    //when you calculate n-k rings call the stored ring
    set<int> *nhdV = new set<int>[vertexSize];
    cout<<"start Nhd"<<endl;
    for(unsigned int i=0;i < vertexSize; i++){
        set<int> nbhoodVertex = calculateNeighbourhood(i+1, meshes.faces);
        nhdV[i].insert(nbhoodVertex.begin(),nbhoodVertex.end()); //add neighbours of each vertex at ring
    }
    cout<<"end Nhd"<<endl;
    for(unsigned int indexVertex=0;indexVertex < vertexSize; indexVertex++){ //vertexSize
        //get the x,y,z coordinates
        x_coord.push_back(meshes.vertices[indexVertex].getx());
        y_coord.push_back(meshes.vertices[indexVertex].gety());
        z_coord.push_back(meshes.vertices[indexVertex].getz());


        //caculate the neighbourhood
        set<int> nhd_k;
         //calculate the neighbourhood of vertex of the first ring
        nhd_k=calculateRingNeighbourhood(indexVertex+1, meshes.faces, nhdV); //calculate the k rings neighbourhood of each vertex

        cout<<"Nhood-ring size"<<nhd_k.size()<<endl;
        cout<<"Index"<<indexVertex<<endl;
        set<int>::iterator itr;
        for(itr=nhd_k.begin(); itr!=nhd_k.end(); ++itr)
            cout<<"NHD: "<<*itr<<endl;

        //calculate centroid of the neighbourhood Vk(v)
    /*
        double sum_x = std::accumulate(x_coord.begin(), x_coord.end(), 0.0);
        double averg_x = (double) sum_x/vertexSize;

        double sum_y = std::accumulate(y_coord.begin(), y_coord.end(), 0.0);
        double averg_y = (double) sum_y/vertexSize;

        double sum_z = std::accumulate(z_coord.begin(), z_coord.end(), 0.0);
        double averg_z = (double) sum_z/vertexSize;

        //translate the set of points so that centroid is on the origin
        vector<double> x_trans, y_trans, z_trans;
        for ( int jj = 0; jj < vertexSize; jj++ )
        {
           //x_trans = x_coord - x_centroid
           x_trans.push_back(x_coord[jj] - averg_x);
           y_trans.push_back(y_coord[jj] - averg_y);
           z_trans.push_back(z_coord[jj] - averg_z);
        }
    */
        //apply PCA to get the normal of the fitting plane
        //using Eigen, GSL or Pointcloud

    }

}
