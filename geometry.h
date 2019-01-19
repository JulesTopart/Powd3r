#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QVector>
#include <QMatrix4x4>

#define PI 3.1415926535897932384626433832795
#define INTERRES 0.05

//----------------------------------------------------------------
//-------------------------- Vec3 class --------------------------
//----------------------------------------------------------------

class Vec3{
public:
//---------------- Constructor --------------
    Vec3();                     //default
    Vec3(QVector3D a_   );      // Vector from 3D Vector coord is 3D
    Vec3(float a[3]     );      // Vector from float array
    Vec3(float x_,              // Vector from 3 coord is 3D
         float y_,
         float z_       );

//----------------- Methods ---------------

    float   dotproduct  (const Vec3 &v          ) const;
    void    transform   (const QMatrix4x4 &mat  )      ;
    float   normalize   ()                        const;

//---------------- Operator --------------

    Vec3 &operator-=    (const Vec3 &pt);
    Vec3 operator-      (const Vec3 &pt);
    Vec3 operator+      (const Vec3 &pt);
    Vec3 operator/      (float a);
    Vec3 operator*      (float a);
    Vec3 operator*=     (Vec3 a);

    operator QVector3D() const; //Cast to QVector

//-------------- Attribute --------------
    float x,y,z;
};

typedef QVector<Vec3> Vec3Collection;
Vec3 operator-(const Vec3 &a, const Vec3 &b);
Vec3 operator+(const Vec3 &a, const Vec3 &b);
bool operator==(const Vec3 &a, const Vec3 &b);
bool operator<(const QVector2D&a, const QVector2D&b);

//----------------------------------------------------------------
//---------------------- LineSegment Class -----------------------
//----------------------------------------------------------------

enum CONNECTION{
    AA, //Connection problem need to be rearranged
    BB, //Connection problem need to be rearranged
    AB, //Conventionnal segment
    BA,  //Reversed segment
    NO_CONNECTION
};

class LineSegment{
public:
//------------- Constructor -----------
    LineSegment ();
    LineSegment ( Vec3 a_     , Vec3 b_         );
    LineSegment ( QVector2D a_, QVector2D b_, float z);
    LineSegment ( QVector3D a_, QVector3D b_    );
    LineSegment ( float ax_,float ay_,float az_ ,
                  float bx_,float by_,float bz_ );

//-------------- Operator -------------
    Vec3& operator [](int idx);

//-------------- Methods -------------
    QVector3D asVector3D();

    Vec3& A(){return v[0];}
    Vec3& B(){return v[1];}

    void invert(); //A becomes B and B becomes A
    bool isConnectedTo(LineSegment b);
    CONNECTION connectionType(LineSegment b);

    Vec3 getMin();
    Vec3 getMax();

    float length(){ return QVector3D((v[1] - v[0])).length();}

    Vec3 v[2];
};
//-------------- Extern Operator -------------
typedef QVector<LineSegment> LineSegments;
bool operator==(LineSegment const& a, LineSegment const& b);



//----------------------------------------------------------------
//---------------------- LineSegment2D Class -----------------------
//----------------------------------------------------------------

class LineSegment2D{
public:
//------------- Constructor -----------
    LineSegment2D ();
    LineSegment2D ( QVector2D a_     , QVector2D b);
    LineSegment2D ( float ax_,float ay_ ,
                    float bx_,float by_           );

//-------------- Operator -------------
    QVector2D& operator [](int idx);

//-------------- Methods -------------
    QVector2D asVector2D();

    QVector2D& A(){return v[0];}
    QVector2D& B(){return v[1];}

    bool isInSegmentRange2D(QVector2D point);

    QVector2D intersect2D   (LineSegment2D b);
    bool      isParralelTo  (LineSegment2D b);

    void invert(); //A becomes B and B becomes A
    bool isConnectedTo(LineSegment b        );
    CONNECTION connectionType(LineSegment b );

    QVector2D getMin();
    QVector2D getMax();

    float length(){ return QVector3D((v[1] - v[0])).length();}

    QVector2D v[2];
};
//-------------- Extern Operator -------------
typedef QVector<LineSegment2D> LineSegment2Ds;
bool operator==(LineSegment2D const& a, LineSegment2D const& b);


//----------------------------------------------------------------
//------------------------- Plane Class --------------------------
//----------------------------------------------------------------

class Plane
{
public:
//------------- Constructor -----------
    Plane() : mDistance(0) {}

//-------------- Methods -------------
    float   distance        () const;
    float   distanceToPoint (const Vec3 &vertex ) const;
    void    setNormal       (Vec3 normal        );
    void    setDistance     (float distance     );

protected:
    Vec3  mNormal;      // normalized Normal-Vector of the plane
    float mDistance;    // shortest distance from plane to Origin
};


class Facet {
public:
//------------- Constructor -----------
    Facet();
    Facet(QVector3D normalp,
          QVector3D v1p,
          QVector3D v2p,
          QVector3D v3p );

    Facet(Vec3 normalp,
          Vec3 v1p,
          Vec3 v2p,
          Vec3 v3p      );

//-------------- Operator -------------

    Facet &operator-=(const Vec3 &pt        );
    Facet &operator-=(const QVector3D &pt   );


//-------------- Methods -------------

    void transform      (const QMatrix4x4 &mat);

    // @return -1 = all triangle is on plane back side
    //          0 = plane intersects the triangle
    //          1 = all triangle is on plane front side
    //         -2 = error in function
    int intersectPlane  (const Plane &plane, LineSegment2D &ls) const;

    void scale          (Vec3 sc);

    Vec3 normal;
    Vec3 v[3];

};
typedef QVector<Facet> Facets;



class Slice{
public:
    Slice(){}
    Slice(LineSegment2Ds lines);

    LineSegment2D   get         (int i   );
    LineSegment2Ds  asLines     ();
    void            push        (LineSegment2D p);
    LineSegment2Ds  subSlice    (int DPI);
    QVector2D       getMin      ();
    QVector2D       getMax      ();

    LineSegment2Ds lines;
};
typedef QVector<Slice> Slices;

QVector<QVector2D> sortQVector2DByX(QVector<QVector2D> array);



#endif // GEOMETRY_H
