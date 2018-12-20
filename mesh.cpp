#include "mesh.h"

namespace mesh{



std::ostream& operator<<(std::ostream& out, const Point p) {
  out << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
  return out;
}

std::ostream& operator<<(std::ostream& out, const Facet& t) {
  out << "---- TRIANGLE ----" << std::endl;
  out << t.normal << std::endl;
  out << t.v1 << std::endl;
  out << t.v2 << std::endl;
  out << t.v3 << std::endl;
  return out;
}

double parseDouble(std::ifstream& s) {
  char f_buf[sizeof(double)];
  s.read(f_buf, 4);
  double* fptr = (double*) f_buf;
  return *fptr;
}

Point parsePoint(std::ifstream& s) {
  double x = parseDouble(s);
  double y = parseDouble(s);
  double z = parseDouble(s);
  return Point(x, y, z);
}

Point parseAsciiPoint(QString cBuf){
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
    return Point(coord[0], coord[1], coord[2]);
}

Model parseAscii(const QString& stl_path, QProgressBar &pBar){
    QFile file(stl_path);
    if (!file.open(QIODevice::ReadOnly)){
      assert(false);
    }
    file.read(6);
    QString name = file.readLine();
    Model output;
    output._name = name;
    QString cBuf;
    Facet tBuff;
    cBuf = file.readLine();
    do{
        int index = 0;
        while(cBuf[index] != 'l'){
            index ++;
        }
        index++;
        cBuf.remove(0,index); //Skip "facet normal"
        Point normal = parseAsciiPoint(cBuf);
        tBuff.normal =  normal.asVector3D();

        file.readLine(); //Skip "outer loop"
        for(int i(0); i < 3; i++){
            cBuf = file.readLine();
            index = 0;
            while(cBuf[index] != 'x'){
                index ++;
            }
            index++;
            cBuf.remove(0,index);
            Point v = parseAsciiPoint(cBuf);
            if(i == 0) tBuff.v1 =  v.asVector3D();
            if(i == 1) tBuff.v2 =  v.asVector3D();
            if(i == 2){
                tBuff.v3 =  v.asVector3D();
                output.facets.push_back(tBuff);
            }
            pBar.setValue(pBar.value() + 1);
        }
        file.readLine();//Skip "endloop"
        file.readLine();//Skip "endFacet"
        cBuf = file.readLine();
        if(QString(cBuf).startsWith("endsolid")) return output;
    }while(!cBuf.isNull());

    return output;
}

Model parseBinary(const std::string& stl_path, QProgressBar &pBar){
    std::ifstream stl_file(stl_path.c_str(), std::ios::in | std::ios::binary);
    if (!stl_file) {
      assert(false);
    }

    char header_info[80] = "";
    char n_triangles[4];
    stl_file.read(header_info, 80);
    stl_file.read(n_triangles, 4);
    QString h(header_info);
    Model model;
    unsigned int* r = (unsigned int*) n_triangles;
    unsigned int num_triangles = *r;
    for (unsigned int i = 0; i < num_triangles; i++) {
      auto normal = parsePoint(stl_file);
      auto v1 = parsePoint(stl_file);
      auto v2 = parsePoint(stl_file);
      auto v3 = parsePoint(stl_file);
      model.facets.push_back(Facet(normal, v1, v2, v3));
      char dummy[2];
      stl_file.read(dummy, 2);
      pBar.setValue(pBar.value() + 4);
    }
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


void Model::draw(){
    if(facets.size() != 0){
        GLdouble x,y,z;
        for(Facets::Iterator facet = facets.begin(); facet != facets.end(); facet++){
            glBegin(GL_TRIANGLES);
                x = facet->normal.x;
                y = facet->normal.y;
                z = facet->normal.z;
                  glNormal3d(x, y, z);

                x = facet->v1.x;
                y = facet->v1.y;
                z = facet->v1.z;
                  glVertex3d(x, y, z);

                x = facet->v2.x;
                y = facet->v2.y;
                z = facet->v2.z;
                  glVertex3d(x, y, z);

                x = facet->v3.x;
                y = facet->v3.y;
                z = facet->v3.z;
                  glVertex3d(x, y, z);

            glEnd();
        }
    }
}

void Model::rotate(float x, float y, float z){
    _rotation += QVector3D(x,y,z);
}

void Model::rotate(QVector3D v){
    _rotation += v;
}

void Model::scale(float x, float y, float z){
    _scale = QVector3D(x,y,z);
}

void Model::scale(QVector3D v){
    _scale = v;
}

void Model::move(float x, float y, float z){
    _position += QVector3D(x,y,z);
}

void Model::move(QVector3D v){
    _position += v;
}

void Model::moveTo(float x, float y, float z){
    _position = QVector3D(x,y,z);
}

void Model::moveTo(QVector3D v){
    _position = v;
}

//Get Methods
QVector3D Model::pos(){
    return this->_position;
}

QVector3D Model::rot(){
    return this->_rotation;
}

QVector3D Model::scale(){
    return this->_scale;
}



}
