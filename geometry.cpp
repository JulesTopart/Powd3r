#include "geometry.h"

bool operator==(Point const& a, Point const& b)
{
    if(a.ctx == b.ctx){
        if (a.ctx == c2D && a.x == b.x && a.y == b.y) return true;
        if (a.ctx == c3D && a.x == b.x && a.y == b.y && a.z == b.z) return true;
        return false;
    }else{
        if (a.ctx == c3D && a.x == b.x && a.y == b.y && a.z == 0) return true;
        if (a.ctx == c2D && a.x == b.x && a.y == b.y && b.z == 0) return true;
        return false;
    }
}

bool operator==(Line const& a, Line const& b)
{
    if ((a.a == b.a && a.b == b.b) || (a.b == b.a && a.a == b.b)) return true;
    return false;
}

Polygon::Polygon(Points points_) : points(points_){
    Points buf;
    Lines lBuf;
    for(Points::Iterator point = points_.begin(); points_.end(); point++){
        buf.push_back(*point);
        if(buf.size() == 2){
            lBuf.push_back(Line(buf[0], buf[1]));
            buf.pop_front();
        }
    }
    if(buf.size() != 0){
        lBuf.push_back(Line(buf[0], points_[0])); //Close the polygon
    }
    this->lines = lBuf;
}

Polygon::Polygon(Lines lines_) : lines(lines_){
    Points pBuf;
    Point lastBuf = lines_[0].a;
    for(Lines::Iterator line = lines_.begin(); lines_.end(); line++){
        if(lastBuf == line->a)
        pBuf.push_back(line->a);
        lastBuf = line->b;
    }
    this->points = pBuf;
}


