#ifndef FACE_H
#define FACE_H
#include <vector>
using namespace std;

class face{

    private:
        vector<int> verts;

    public:
        face();
        ~face();
        int getv1();
        int getv2();
        int getv3();
        vector<int> getvertices();
        void addvertex(int );


};

#endif // FACE_H
