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

Vec3
Vec3::operator*=    (Vec3 a){
    x *= a.x;
    y *= a.y;
    z *= a.z;

    return *this;
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

bool operator==(const Vec3 &a, const Vec3 &b){
    //INTERRES
    if(!(a.x < b.x + INTERRES && a.x > b.x - INTERRES)) return false;
    if(!(a.y < b.y + INTERRES && a.y > b.y - INTERRES)) return false;
    if(!(a.z < b.z + INTERRES && a.z > b.z - INTERRES)) return false;
    return true;
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

LineSegment::LineSegment (QVector2D a_, QVector2D b_, float z){
    *this = LineSegment(a_.x(), z, a_.y(), b_.x(), z, b_.y());
}


//               ------------- Operator -----------

bool operator==(LineSegment const& a, LineSegment const& b)
{
    if ((a.v[0] == b.v[0] && a.v[1] == b.v[1]) || (a.v[1] == b.v[0] && a.v[0] == b.v[1])) return true;
    return false;
}

Vec3& LineSegment::operator [](int idx) {
    return v[idx];
}

//               ------------- Methods -----------

QVector3D
LineSegment::asVector3D(){ return QVector3D(v[1].x - v[0].x, v[1].y - v[0].y, v[1].z - v[0].z);}

bool LineSegment::isConnectedTo(LineSegment b){
    if(b.A() == v[0] || b.B() == v[0] || b.A() == v[1] || b.B() == v[1]) return true;
    return false;
}

CONNECTION LineSegment::connectionType(LineSegment b){
    if( b.A() == v[0] ){
        return AA;
    }else if(b.B() == v[0]){
        return AB;
    }else if(b.A() == v[1]){
        return BA;
    }else if(b.B() == v[1]){
        return BB;
    }else{
        return NO_CONNECTION;
    }
}


Vec3 LineSegment::getMin(){
    float xmin = A().x, ymin = A().y, zmin = A().z;
    if(B().x < xmin) xmin = B().x;
    if(B().y < ymin) ymin = B().y;
    if(B().z < zmin) zmin = B().z;
    return Vec3(xmin, ymin, zmin);
}

Vec3 LineSegment::getMax(){
    float xmax = A().x, ymax = A().y, zmax = A().z;
    if(B().x < xmax) xmax = B().x;
    if(B().y < ymax) ymax = B().y;
    if(B().z < zmax) zmax = B().z;
    return Vec3(xmax, ymax, zmax);
}

void LineSegment::invert(){
    Vec3 buf = v[0];
    v[0] = v[1];
    v[1] = buf;
}



//----------------------------------------------------------------
//---------------------- LineSegment2D Class -----------------------
//----------------------------------------------------------------

//              ----------- Constructor ----------
LineSegment2D::LineSegment2D (): v{QVector2D(), QVector2D()} {}
LineSegment2D::LineSegment2D (QVector2D a_, QVector2D b_) : v{a_, b_} {}
LineSegment2D::LineSegment2D( float ax_,
      float ay_,
      float bx_,
      float by_
     ) : v{QVector2D(ax_, ay_), QVector2D(bx_, by_)}{}

//               ------------- Operator -----------

bool operator==(LineSegment2D const& a, LineSegment2D const& b)
{
    if ((a.v[0] == b.v[0] && a.v[1] == b.v[1]) || (a.v[1] == b.v[0] && a.v[0] == b.v[1])) return true;
    return false;
}

QVector2D& LineSegment2D::operator [](int idx) {
    return v[idx];
}

//               ------------- Methods -----------

QVector2D
LineSegment2D::asVector2D(){ return QVector2D(v[1].x() - v[0].x(), v[1].y() - v[0].y());}


bool LineSegment2D::isConnectedTo(LineSegment b){
    if(b.A() == v[0] || b.B() == v[0] || b.A() == v[1] || b.B() == v[1]) return true;
    return false;
}

CONNECTION LineSegment2D::connectionType(LineSegment b){
    if( b.A() == v[0] ){
        return AA;
    }else if(b.B() == v[0]){
        return AB;
    }else if(b.A() == v[1]){
        return BA;
    }else if(b.B() == v[1]){
        return BB;
    }else{
        return NO_CONNECTION;
    }
}

QVector2D LineSegment2D::intersect2D(LineSegment2D b)
{

    // Line AB represented as a1x + b1y = c1
    double a1 = B().y() - A().y();
    double b1 = A().x()- B().x();
    double c1 = a1*(A().x()) + b1*(A().y());

    // Line CD represented as a2x + b2y = c2
    double a2 = b.B().y() - b.A().y();
    double b2 = b.A().x() - b.B().x();
    double c2 = a2*(b.A().x())+ b2*(b.A().y());

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        return QVector2D();
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return QVector2D(x, y);
    }
}

bool LineSegment2D::isParralelTo(LineSegment2D b){
    // Line AB represented as a1x + b1y = c1
    double a1 = B().y() - A().y();
    double b1 = A().x()- B().x();
    double c1 = a1*(A().x()) + b1*(A().y());

    // Line CD represented as a2x + b2y = c2
    double a2 = b.B().y() - b.A().y();
    double b2 = b.A().x() - b.B().x();
    double c2 = a2*(b.A().x())+ b2*(b.A().y());

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        return true;

    }
    return false;
}

QVector2D LineSegment2D::getMin(){
    float xmin = A().x(), ymin = A().y();
    if(B().x() < xmin) xmin = B().x();
    if(B().y() < ymin) ymin = B().y();
    return QVector2D(xmin, ymin);
}

QVector2D LineSegment2D::getMax(){
    float xmax = A().x(), ymax = A().y();
    if(B().x() > xmax) xmax = B().x();
    if(B().y() > ymax) ymax = B().y();
    return QVector2D(xmax, ymax);
}

bool LineSegment2D::isInSegmentRange2D(QVector2D point){
    if( point.x() >= getMin().x() &&
        point.y() >= getMin().y() &&
        point.x() <= getMax().x() &&
        point.y() <= getMax().y()) return true;
    return false;
}

void LineSegment2D::invert(){
    QVector2D buf = v[0];
    v[0] = v[1];
    v[1] = buf;
}


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
int Facet::intersectPlane(const Plane &plane, LineSegment2D &ls) const
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
        ls.v[0] = QVector2D(intersectPoints[0].x, intersectPoints[0].y);
        ls.v[1] = QVector2D(intersectPoints[1].x, intersectPoints[1].y);
        return 0;
    }
    return -2;
}

void Facet::scale(Vec3 r){
    for(int i(0); i < 3 ; i++)
        v[i] *= r;
}


//----------------------------------------------------------------
//------------------------ Slice Class ---------------------------
//----------------------------------------------------------------

Slice::Slice( LineSegment2Ds lines_ ){
    lines = lines_;
}

LineSegment2D Slice::get(int i){
    if(i < lines.size() && i > 0) return lines[i];
    return LineSegment2D();
}

void Slice::push(LineSegment2D p){
    lines.push_back(p);
}

LineSegment2Ds Slice::asLines(){
    return lines;
}

QVector2D Slice::getMin(){
    if(lines.size() > 0){
        float xmin = lines[0].A().x(), ymin = lines[0].A().y();
        for(int i(0); i < lines.size(); i++){
            if(lines[i].A().x() < xmin) xmin = lines[i].A().x();
            if(lines[i].A().y() < ymin) ymin = lines[i].A().y();
            if(lines[i].B().x() < xmin) xmin = lines[i].B().x();
            if(lines[i].B().y() < ymin) ymin = lines[i].B().y();
        }
        return QVector2D(xmin, ymin);
    }
    return QVector2D();
}

QVector2D Slice::getMax(){
    if(lines.size() > 0){
        float xmax = lines[0].A().x(), ymax = lines[0].A().y();
        for(int i(0); i < lines.size(); i++){
            if(lines[i].A().x() > xmax) xmax = lines[i].A().x();
            if(lines[i].A().y() > ymax) ymax = lines[i].A().y();
            if(lines[i].B().x() > xmax) xmax = lines[i].B().x();
            if(lines[i].B().y() > ymax) ymax = lines[i].B().y();
        }
        return QVector2D(xmax, ymax);
    }
    return QVector2D();
}




LineSegment2Ds Slice::subSlice(int DPI){
    QVector2D min = getMin();
    QVector2D max = getMax();
    QVector2D size = getMax() - getMin();
    float res = 25.4/float(DPI);
    int n = size.y() / res;
    QVector<QVector2D> intersectionPoints;
    LineSegment2Ds insideLines;

    for(int k(1); k <= n; k++){
        LineSegment2D HLine(min.x() - res , k*res + min.y(), max.x() + res, k*res + min.y()); //Intersection line
        for(int i(0); i < lines.size(); i++){
            if(!lines[i].isParralelTo(HLine)){
                LineSegment2D curLine = lines[i];
                QVector2D inter = curLine.intersect2D(HLine);
                if(curLine.isInSegmentRange2D(inter)) intersectionPoints.push_back(inter);
            }
        }
        intersectionPoints = sortQVector2DByX(intersectionPoints);
        QVector<QVector2D> buf;
        for(QVector<QVector2D>::Iterator point = intersectionPoints.begin(); point != intersectionPoints.end(); point++){
            buf.push_back(*point);
            if(buf.size() == 2){
                insideLines.push_back(LineSegment2D(buf[0], buf[1]));
                buf.clear();
            }
        }
        buf.clear();
        intersectionPoints.clear();
    }




    return insideLines;
}

QVector2D getMinX(QVector<QVector2D> array){
    QVector2D output;
    if(array.size() > 0){
        output = array[0];
        for(QVector<QVector2D>::Iterator point = array.begin(); point != array.end(); point++){
            if(point->x() < output.x()) output = *point;
        }
    }
    return output;
}

int getMinXIndex(QVector<QVector2D> array){
    QVector2D output;
    int  index(-1);
    if(array.size() > 0){
        output = array[0];
        int i(0);
        index = 0;
        for(QVector<QVector2D>::Iterator point = array.begin(); point != array.end(); point++, i++){
            if(point->x() < output.x()){
                index = i;
                output = *point;

            }
        }
    }
    return index;
}


QVector<QVector2D> sortQVector2DByX(QVector<QVector2D> array){
    QVector<QVector2D> buf;
    while(array.size() > 0){
        int index = getMinXIndex(array);
        if( index > -1 && index < array.size()){
            buf.push_back(array[index]);
            array.remove(index);
        }
    }
    return buf;
}
