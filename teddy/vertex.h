#ifndef VERTEX_H
#define VERTEX_H
class vertex{
private:
    int index;
public:
    double x,y,z;
    vertex();
    double getx();
    double gety();
    double getz();
    void setx(double );
    void sety(double );
    void setz(double );
    void setxyz(double, double, double);



};

#endif // VERTEX_H
