#include "mesh.h"

//-----------------------------------------------------------------
//-------------------------- Mesh class ---------------------------
//-----------------------------------------------------------------

//------------------------- Constructor ---------------------------

Mesh::Mesh(){
    _bb = BoundingBox();
}
Mesh::Mesh(const Mesh &copy) :  _name               (copy._name     ),
                                _id                 (copy._id       ),
                                _facets             (copy._facets   ),
                                _bb                 (copy._bb       ),
                                _position           (copy._position ),
                                _rotation           (copy._rotation ),
                                _scale              (copy._scale    ){}

//--------------------------- Methods -----------------------------


void Mesh::draw(){


    glTranslatef(_position.x(),
                 _position.y(),
                 _position.z());

    glRotatef(_rotation.x(), 1, 0, 0);
    glRotatef(_rotation.y(), 0, 1, 0);
    glRotatef(_rotation.z(), 0, 0, 1);

    glScalef(_scale.x(), _scale.y(), _scale.z());

    if(_facets.size() > 0){
        GLdouble x,y,z;
        for(Facets::Iterator facet = _facets.begin(); facet != _facets.end(); facet++){
            glBegin(GL_TRIANGLES);
                x = GLdouble(facet->normal.x);
                y = GLdouble(facet->normal.y);
                z = GLdouble(facet->normal.z);
                  glNormal3d(x, y, z);

              for(int i(0); i < 3; i++){
                  x = GLdouble(facet->v[i].x);
                  y = GLdouble(facet->v[i].y);
                  z = GLdouble(facet->v[i].z);
                    glVertex3d(x, y, z);
              }


            glEnd();
        }
    }
}

void Mesh::drawBB(){
    _bb.draw();
}

void Mesh::updateBB(){
    _bb.calculate(this);

}

void Mesh::push_back(Facet t){
    _facets.push_back(t);
}

//----------------- Transformation Methods --------------------

//Thes methods should modify mesh
void Mesh::rotate(float x, float y, float z){
    _rotation += Vec3(x,y,z);
}

void Mesh::rotate(Vec3 v){
    _rotation += v;
}

void Mesh::scale(float x, float y, float z){
    Vec3 value = Vec3(x,y,z);
    for(size_t i(0); i < this->size(); i++){
       _facets[int(i)].scale(value);
    }
}

void Mesh::scale(Vec3 v){
    for(size_t i(0); i < this->size(); i++){
       _facets[int(i)].scale(v);
    }
}

void Mesh::move(float x, float y, float z){
    _position += Vec3(x,y,z);
}

void Mesh::move(Vec3 v){
    _position += v;
}

void Mesh::applyChange(){
    _bb.calculate(this);
}

/************************************
 *    Danger Zone de magie noire    *
 ************************************/
               /*||*/
               /*||*/
               /*||*/
               /*||*/
               /*||*/
               /*||*/

//Normalize STL to place origin at center
void Mesh::normalize(){
    _bb.calculate(this);
    Vec3 halfBbox = _bb.getHalfBox();
    Vec3 start = _bb.getBottomLeft() + halfBbox;
    for (size_t i = 0; i < size(); ++i)
    {
        _facets[int(i)] -= start;
    }
    _bb.calculate(this);
}

void Mesh::transform (QMatrix4x4 mat){
    for (size_t i = 0; i < size(); ++i)
    {
        Facet &triangle = _facets[int(i)];
        triangle.transform(mat);
     }
}


//---------------------- Get Methods -------------------------

BoundingBox Mesh::getBoundingBox(){
    _bb.calculate(this);
    return _bb;
}

Vec3 Mesh::getBBSize(){
    _bb.calculate(this);
    return _bb.getSize();
}

QVector3D Mesh::getPosition(){
    return _position;
}

QVector3D Mesh::getRotation(){
    return _rotation;
}

QVector3D Mesh::getScale(){
    return _scale;
}

QString Mesh::getName(){
    return _name;
}

int Mesh::getId(){
    return _id;
}

size_t Mesh::size(){
    return _facets.size();
}

//---------------------- Set Methods -------------------------
//These methods shouldn't modify mesh
void Mesh::setPosition(QVector3D value){
    _position = value;
}

void Mesh::setRotation(QVector3D value){
    _rotation = value;
}

void Mesh::setScale(QVector3D value){
    _scale = value;
}

void Mesh::setName(QString n){
    _name = n;
}

void Mesh::setId(int i){
    _id = i;
}



//-----------------------------------------------------------------
//----------------------- Extern function -------------------------
//-----------------------------------------------------------------



// Stream operator (for debug)
std::ostream& operator<<(std::ostream& out, const Vec3 p) {
  out << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
  return out;
}

std::ostream& operator<<(std::ostream& out, const Facet& t) {
  out << "---- TRIANGLE ----" << std::endl;
  out << t.normal << std::endl;
  out << t.v[0] << std::endl;
  out << t.v[1] << std::endl;
  out << t.v[2] << std::endl;
  return out;
}



Mesh prepareMesh(Mesh mesh){
    QMatrix4x4 trans;
    trans.translate(mesh.getPosition());

    trans.rotate(mesh.getRotation().x(), 1,0,0);
    trans.rotate(mesh.getRotation().y(), 0,1,0);
    trans.rotate(mesh.getRotation().z(), 0,0,1);
    trans.scale(mesh.getScale());

    mesh.transform(trans);
    return mesh;
}

// Take an input Triangle Mesh ‘mesh’ and fill the output
// parameter ‘slicesWithLineSegments’ with line segments for
// each slice
void triMeshSlicer(
    QVector<Mesh> multiMesh, // the const input mesh
    QVector<Lines2D> &slicesWithLineSegments, const float sliceSize, QProgressBar *p)
{                                                           // slice size in 3D Model digital units
    Plane plane;                                            // The intersection plane
    QVector<Facets> facets;

    float zMax = 0;
    float z0 = 0;

    foreach (Mesh mesh, multiMesh) {
        const float tzMax = mesh.getBoundingBox().getUpperRight().z - mesh.getBoundingBox().getBottomLeft().z ;  // as the model for it’s 3D axis-aligned bounding-box
        if(tzMax > zMax) zMax = tzMax;

        const float tz0 = mesh.getBoundingBox().getBottomLeft().z;   // find the minimal z coordinate of the model (z0)
        if(tz0 < z0) z0 = tz0;
        mesh = prepareMesh(mesh);
        const Facets &m = mesh.getMesh();                       // get a const handle to the input mesh
        facets.push_back(m);
    }

    const size_t nSlices = 1 + (int)( zMax / sliceSize);    // compute number of output slices
    plane.setNormal(Vec3(0, 0, 1));                         // normal does not change during slicing    


    for (size_t i = 0; i < nSlices; ++i)
    {
        p->setValue(double(double(i) / double(nSlices)) * 100); // start generating slices
        Lines2D linesegs;                                    // the linesegs vector for each slice
        plane.setDistance(z0 + (double)i * sliceSize);        // position the plane according to slice index
        foreach (Facets m, facets) {
            for (size_t t = 0; t < m.size(); ++t)
            {                                                    // iterate all mesh triangles
                const Facet &triangle = m[t];                    // get a const handle to a triangle
                Line2D ls;
                int result = triangle.intersectPlane(plane, ls);
                if (0 == result)
                {                                                // the plane does intersect the triangle
                    linesegs.push_back(ls);                      // push a new Line Segment object to this slice
                }
            }
        }
        slicesWithLineSegments.push_back(linesegs);          // push this vector to the slices vector
    }
    return;
}

