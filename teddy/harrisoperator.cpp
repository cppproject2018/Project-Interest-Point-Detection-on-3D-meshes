#include <iostream>
#include <set>
#include <iterator>
#include <algorithm> //for difference
#include <numeric> //for sum
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
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
set<int> harrisoperator::calculateRingNeighbourhood(int indexVertex){

    set<int> s_prev, s_current, newring, s_total, s_ring, temp;
    set<int> nbhood = meshes.vertices[indexVertex].getNeighbours();

    s_prev.insert( indexVertex); //insert the index of the vertex
    s_current.insert(nbhood.begin(),nbhood.end()); //insert the neighbourhood at ring 1

    s_total.insert(nbhood.begin(),nbhood.end()); //set of all neighbours of the vertex

    //store neighbours at each ring
    set<int> *nhd = new set<int>[ringSize];
    nhd[0].insert(nbhood.begin(), nbhood.end()); // at ring 1
    set<int> set_nhd;
    for(int j=1;j<ringSize;++j){

        set<int>::iterator itr;
        for(itr=nhd[j-1].begin(); itr!=nhd[j-1].end(); ++itr){

            set_nhd = meshes.vertices[*itr].getNeighbours();
            s_ring.insert(set_nhd.begin(),set_nhd.end()); //add neighbours of each vertex at ring j-1
//            cout<<"Ring "<<j-1<<" Iter:"<<*itr<<" : "<<(*itr)<<endl;
            set_nhd.clear();

        }
        cout<<endl;

        //calculating the difference s_ring - s_current
        set_difference(s_ring.begin(), s_ring.end(), s_current.begin(), s_current.end(),inserter(temp, temp.end()));
        //calculating the difference (s_ring - s_current) - s_prev
        set_difference(temp.begin(), temp.end(), s_prev.begin(), s_prev.end(),inserter(newring, newring.end()));

//        s_prev.clear();
        s_prev.insert(s_current.begin(), s_current.end());

//        s_current.clear();
        s_current.insert(s_ring.begin(), s_ring.end());
        s_ring.clear();

        // add it to the array of rings
        s_total.insert(newring.begin(), newring.end());
        nhd[j].insert(newring.begin(), newring.end());

    }

    delete[] nhd;

    return s_total;
}

void harrisoperator::calculateHarrisResponse(){



    unsigned int vertexSize = meshes.vertices.size();

    cout<<"start Nhd"<<endl;

    cout<<"end Nhd"<<endl;

    for(unsigned int indexVertex=0; indexVertex < vertexSize; indexVertex++){ //vertexSize

        vector<double> x_coord, y_coord, z_coord;
        //caculate the neighbourhood
        set<int> set_nhd;
        //calculate the neighbourhood of vertex of the first ring
        set_nhd=calculateRingNeighbourhood(indexVertex); //calculate the k rings neighbourhood of each vertex

        cout<<"Nhood-ring size"<<set_nhd.size()<<endl;
        cout<<"Index"<<indexVertex<<endl;

        set<int>::iterator itr;
        for(itr=set_nhd.begin(); itr!=set_nhd.end(); ++itr){
            cout<<"NHD: "<<*itr<<endl;

            //get the x,y,z coordinates
            x_coord.push_back(meshes.vertices[*itr].getx());
            y_coord.push_back(meshes.vertices[*itr].gety());
            z_coord.push_back(meshes.vertices[*itr].getz());

        }
        //adding the vertex itself to the set, the last element
        x_coord.push_back(meshes.vertices[indexVertex].getx());
        y_coord.push_back(meshes.vertices[indexVertex].gety());
        z_coord.push_back(meshes.vertices[indexVertex].getz());


        //calculate centroid of the neighbourhood Vk(v)

        unsigned int nhd_size = x_coord.size();
        cout<<"X vertex"<<x_coord[nhd_size]<<endl;
        cout<<"Y vertex"<<y_coord[nhd_size]<<endl;
        cout<<"Z vertex"<<z_coord[nhd_size]<<endl;

        double sum_x = std::accumulate(x_coord.begin(), x_coord.end(), 0.0);
        double averg_x = (double) sum_x/nhd_size;

        double sum_y = std::accumulate(y_coord.begin(), y_coord.end(), 0.0);
        double averg_y = (double) sum_y/nhd_size;

        double sum_z = std::accumulate(z_coord.begin(), z_coord.end(), 0.0);
        double averg_z = (double) sum_z/nhd_size;
        cout<<"averg X: "<<averg_x<<"\naverg Y: "<<averg_y<<"\naverg Z "<<averg_z<<endl;
        cout<<"vertex "<<indexVertex<<" X"<<meshes.vertices[indexVertex].getx()<<endl;
        cout<<"vertex Y"<<meshes.vertices[indexVertex].gety()<<endl;
        cout<<"vertex Z"<<meshes.vertices[indexVertex].getz()<<endl;

        //apply PCA to get the normal of the fitting plane
        //using Eigen


        //translate the set of points so that centroid is on the origin
        //Matrix= 3*nhd_size

        Eigen::MatrixXd nhd_matrix(3,nhd_size);
        for ( unsigned int jj = 0; jj < nhd_size; jj++ )
        {
            //store them in Matrix
            //x_trans = x_coord - x_centroid
            nhd_matrix(0, jj) = x_coord[jj] - averg_x;
            nhd_matrix(1, jj) = y_coord[jj] - averg_y;
            nhd_matrix(2, jj) = z_coord[jj] - averg_z;
//            cout<<"X_trans: "<<nhd_matrix(0, jj)<<endl;
//            cout<<"Y_trans: "<<nhd_matrix(1, jj)<<endl;
//            cout<<"Z_trans: "<<nhd_matrix(2, jj)<<endl;
        }


        //Covariance matrix C
        // 1/n-1*X*Xt
        Eigen::Matrix3d CovM;
        CovM = (nhd_matrix * nhd_matrix.transpose())/(nhd_size-1); //creates a symmetric matrix
        // Calculate EigenVectors and EigenValues of Covaraince matrix
        cout<<"Covariance Matrix"<<endl;
        cout<<CovM<<endl;

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(CovM); //SelfAdjointEigenSolver if the matrix is symmetric(faster)
        cout << "The eigenvalues of A are:" << endl << es.eigenvalues().real() << endl;
        cout << "The matrix of eigenvectors, V, is:" << endl << es.eigenvectors().real() << endl << endl;
        cout << "The eigenvalues of A Size:" << endl << es.eigenvalues().real().rows() <<"by cols: "<<es.eigenvalues().real().cols() << endl;
        cout << "The matrix of eigenvectors, size:" << endl << es.eigenvectors().real().rows() <<"by cols: "<< es.eigenvectors().real().cols() << endl << endl;


        Eigen::MatrixXd eig_values(3,1);
        eig_values= es.eigenvalues().real(); //sorted in increasing order
        Eigen::Matrix3d principal_comps = es.eigenvectors().real();

        //sort eigenvectors in decreasing order by swaping
        Eigen::MatrixXd tmp(3,1);
        tmp = principal_comps.col(0);
        principal_comps.col(0) = principal_comps.col(2);
        principal_comps.col(2) = tmp;


        // set of points is rotated so that the normal of the fitting plane is the z-axis
        Eigen::MatrixXd rotated_points(3,nhd_size);
        rotated_points = principal_comps.transpose() * nhd_matrix;

        //translate the set of points so that the point v is in the origin of the XY-plane
        double x_vertex = rotated_points(0, nhd_size-1);
        double y_vertex = rotated_points(1, nhd_size-1);
        double z_vertex = rotated_points(2, nhd_size-1);
        cout<<"Rot X_: "<<x_vertex<<endl;
        cout<<"Rot Y_: "<<y_vertex<<endl;
        cout<<"Rot Z_: "<<z_vertex<<endl;

        Eigen::MatrixXd trans_points(3,nhd_size);
        for ( int jk = 0; jk < nhd_size; jk++ )
        {
            //trans_points = rotated_points - vertex
            trans_points(0, jk)  = rotated_points(0, jk) - x_vertex;
            trans_points(1, jk) = rotated_points(1, jk)  - y_vertex;

//            trans_points(2, jk)  = rotated_points(2, jk) - z_vertex;
        }
        cout<<"vertex "<<indexVertex<<" X"<<trans_points(1, nhd_size-1)<<endl;
        cout<<"vertex Y"<<trans_points(1, nhd_size-1)<<endl;
        cout<<"vertex Z"<<trans_points(2, nhd_size-1)<<endl;
        //fit a quadratic surface to the set of transformed points
        //z = f(x,y) =p1/2*x2 +p2*x*y + p3/2*y2 +p4*x +p5*y +p6
        Eigen::MatrixXd eqns(nhd_size, 6); // equations
        Eigen::MatrixXd bvector(nhd_size, 1);
        Eigen::MatrixXd xvector(6, 1);
        for ( int kk = 0; kk < nhd_size; kk++ )
        {
            double xv = trans_points(0, kk);
            double yv = trans_points(1, kk);
            double zv = trans_points(2, kk);

            bvector(kk,0) = zv;

            eqns(kk, 0) = (xv*xv)/2; //coefficient of p1
            eqns(kk, 1) = xv*yv; //p2
            eqns(kk, 2) = (yv*yv)/2 ; //p3
            eqns(kk, 3) = xv; //p4
            eqns(kk, 4) = yv; //p5
            eqns(kk, 5) = 1; //p6
        }

        //solve the linear system Ax=b
        xvector = eqns.colPivHouseholderQr().solve(bvector);

        //extract the solution of the linear system
        double p1 = xvector(0,0);
        double p2 = xvector(1,0);
        double p3 = xvector(2,0);
        double p4 = xvector(3,0);
        double p5 = xvector(4,0);
        double p6 = xvector(5,0);

        cout<<"P1: "<<p1<<endl;
        cout<<"P2: "<<p2<<endl;
        cout<<"P3: "<<p3<<endl;
        cout<<"P4: "<<p4<<endl;
        cout<<"P5: "<<p5<<endl;
        cout<<"P6: "<<p6<<endl;

        double A = p4*p4 + 2*p1*p1 + 2*p2*p2;
        double B = p5*p5 + 2*p2*p2 + 2*p3*p3; //difference with source code p5 = p2 =0.3..
        double C = p4*p5 + 2*p1*p2 + 2*p2*p3;

        cout<<"A: "<<A<<endl;
        cout<<"B: "<<B<<endl;
        cout<<"C: "<<C<<endl;


        //Harris operator value in the point v
        double kvalue = 0.04;
        double harrisV = (A*B) - (C*C) - kvalue * ((A + B)*(A + B));
        harrisRPoints.push_back(harrisV);
        cout<<"Harris Value:"<<harrisV<<endl;


    }//endforeachvertex

        //Pre-selection of the interest points
        //preserve the vertices which are local maximum
        vector<int> preselected;
        cout<<"Preselected vertices:"<<endl;
        for(int nV = 0; nV < vertexSize; nV++){
            bool localMaxima = isLocalMaxima(nV);
            if(localMaxima == true){
                preselected.push_back(nV);
                cout<<nV<<endl;
            }
        }

        //sort the preselected vertices
        sort(preselected.begin(), preselected.end(), std::greater<int>());

        //Selecting interest points
        vector<int> selectedVertices;
        int typeselect = 1;
        if (typeselect == 1){
            //fraction_constant
            //double fraction_constant = 0.5;
            int selected_size = preselected.size(); //constant fraction
            cout<<"Interest Points are:";
            for(int ss = 0; ss < selected_size; ss++){
                selectedVertices.push_back(preselected[ss]);
                cout<<preselected[ss]<<endl;
            }
        }else{
            int selected_size = preselected.size();
            for(int ss = 0; ss < selected_size; ss++){
                if(ss)//min j∈[1,|Q|] ||Pi − Qj||2 > ρ
                    selectedVertices.push_back(preselected[ss]);
            }
        }



} //endfun

bool harrisoperator::isLocalMaxima(unsigned int vertexIndex){


    set<int> nhd = meshes.vertices[vertexIndex].getNeighbours();
    set<int>::iterator itrr;
    for(itrr=nhd.begin(); itrr!=nhd.end(); ++itrr){
        if(harrisRPoints[vertexIndex] < harrisRPoints[*itrr]){
            return false;
        }
    }
    return true;
}
