#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "mesh.h"
#include "harrisoperator.h"
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    string file1="C:\\Users\\Teddyw\\Documents\\DevC++\\david0.vert";
    string file2="C:\\Users\\Teddyw\\Documents\\DevC++\\david0.tri";
    int ringNo = 5;
    mesh m(file1.c_str(),file2.c_str());
    harrisoperator hop(m,ringNo);
    hop.calculateHarrisResponse();

    return a.exec();
}
