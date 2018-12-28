#include "geometry.h"

//----------------------------------------------------------------
//-------------------------- Vec3 class --------------------------
//----------------------------------------------------------------

//               ----------- Constructor ----------

Vec3::Vec3() :
    x(0.0),
    y(0.0),
    z(0.0){}

Vec3::Vec3(  float x_, float y_, float z_) :
    x(x_),
    y(y_),
    z(z_){}                   // Point from 3 coord is 3D

Vec3::Vec3(QVector3D a_) :
    x(a_.x()),
    y(a_.y()),
    z(a_.z()){}       // Point from 3D Vector coord is 3D

Vec3::Vec3(float a[3]){
    x = a[0];
    y = a[1];
    z = a[2];
}


//               ------------- Methods -----------

float
Vec3::dotproduct(const Vec3 &v) const{
    return x * v.x + y * v.y + z * v.z;
}


void
Vec3::transform (const QMatrix4x4 &mat){

    QVector4D v (x, y, z, 1.0f);
    QVector4D vt;
    vt = mat * v;
    x = vt.x();
    y = vt.y();
    z = vt.z();
}

float
Vec3::normalize () const{
    return sqrt(x * x + y * y + z * z);
}


//               ------------- Operator -----------
Vec3
&Vec3::operator-=   (const Vec3 &pt)
{
    x -= pt.x;
    y -= pt.y;
    z -= pt.z;

    return *this;
}

Vec3
Vec3::operator-     (const Vec3 &pt){
    return Vec3(x - pt.x, y - pt.y, z - pt.z);
}

Vec3
Vec3::operator+     (const Vec3 &pt){
    return Vec3(x + pt.x, y + pt.y, z + pt.z);
}

Vec3
Vec3::operator/     (float a){
    return Vec3(x / a, y / a, z / a);
}

Vec3
Vec3::operator*     (float a){
    return Vec3(x * a, y * a, z * a);
}

Vec3::operator QVector3D() const {
    return QVector3D(x,y,z);
}

//         ------------- Extern Operator -----------

Vec3 operator-(const Vec3 &a, const Vec3 &b)
{
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 operator+(const Vec3 &a, const Vec3 &b)
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}



//----------------------------------------------------------------
//---------------------- LineSegment Class -----------------------
//----------------------------------------------------------------

//              ----------- Constructor ----------
LineSegment::LineSegment ()                            : v{Vec3(), Vec3()} {}
LineSegment::LineSegment (Vec3 a_, Vec3 b_           ) : v{a_    , b_    } {}
LineSegment::LineSegment (QVector3D a_, QVector3D b_ ) : v{a_    , b_    } {}
LineSegment::LineSegment( float ax_,
      float ay_,
      float az_ ,
      float bx_,
      float by_,
      float bz_
     ) : v{Vec3(ax_, ay_, az_), Vec3(bx_, by_, bz_)}{}

//               ------------- Operator -----------

bool operator==(Vec3 const& a, Vec3 const& b)
{
    if (    a.x == b.x
         && a.y == b.y
         && a.z == b.z ) return true;
    return false;
}

bool operator==(LineSegment const& a, LineSegment const& b)
{
    if ((a.v[0] == b.v[0] && a.v[1] == b.v[1]) || (a.v[1] == b.v[0] && a.v[0] == b.v[1])) return true;
    return false;
}

Vec3& LineSegment::operator [](int idx) {
    return v[idx];
}

//               ------------- Methods -----------

QVector2D
LineSegment::asVector2D(){ return QVector2D(v[1].x - v[0].x, v[1].y - v[0].y);}

QVector3D
LineSegment::asVector3D(){ return QVector3D(v[1].x - v[0].x, v[1].y - v[0].y, v[1].z - v[0].z);}


//----------------------------------------------------------------
//------------------------- Plane Class --------------------------
//----------------------------------------------------------------

//               ------------- Methods -----------

float   Plane::distance() const {
    return mDistance;
}

float   Plane::distanceToPoint(const Vec3 &vertex ) const  {
    return QVector3D::dotProduct(vertex, mNormal) - mDistance;
}

void    Plane::setNormal(Vec3 normal        ){
    mNormal = normal;
}

void    Plane::setDistance(float distance     ){
    mDistance = distance;
}


//----------------------------------------------------------------
//------------------------- Facet Class --------------------------
//----------------------------------------------------------------

//              ----------- Constructor ----------

Facet::Facet(){};
Facet::Facet(QVector3D normalp,QVector3D v1p,QVector3D v2p,QVector3D v3p ) :
    normal(normalp), v{v1p, v2p, v3p} {}

Facet::Facet(Vec3 normalp,Vec3 v1p,Vec3 v2p,Vec3 v3p) :
    normal(normalp), v{v1p, v2p, v3p} {}

Facet &Facet::operator-=(const Vec3 &pt)
{
    v[0] -= pt;
    v[1] -= pt;
    v[2] -= pt;
    return *this;
}

Facet &Facet::operator-=(const QVector3D &pt)
{
    v[0] -= pt;
    v[1] -= pt;
    v[2] -= pt;
    return *this;
}

void Facet::transform(const QMatrix4x4 &mat)
{
    v[0].transform(mat);
    v[1].transform(mat);
    v[2].transform(mat);
}

// @return -1 = all triangle is on plane back side
//          0 = plane intersects the triangle
//          1 = all triangle is on plane front side
//         -2 = error in function
int Facet::intersectPlane(const Plane &plane, LineSegment &ls) const
{
    // a triangle has 3 vertices that construct 3 line segments
    size_t cntFront = 0, cntBack = 0;
    for (size_t j = 0; j < 3; ++j)
    {
        float distance = plane.distanceToPoint(v[j]);
        if (distance < 0)
            ++cntBack;
        else
            ++cntFront;
    }
    if (3 == cntBack)
    {
        return -1;
    }
    else if (3 == cntFront)
    {
        return 1;
    }
    size_t lines[] = {0, 1, 1, 2, 2, 0}; // CCW Triangle
    std::vector<Vec3> intersectPoints;
    for (size_t i = 0; i < 3; ++i)
    {
        const Vec3 &a = v[lines[i * 2 + 0]];
        const Vec3 &b = v[lines[i * 2 + 1]];
        const float da = plane.distanceToPoint(a);
        const float db = plane.distanceToPoint(b);
        if (da * db < 0)
        {
            const float s = da / (da - db); // intersection factor (between 0 and 1)
            Vec3 bMinusa = b - a;
            intersectPoints.push_back(a + bMinusa * s);
        }
        else if (0 == da)
        { // plane falls exactly on one of the three Triangle vertices
            if (intersectPoints.size()<2)
            intersectPoints.push_back(a);
        }
        else if (0 == db)
        { // plane falls exactly on one of the three Triangle vertices
            if (intersectPoints.size()<2)
            intersectPoints.push_back(b);
        }
    }
    if (2 == intersectPoints.size())
    {
        // Output the intersecting line segment object
        ls.v[0] = intersectPoints[0];
        ls.v[1] = intersectPoints[1];
        return 0;
    }
    return -2;
}



//----------------------------------------------------------------
//----------------------- Polygon Class --------------------------
//----------------------------------------------------------------


Polygon::Polygon(Vec3Collection points_) : points(points_){
    Vec3Collection buf;
    LineSegments lBuf;
    for(Vec3Collection::Iterator point = points_.begin(); points_.end(); point++){
        buf.push_back(*point);
        if(buf.size() == 2){
            lBuf.push_back(LineSegment(buf[0], buf[1]));
            buf.pop_front();
        }
    }
    if(buf.size() != 0){
        lBuf.push_back(LineSegment(buf[0], points_[0])); //Close the polygon
    }
    this->lines = lBuf;
}

Polygon::Polygon(LineSegments lines_) : lines(lines_){
    Vec3Collection pBuf;
    Vec3 lastBuf = lines_[0][0];
    for(LineSegments::Iterator line = lines_.begin(); lines_.end(); line++){
        if(lastBuf == line->v[0])
        pBuf.push_back(line->v[0]);
        lastBuf = line->v[1];
    }
    this->points = pBuf;
}
