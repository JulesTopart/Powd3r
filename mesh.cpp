#include "mesh.h"

//-----------------------------------------------------------------
//-------------------------- Mesh class ---------------------------
//-----------------------------------------------------------------

//------------------------- Constructor ---------------------------

    Mesh::Mesh(): _bottomLeftVertex(999999, 999999, 999999), _upperRightVertex(-999999, -999999, -999999) {}
    Mesh::Mesh(const Mesh &copy) :  _name               (copy._name                 ),
                                    _id                 (copy._id                   ),
                                    _facets             (copy._facets               ),
                                    _bottomLeftVertex   (copy._bottomLeftVertex     ),
                                    _upperRightVertex   (copy._upperRightVertex     ),
                                    _position           (copy._position             ),
                                    _rotation           (copy._rotation             ),
                                    _scale              (copy._scale                ){}

//--------------------------- Methods -----------------------------


void Mesh::draw(){
    if(_facets.size() > 0){
        GLdouble x,y,z;
        for(Facets::Iterator facet = _facets.begin(); facet != _facets.end(); facet++){
            glBegin(GL_TRIANGLES);
                x = facet->normal.x;
                y = facet->normal.y;
                z = facet->normal.z;
                  glNormal3d(x, y, z);

              for(int i(0); i < 3; i++){
                  x = facet->v[i].x;
                  y = facet->v[i].y;
                  z = facet->v[i].z;
                    glVertex3d(x, y, z);
              }


            glEnd();
        }
    }
}

void Mesh::push_back(Facet t){
    _facets.push_back(t);
    for (size_t i = 0; i < 3; ++i)
    {
        if (t.v[i].x < _bottomLeftVertex.x)
            _bottomLeftVertex.x = t.v[i].x;
        if (t.v[i].y < _bottomLeftVertex.y)
            _bottomLeftVertex.y = t.v[i].y;
        if (t.v[i].z < _bottomLeftVertex.z)
            _bottomLeftVertex.z = t.v[i].z;
        if (t.v[i].x > _upperRightVertex.x)
            _upperRightVertex.x = t.v[i].x;
        if (t.v[i].y > _upperRightVertex.y)
            _upperRightVertex.y = t.v[i].y;
        if (t.v[i].z > _upperRightVertex.z)
            _upperRightVertex.z = t.v[i].z;
    }
}

//----------------- Transformation Methods --------------------


void Mesh::rotate(float x, float y, float z){
    _rotation += Vec3(x,y,z);
}

void Mesh::rotate(Vec3 v){
    _rotation += v;
}

void Mesh::scale(float x, float y, float z){
    _scale *= Vec3(x,y,z);
}

void Mesh::scale(Vec3 v){
    _scale *= v;
}

void Mesh::move(float x, float y, float z){
    _position += Vec3(x,y,z);
}

void Mesh::move(Vec3 v){
    _position += v;
}

void Mesh::normalize(){
    Vec3 halfBbox = (_upperRightVertex - _bottomLeftVertex) / 2.0f;
    Vec3 start = _bottomLeftVertex + halfBbox;
    for (size_t i = 0; i < size(); ++i)
    {
        _facets[i] -= start;
    }
    _bottomLeftVertex = halfBbox * -1.0f;
    _upperRightVertex = halfBbox;
}

void Mesh::transform (QMatrix4x4 mat){
    for (size_t i = 0; i < size(); ++i)
    {
        Facet &triangle = _facets[i];
        triangle.transform(mat);
    }
}

//---------------------- Get Methods -------------------------
Vec3 Mesh::getBBSize(){
    return Vec3(_upperRightVertex.x - _bottomLeftVertex.x,
                _upperRightVertex.y - _bottomLeftVertex.y,
                _upperRightVertex.z - _bottomLeftVertex.z) * _scale;
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

//Useful parsing stuff
double parseDouble(std::ifstream& s) {
  char f_buf[sizeof(double)];
  s.read(f_buf, 4);
  double* fptr = (double*) f_buf;
  return *fptr;
}

Vec3 parsePoint(std::ifstream& s) {
  double x = parseDouble(s);
  double y = parseDouble(s);
  double z = parseDouble(s);
  return Vec3(x, y, z);
}

Vec3 parseAsciiPoint(QString cBuf){
    double coord[3] = {0.0,0.0,0.0};
    for(int i(0); i < 3; i++){
        int index(0);
        QString tmp = cBuf;
        QByteArray array = tmp.toLatin1();
        coord[i] = std::atof(array);
        while(isspace(cBuf[index].toLatin1())){
            index ++;
        }
        while(!isspace(cBuf[index].toLatin1())){
            index ++;
        }
        index ++;
        cBuf.remove(0,index);
    }
    return Vec3(coord[0], coord[1], coord[2]);
}


//Read Ascii .stl file and return its mesh
Mesh parseAscii(const QString& stl_path, QProgressBar &pBar){
    QFile file(stl_path);
    if (!file.open(QIODevice::ReadOnly)){
      assert(false);
    }
    file.read(6);
    pBar.setValue(float(float(file.pos()) / float(file.size())) * 100);
    QString name = file.readLine();
    name.chop(1);
    Mesh output;
    output.setName(name);
    QString cBuf;
    Facet tBuff;
    cBuf = file.readLine();
    pBar.setValue(float(float(file.pos()) / float(file.size())) * 100);
    do{
        int index = 0;
        while(cBuf[index] != 'l'){
            index ++;
        }
        index++;
        cBuf.remove(0,index); //Skip "facet normal"
        Vec3 normal = parseAsciiPoint(cBuf);
        tBuff.normal =  normal;

        file.readLine(); //Skip "outer loop"
        for(int i(0); i < 3; i++){
            cBuf = file.readLine();
            index = 0;
            while(cBuf[index] != 'x'){
                index ++;
            }
            index++;
            cBuf.remove(0,index);
            Vec3 v = parseAsciiPoint(cBuf);
            if(i == 0) tBuff.v[0]=  v;
            if(i == 1) tBuff.v[1] =  v;
            if(i == 2){
                tBuff.v[2] =  v;
                output.push_back(tBuff);
            }
            pBar.setValue(float(float(file.pos()) / float(file.size())) * 100);
        }
        file.readLine();//Skip "endloop"
        pBar.setValue(float(float(file.pos()) / float(file.size())) * 100);
        file.readLine();//Skip "endFacet"
        pBar.setValue(float(float(file.pos()) / float(file.size())) * 100);
        cBuf = file.readLine();
        pBar.setValue(float(float(file.pos()) / float(file.size())) * 100);
        if(QString(cBuf).startsWith("endsolid")) return output;

    }while(!cBuf.isNull());

    output.normalize();
    return output;
}

Mesh parseBinary(const std::string& stl_path, QProgressBar &pBar){
    std::ifstream stl_file(stl_path.c_str(), std::ios::in | std::ios::binary);
    if (!stl_file) {
      assert(false);
    }

    char header_info[80] = "";
    char n_triangles[4];
    stl_file.read(header_info, 80);
    stl_file.read(n_triangles, 4);
    QString h(header_info);
    Mesh model;
    unsigned int* r = (unsigned int*) n_triangles;
    unsigned int num_triangles = *r;
    for (unsigned int i = 0; i < num_triangles; i++) {
      auto normal = parsePoint(stl_file);
      auto v1 = parsePoint(stl_file);
      auto v2 = parsePoint(stl_file);
      auto v3 = parsePoint(stl_file);
      model.push_back(Facet(normal, v1, v2, v3));
      char dummy[2];
      stl_file.read(dummy, 2);
      pBar.setValue(pBar.value() + 4);
    }
    model.normalize();
    return model;
}


FILE_FORMAT getFileFormat(const QString &path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug("\n\tUnable to open \"%s\"", qPrintable(path));
            printf("\n\tUnable to open \"%s\"");
        return INVALID;
    }

    QFileInfo fileInfo(path);
    size_t fileSize = fileInfo.size();

    // Look for text "solid " in first 6 bytes, indicating the possibility that this is an ASCII STL format.

    if (fileSize < 15)
    {
        // "solid " and "endsolid " markers for an ASCII file
        qDebug("\n\tThe STL file is not long enough (%u bytes).", uint(fileSize));
        printf("\n\tThe STL file is not long enough (%u bytes).");
        return INVALID;
    }

    // binary files should never start with "solid ", but
    // just in case, check for ASCII, and if not valid then
    // check for binary...

    QByteArray sixBytes = file.read(6);
    if (sixBytes.startsWith("solid "))
    {
        QByteArray name = file.readLine();
        QByteArray endLine = name.prepend("endsolid ");

        QByteArray nextLine = file.readLine();
        if (nextLine.startsWith("  facet "))
        {
            QFile temp(path);
            if (!temp.open(QIODevice::ReadOnly))
            {
                qDebug("\n\tUnable to open temp file for ascii check", qPrintable(path));
            }

            QString line = temp.readLine();
            QString lastLine;
            while (!line.isNull()) {
                lastLine = line;
                line = temp.readLine();
            }

            line = lastLine;
            if (!line.startsWith("endsolid")){
                return INVALID;
            }
            return ASCII;
        }
        if (nextLine.startsWith(endLine))
            return ASCII;

        // reset and check for binary...
        if (!file.reset())
        {
            qDebug("\n\tCannot seek to the 0th byte (before the header)");
             printf("\n\tCannot seek to the 0th byte (before the header)");
            return INVALID;
        }
    }

    if (fileSize < 84)
    {
        // 80-byte header + 4-byte "number of triangles" for a binary file
        qDebug("\n\tThe STL file is not long enough (%u bytes).", uint(fileSize));
        printf("\n\tThe STL file is not long enough (%u bytes).");
        return INVALID;
    }

    // Header is from bytes 0-79; numTriangleBytes starts at byte offset 80.
    if (!file.seek(80))
    {
        qDebug("\n\tCannot seek to the 80th byte (after the header)");
         printf("\n\tCannot seek to the 80th byte (after the header)");
        return INVALID;
    }

    // Read the number of triangles, uint32_t (4 bytes), little-endian
    QByteArray nTrianglesBytes = file.read(4);
    if (nTrianglesBytes.size() != 4)
    {
        qDebug("\n\tCannot read the number of triangles (after the header)");
         printf("\n\tCannot read the number of triangles (after the header)");
        return INVALID;
    }

    uint32_t nTriangles = *((uint32_t*)nTrianglesBytes.data());

    // Verify that file size equals the sum of header + nTriangles value + all triangles
    if (fileSize == (84 + (nTriangles * 50)))
        return BINARY;

    qDebug("\n\tLast", qPrintable(path));
    printf("\n\tLast");
    return INVALID;
}

// Take an input Triangle Mesh ‘mesh’ and fill the output
// parameter ‘slicesWithLineSegments’ with line segments for
// each slice
void triMeshSlicer(
    const Mesh *meshPtr, // the const input mesh
    std::vector<std::vector<LineSegment>> &slicesWithLineSegments, const float sliceSize)
{                                                         // slice size in 3D Model digital units
    Plane plane;                                          // The intersection plane
    Mesh mesh = *meshPtr;
    plane.setNormal(Vec3(0, 0, 1));                         // normal does not change during slicing
    const Vec3 aabb = mesh.getBBSize();                 // as the model for it’s 3D axis-aligned bounding-box
    const size_t nSlices = 1 + (int)(aabb.z / sliceSize); // compute number of output slices
    const Facets &m = mesh.getMesh();     // get a const handle to the input mesh
    const float z0 = mesh.getBottomLeftVertex().z;       // find the minimal z coordinate of the model (z0)
    for (size_t i = 0; i < nSlices; ++i)
    {                                                 // start generating slices
        std::vector<LineSegment> linesegs;            // the linesegs vector for each slice
        plane.setDistance(z0 + (float)i * sliceSize); // position the plane according to slice index
        for (size_t t = 0; t < m.size(); ++t)
        {                                    // iterate all mesh triangles
            const Facet &triangle = m[t]; // get a const handle to a triangle
            LineSegment ls;
            if (0 == triangle.intersectPlane(plane, ls))
            {                           // the plane does intersect the triangle
                linesegs.push_back(ls); // push a new Line Segment object to this slice
            }
        }
        slicesWithLineSegments.push_back(linesegs); // push this vector to the slices vector
    }
    return;
}
