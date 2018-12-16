#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector3D>
#include <QVector2D>
#include <QVector>

#define PI 3.1415926535897932384626433832795

enum cCONTEXT{
    c2D,
    c3D,
    UNDIFINED
};


class Point{
public:
    Point() :
        x(0.0), y(0.0), z(0.0), ctx(UNDIFINED){}        // default constructor
    Point(double x_,double y_) :
        x(x_), y(y_), z(0.0), ctx(c2D)        {}        // Point from 2 coord is 2D
    Point(double x_,double y_,double z_) :
        x(x_), y(y_), z(z_), ctx(c3D){}                 // Point from 3 coord is 3D
    Point(QVector2D a_) :
        x(a_.x()), y(a_.y()), z(0.0), ctx(c2D){}        // Point from 2D Vector coord is 2D
    Point(QVector3D a_) :
        x(a_.x()), y(a_.y()), z(a_.z()), ctx(c3D){}     // Point from 3D Vector coord is 3D
    Point(double a[]);
    double x,y,z;

    QVector2D asVector2D(){ return QVector2D(x,y);}     // 2D Vector for point coord
    QVector3D asVector3D(){ return QVector3D(x,y,z);}   // 3D Vector for point coord

    bool is2D(){ return ctx == c2D;}                    // Is 2D context ?
    bool is3D(){ return ctx == c3D;}                    // Is 3D context ?

    cCONTEXT ctx;
};
typedef QVector<Point> Points;


class Line{
public:
    Line() : a(Point()), b(Point()) {}
    Line(Point a_, Point b_) : a(a_), b(b_) {}
    Line(QVector2D a_, QVector2D b_) : a(a_), b(b_){}
    Line( double ax_,
          double ay_,
          double az_ ,
          double bx_,
          double by_,
          double bz_
         ) : a(Point(ax_, ay_, az_)), b(Point(bx_, by_, bz_)), ctx(c2D){}

    Line( double ax_,
          double ay_,
          double bx_,
          double by_
         ) : a(Point(ax_, ay_)), b(Point(bx_, by_)), ctx(c2D){}

    QVector2D asVector2D(){ return QVector2D(b.x - a.x, b.y - a.y);}
    QVector3D asVector3D(){ return QVector3D(b.x - a.x, b.y - a.y, b.z - a.z);}

    bool is2D(){ return ctx == c2D;}
    bool is3D(){ return ctx == c3D;}

    Point a,b;
    cCONTEXT ctx;
};
typedef QVector<Line> Lines;
bool operator==(Line const& a, Line const& b);


struct Facet {
    Point normal;
    Point v1;
    Point v2;
    Point v3;
    Facet(QVector3D normalp, QVector3D v1p, QVector3D v2p, QVector3D v3p) :
      normal(normalp), v1(v1p), v2(v2p), v3(v3p) {}
    Facet(Point normalp, Point v1p, Point v2p, Point v3p) :
      normal(normalp.asVector3D()), v1(v1p.asVector3D()), v2(v2p.asVector3D()), v3(v3p.asVector3D()) {}
    Facet(){}
};
typedef QVector<Facet> Facets;

class Polygon{
public:
    Polygon();
    Polygon(Points points_);
    Polygon(Lines lines_);

    Points points;
    Lines lines;
};


#endif // GEOMETRY_H
