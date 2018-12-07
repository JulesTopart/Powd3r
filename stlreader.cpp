#include "stlreader.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>

#include "stlreader.h"

namespace stl {

  std::ostream& operator<<(std::ostream& out, const point p) {
    out << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const triangle& t) {
    out << "---- TRIANGLE ----" << std::endl;
    out << t.normal << std::endl;
    out << t.v1 << std::endl;
    out << t.v2 << std::endl;
    out << t.v3 << std::endl;
    return out;
  }

  float parse_float(std::ifstream& s) {
    char f_buf[sizeof(float)];
    s.read(f_buf, 4);
    float* fptr = (float*) f_buf;
    return *fptr;
  }

  point parse_point(std::ifstream& s) {
    float x = parse_float(s);
    float y = parse_float(s);
    float z = parse_float(s);
    return point(x, y, z);
  }

  stl_data parse_stl(const std::string& stl_path) {
    std::ifstream stl_file(stl_path.c_str(), std::ios::in | std::ios::binary);
    if (!stl_file) {
      assert(false);
    }

    char header_info[80] = "";
    char n_triangles[4];
    stl_file.read(header_info, 80);
    stl_file.read(n_triangles, 4);
    std::string h(header_info);
    stl_data info(h);
    unsigned int* r = (unsigned int*) n_triangles;
    unsigned int num_triangles = *r;
    for (unsigned int i = 0; i < num_triangles; i++) {
      auto normal = parse_point(stl_file);
      auto v1 = parse_point(stl_file);
      auto v2 = parse_point(stl_file);
      auto v3 = parse_point(stl_file);
      info.triangles.push_back(triangle(normal, v1, v2, v3));
      char dummy[2];
      stl_file.read(dummy, 2);
    }
    return info;
  }


  STL_STATUS checkStlFileFormat(const QString &path)
  {
      QFile file(path);
      if (!file.open(QIODevice::ReadOnly))
      {
          qDebug("\n\tUnable to open \"%s\"", qPrintable(path));
          return STL_INVALID;
      }

      QFileInfo fileInfo(path);
      size_t fileSize = fileInfo.size();

      // Look for text "solid " in first 6 bytes, indicating the possibility that this is an ASCII STL format.

      if (fileSize < 15)
      {
          // "solid " and "endsolid " markers for an ASCII file
          qDebug("\n\tThe STL file is not long enough (%u bytes).", uint(fileSize));
          return STL_INVALID;
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
          if (nextLine.startsWith("facet "))
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
              if (!line.startsWith("endLine"))
                  return STL_INVALID;

              return STL_ASCII;
          }
          if (nextLine.startsWith(endLine))
              return STL_ASCII;

          // reset and check for binary...
          if (!file.reset())
          {
              qDebug("\n\tCannot seek to the 0th byte (before the header)");
              return STL_INVALID;
          }
      }

      if (fileSize < 84)
      {
          // 80-byte header + 4-byte "number of triangles" for a binary file
          qDebug("\n\tThe STL file is not long enough (%u bytes).", uint(fileSize));
          return STL_INVALID;
      }

      // Header is from bytes 0-79; numTriangleBytes starts at byte offset 80.
      if (!file.seek(80))
      {
          qDebug("\n\tCannot seek to the 80th byte (after the header)");
          return STL_INVALID;
      }

      // Read the number of triangles, uint32_t (4 bytes), little-endian
      QByteArray nTrianglesBytes = file.read(4);
      if (nTrianglesBytes.size() != 4)
      {
          qDebug("\n\tCannot read the number of triangles (after the header)");
          return STL_INVALID;
      }

      uint32_t nTriangles = *((uint32_t*)nTrianglesBytes.data());

      // Verify that file size equals the sum of header + nTriangles value + all triangles
      if (fileSize == (84 + (nTriangles * 50)))
          return STL_BINARY;

      return STL_INVALID;
  }
}
