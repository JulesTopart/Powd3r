#include "mesh.h"

//Useful parsing stuff
float parseDouble(std::ifstream& s) {
  char f_buf[sizeof(float)];
  s.read(f_buf, 4);
  float* fptr = (float*) f_buf;
  return *fptr;
}

Vec3 parsePoint(std::ifstream& s) {
  float x = parseDouble(s);
  float y = parseDouble(s);
  float z = parseDouble(s);
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
    return Vec3(float(coord[0]), float(coord[1]), float(coord[2]));
}


//Read Ascii .stl file and return its mesh
Mesh parseAscii(const QString& stl_path, QProgressBar &pBar){
    QFile file(stl_path);
    if (!file.open(QIODevice::ReadOnly)){
      assert(false);
    }
    file.read(6);
    pBar.setValue(int(float(float(file.pos()) / float(file.size())) * 100));
    QString name = file.readLine();
    name.chop(1);
    Mesh output;
    output.setName(name);
    QString cBuf;
    Facet tBuff;
    cBuf = file.readLine();
    pBar.setValue(int(float(float(file.pos()) / float(file.size())) * 100));
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
            pBar.setValue(int(float(float(file.pos()) / float(file.size())) * 100));
        }
        file.readLine();//Skip "endloop"
        pBar.setValue(int(float(float(file.pos()) / float(file.size())) * 100));
        file.readLine();//Skip "endFacet"
        pBar.setValue(int(float(float(file.pos()) / float(file.size())) * 100));
        cBuf = file.readLine();
        pBar.setValue(int(float(float(file.pos()) / float(file.size())) * 100));
        if(QString(cBuf).startsWith("endsolid")) return output;

    }while(!cBuf.isNull());

    //Put on plate
    output.applyChange();
    output.move(0, 0, output.getBBSize().z/2);
    output.applyChange();

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
    pBar.setMaximum(int(num_triangles));
    for (unsigned int i = 0; i < num_triangles; i++) {
      auto normal = parsePoint(stl_file);
      auto v1 = parsePoint(stl_file);
      auto v2 = parsePoint(stl_file);
      auto v3 = parsePoint(stl_file);
      model.push_back(Facet(normal, v1, v2, v3));
      char dummy[2];
      stl_file.read(dummy, 2);
      pBar.setValue(int(i));
    }
    pBar.setMaximum(100);
    pBar.setValue(100);
    model.normalize();

    //Put on plate
    model.applyChange();
    model.move(0, 0, model.getBBSize().z/2);
    model.applyChange();

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
    qint64 fileSize = fileInfo.size();

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
