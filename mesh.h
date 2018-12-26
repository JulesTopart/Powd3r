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


class Model;

class BBox{
public:
    BBox() :
        xmin(0), ymin(0), zmin(0), xmax(0), ymax(0), zmax(0), width(0), height(0), depth(0){} // default constructor
    BBox(Model m); //BBox from mesh
    BBox(QVector3D a_ , QVector3D b_) :
        xmin(a_.x()),
        ymin(a_.y()),
        zmin(a_.z()),
        xmax(b_.x()),
        ymax(b_.y()),
        zmax(b_.z()),
        width(xmax - xmin),
        height(ymax - ymin),
        depth(zmax - zmin) {}

    QVector3D asVector3D(){ return QVector3D(width,height,depth);}   // 3D Vector for point coord

    float xmin,ymin,zmin,xmax,ymax,zmax;
    float width, height, depth; // May have to be recalculated when rotated...
};


class Model{
public:
    Model(){}
    Model(const Model &copy){
        _name = copy._name;
        id = copy.id;
        facets = copy.facets;
        bbox = copy.bbox;
        _position = copy._position;
        _rotation = copy._rotation;
        _scale    = copy._scale;
    }

    void draw();
    void rotate(float x, float y, float z);
    void rotate(QVector3D v);
    void scale(float x, float y, float z);
    void scale(QVector3D v);
    void move(float x, float y, float z);
    void move(QVector3D v);
    void moveTo(float x, float y, float z);
    void moveTo(QVector3D v);

    void calculateBBox(){bbox = BBox(*this);}

    //Get Methods
    QVector3D getPosition();
    QVector3D getRotation();
    QVector3D getScale();

    //Set Methods
    void setName(QString n);
    void setId(int i);
    void setPosition(QVector3D value);
    void setRotation(QVector3D value);
    void setScale(QVector3D value);


    QString _name;
    int id;
    Facets facets;
    BBox bbox;
private:
    QVector3D _position = QVector3D(0,0,0);
    QVector3D _rotation = QVector3D(0,0,0);
    QVector3D _scale    = QVector3D(1,1,1);
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
