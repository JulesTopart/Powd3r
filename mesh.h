#ifndef MESH_H
#define MESH_H

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

enum FILE_FORMAT{
    ASCII,
    BINARY,
    INVALID
};

class Mesh{
public:

//----------- Constructor -----------
    Mesh();
    Mesh(const Mesh &copy);

//------------ Methods --------------

    void        draw     ();
    void        push_back(Facet f);

//----- Transformation Methods -----

    void        rotate      (float x, float y, float z  );
    void        rotate      (Vec3 v                     );
    void        scale       (float x, float y, float z  );
    void        scale       (Vec3 v                     );
    void        move        (float x, float y, float z  );
    void        move        (Vec3 v                     );
    void        normalize   ();                 ///move the 3D model coordinate to be center around COG(0,0,0)
    void        transform   (QMatrix4x4 mat);

    //          ------ Get Methods ------

    Vec3        getBBSize   ();                 ///return scaled bounding box size
    QVector3D   getPosition ();                 ///return relative position (self origin)
    QVector3D   getRotation ();                 ///return rotation arround each axis in a QVector
    QVector3D   getScale    ();                 ///return scale factor for each axis
    QString     getName     ();                 ///return name
    int         getId       ();                 ///return ID
    size_t      size        ();                 ///return number of facets

    Facets          &getMesh()          { return _facets; }
    const Facets    &getMesh() const    { return _facets; }

    Vec3 getBottomLeftVertex() const    { return _bottomLeftVertex; }
    Vec3 getUpperRightVertex() const    { return _upperRightVertex; }

    //          --------Set Methods---------

    void        setPosition (QVector3D value);  ///set relative position (self origin)
    void        setRotation (QVector3D value);  ///set rotation around each axis
    void        setScale    (QVector3D value);  ///set scale factor in each direction
    void        setName     (QString n      );  ///set name
    void        setId       (int i          );  ///set ID

//------------------------- Attribute -------------------------

protected:
    // -------- GUI purpose only --------
    QString     _name       ;
    int         _id         ;

    //-------- Geometry --------
    Facets      _facets     ;
    Vec3        _bottomLeftVertex,
                _upperRightVertex;

    //-------- OpenGL --------
    QVector3D   _position = QVector3D(0,0,0);
    QVector3D   _rotation = QVector3D(0,0,0);
    QVector3D   _scale    = QVector3D(1,1,1);

};


std::ostream& operator<<(std::ostream& out, const Vec3 p    );
std::ostream& operator<<(std::ostream& out, const Facet& t  );

FILE_FORMAT getFileFormat       (const QString &path);
FILE_FORMAT checkStlFileFormat  (const QString &path);

float      parseDouble         (std::ifstream& s);
Vec3        parseQVector        (std::ifstream& s);

Mesh        parseAscii          (const QString& stl_path, QProgressBar &pBar);
Mesh        parseBinary         (const std::string& stl_path, QProgressBar &pBar);
void triMeshSlicer(   const Mesh *meshPtr, // the const input mesh
                     QVector<LineSegment2Ds> &slicesWithLineSegments, float sliceSize);


#endif // MESH_H
