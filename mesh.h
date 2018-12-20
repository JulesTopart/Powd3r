#ifndef MESH_H
#define MESH_H

#include <string>
#include <QVector>
#include <QMainWindow>
#include <QFileDialog>
#include <QProgressBar>
#include <QTextStream>
#include <QVector3D>
#include <QVector2D>
#include <QtOpenGL>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <ctype.h>

#include "geometry.h"

namespace mesh{


enum FILE_FORMAT{
    ASCII,
    BINARY,
    INVALID
};

class Model{
public:
    Model(){}
    Facets facets;

    void draw();

    void rotate(float x, float y, float z);
    void rotate(QVector3D v);
    void scale(float x, float y, float z);
    void scale(QVector3D v);
    void move(float x, float y, float z);
    void move(QVector3D v);
    void moveTo(float x, float y, float z);
    void moveTo(QVector3D v);

    //Get Methods
    QVector3D pos();
    QVector3D rot();
    QVector3D scale();


    QString _name;
private:
    QVector3D _position = QVector3D(0,0,0);
    QVector3D _rotation= QVector3D(0,0,0);
    QVector3D _scale= QVector3D(1,1,1);
};


FILE_FORMAT getFileFormat(const QString &path);

std::ostream& operator<<(std::ostream& out, const Point p);
std::ostream& operator<<(std::ostream& out, const Facet& t);

double parseDouble(std::ifstream& s);
Point parseQVector(std::ifstream& s);

Model parseAscii(const QString& stl_path, QProgressBar &pBar);
Model parseBinary(const std::string& stl_path, QProgressBar &pBar);

FILE_FORMAT checkStlFileFormat(const QString &path);
















}


#endif // MESH_H
