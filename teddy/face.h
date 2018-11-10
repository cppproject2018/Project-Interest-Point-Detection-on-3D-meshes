#ifndef FACE_H
#define FACE_H

#include <vector>
using namespace std;

class face{
private:
    vector<int> vert;

public:
    face();
    int getv1();
    int getv2();
    int getv3();
    void addvertex(int );


};

#endif // FACE_H
