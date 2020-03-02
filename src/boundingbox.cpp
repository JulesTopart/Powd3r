#include "mesh.h"



//*******************************
//      Bounding Box
//*******************************


BoundingBox::BoundingBox(){
    _bottomLeft = Vec3(0,0,0);
    _upperRight = Vec3(0,0,0);
    _center     = Vec3(0,0,0);
}


BoundingBox::BoundingBox(Mesh *mdlPtr){
    calculate(mdlPtr);
}

void BoundingBox::calculate(Mesh *mdlPtr){

    Mesh newMesh = prepareMesh(*mdlPtr);

    _center = mdlPtr->getPosition();
    _bottomLeft = Vec3( 999999, 999999, 999999);
    _upperRight = Vec3(-999999,-999999,-999999);

    Facets facets = newMesh.getMesh();

    for(int k(0); k < facets.size(); k++){
        Facet t = facets[k];
        for (size_t i = 0; i < 3; ++i)
        {
            if (t.v[i].x < _bottomLeft.x)
                _bottomLeft.x = t.v[i].x;
            if (t.v[i].y < _bottomLeft.y)
                _bottomLeft.y = t.v[i].y;
            if (t.v[i].z < _bottomLeft.z)
                _bottomLeft.z = t.v[i].z;
            if (t.v[i].x > _upperRight.x)
                _upperRight.x = t.v[i].x;
            if (t.v[i].y > _upperRight.y)
                _upperRight.y = t.v[i].y;
            if (t.v[i].z > _upperRight.z)
                _upperRight.z = t.v[i].z;
        }
    }
}

BoundingBox::BoundingBox(Vec3 ur, Vec3 bl, Vec3 center){
    _upperRight = ur;
    _bottomLeft = bl;
    _center = center;
}

Vec3 BoundingBox::getSize(){
    return Vec3(_upperRight.x - _bottomLeft.x,
                _upperRight.y - _bottomLeft.y,
                _upperRight.z - _bottomLeft.z);
}

void BoundingBox::draw(){
    glTranslatef(_center.x, _center.y, _center.z);

    glRotatef(90, 1, 0, 0); //mê !

    float HALF_GRID_X = getSize().y/2;
    float HALF_GRID_Y = getSize().x/2;
    float HALF_GRID_Z = getSize().z/2;

    glBegin(GL_LINES);

    glColor3f(0.5f, 0.8f, 0.5f);

    //Square
    glVertex3f(-HALF_GRID_Y,-HALF_GRID_Z,-HALF_GRID_X);
    glVertex3f(HALF_GRID_Y,-HALF_GRID_Z,-HALF_GRID_X);

    glVertex3f(-HALF_GRID_Y,-HALF_GRID_Z,HALF_GRID_X);
    glVertex3f(HALF_GRID_Y,-HALF_GRID_Z,HALF_GRID_X);

    glVertex3f(HALF_GRID_Y,-HALF_GRID_Z,HALF_GRID_X);
    glVertex3f(HALF_GRID_Y,-HALF_GRID_Z,-HALF_GRID_X);

    glVertex3f(-HALF_GRID_Y,-HALF_GRID_Z,HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,-HALF_GRID_Z,-HALF_GRID_X);

    //Pilar
    glVertex3f(HALF_GRID_Y,-HALF_GRID_Z,HALF_GRID_X);
    glVertex3f(HALF_GRID_Y,HALF_GRID_Z,HALF_GRID_X);

    glVertex3f(-HALF_GRID_Y,-HALF_GRID_Z,HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,HALF_GRID_Z,HALF_GRID_X);

    glVertex3f(HALF_GRID_Y,-HALF_GRID_Z,-HALF_GRID_X);
    glVertex3f(HALF_GRID_Y,HALF_GRID_Z,-HALF_GRID_X);

    glVertex3f(-HALF_GRID_Y,-HALF_GRID_Z,-HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,HALF_GRID_Z,-HALF_GRID_X);

    //Square :
    glVertex3f(HALF_GRID_Y,HALF_GRID_Z,HALF_GRID_X);
    glVertex3f(HALF_GRID_Y,HALF_GRID_Z,-HALF_GRID_X);

    glVertex3f(-HALF_GRID_Y,HALF_GRID_Z,HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,HALF_GRID_Z,-HALF_GRID_X);

    glVertex3f(HALF_GRID_Y,HALF_GRID_Z,HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,HALF_GRID_Z,HALF_GRID_X);

    glVertex3f(HALF_GRID_Y,HALF_GRID_Z,-HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,HALF_GRID_Z,-HALF_GRID_X);


    glEnd();

}

Vec3 BoundingBox::getHalfBox(){
    Vec3 halfBbox = (_upperRight - _bottomLeft) / 2.0f;
    return halfBbox;
}


void BoundingBox::setBottomLeft  (Vec3 bl){
    _bottomLeft = bl;
}

void BoundingBox::setUpperRight  (Vec3 ur){
    _upperRight = ur;
}

Vec3 BoundingBox::getBottomLeft  (){
    return _bottomLeft;
}

Vec3 BoundingBox::getUpperRight  (){
    return _upperRight;
}

