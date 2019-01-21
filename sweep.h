#ifndef SWEEP_H
#define SWEEP_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>    // std::sort
#include "geometry.h"
//#define DEBUG_SWEEP

typedef QVector2D Point;
typedef QVector<QVector2D> Points;

float getMin(std::vector<Line2D> lines);
float getMinY(std::vector<Line2D> lines);
float getMax(std::vector<Line2D> lines);
float getMaxY(std::vector<Line2D> lines);

class NozzleLine;
bool operator>(NozzleLine const &a, NozzleLine const& b);
bool operator<(NozzleLine const &a, NozzleLine const& b);

class NozzleLine {
public:
    NozzleLine() {}
    NozzleLine(Line2D l) { line_coll.push_back(l); y = l.a.y(); }

    void push(Line2D l) {
        line_coll.push_back(l);
        y = l.a.y();
    }

    void push(std::vector<Line2D>::iterator l) {
        line_coll.push_back(*l);
        y = l->a.y();
    }

    int size() {
        return line_coll.size();
    }

    Line2D get(int i) {
        return line_coll[i];
    }

    void clear() {
        line_coll.clear();
        y = 0;
    }

    bool smallerThan(NozzleLine const& b) const;
    bool biggerThan(NozzleLine const& b) const;

    float getY() { return y; }
    void drawLine();
    void calcMinMax();

    static bool getGreater(NozzleLine a, NozzleLine b) { return a.getY() > b.getY(); };

private:
    std::vector<Line2D> line_coll;
    float xmin, xmax, y;
};



class NozzleAction {
public:
    NozzleAction() : x(0.0) {};
    NozzleAction(float _x) : x(_x), length(1.0f), code(0.0f), dpiconst(25.4f/96.0f) {};
    NozzleAction(float _x, float _length) : x(_x), length(_length), code(0.0f), dpiconst(25.4f / 96.0f) {};
    NozzleAction(float _x, float _length, int _code) : x(_x), length(_length), code(_code), dpiconst(25.4f / 96.0f) {};
    NozzleAction(float _x, float _length, int _code, int dpi) : x(_x), length(_length), code(_code), dpiconst(25.4f / (float)dpi) {};

    std::string toGcode(bool dir);

    void addToLength(float _x) {
        length += _x;
    }

    float dpiconst;
    float x, length;
    int code;
    std::vector<bool> nozzleArray;
};



class Sweep {
public:

    bool absoluteMode = true;

    Sweep() {};

    void push(NozzleLine n) {
        nozzleLines.push_back(n);
    }

    void clear() {
        nozzleLines.clear();
    }

    int size() {
        return nozzleLines.size();
    }

    int sizeA() {
        return nozzleActions.size();
    }

    void calcXMinMax();
    void calcYMinMax();

    void calcMinMax() {
        calcXMinMax();
        calcYMinMax();
        calcOrigin();
    }

    void calcOrigin();

    float getNextX(float boundX);

    std::vector<bool> getbyX(float X);

    int arrayToCode(std::vector<bool> nozzleArray);

    bool isNozzleOn(int nozzleN, float X);

    float getXMin() {
        return xmin;
    }

    float getXMax() {
        return xmax;
    }

    float getYMin() {
        return ymin;
    }

    float getYMax() {
        return ymax;
    }

    NozzleLine getL(int i) {
        return nozzleLines[i];
    }

    NozzleAction getA(int i) {
        return nozzleActions[i];
    }

    void addLengthOf(int i, float _x) {
        nozzleActions[i].addToLength(_x);
    }

    void drawLines();

    void addNozzleAction(NozzleAction n) {
        nozzleActions.push_back(n);
    }

    std::string toGcode(bool dir);
    std::string goToLeftPoint();
    std::string goToRightPoint();

    std::string relative() {
        if (absoluteMode) {
            absoluteMode = false;
            return "G91\n";
        }
        return "";
    }
    std::string absolute() {
        if (!absoluteMode) {
            absoluteMode = true;
            return "G90\n";
        }
        return "";
    }

    void setNozzle(short fN, short lN) {
        firstNozzle = fN;
        lastNozzle = lN;
    }

private:
    Point origin, end;
    float xmin, xmax, ymin, ymax;
    short firstNozzle, lastNozzle;
    std::vector<NozzleLine> nozzleLines; //12 nozzle lines
    std::vector<NozzleAction> nozzleActions;
};



class SweepCollection {
public:
    SweepCollection();
    bool lastDir = true;

    void push(Sweep s){
        sweeps.push_back(s);
    }

    Sweep get(int i) {
        return sweeps[i];
    }

    int size() {
        return sweeps.size();
    }

    std::vector<Sweep>::iterator begin() {
        return sweeps.begin();
    }

    std::vector<Sweep>::iterator end() {
        return sweeps.end();
    }


    std::string toGcode(short nPass = 1);

    static SweepCollection generateSweeps(std::vector<Line2D> lines, short firstNozzle,short lastNozzle, short dpi);

private:

    std::vector<Sweep>::iterator it;     //Un itérateur sur un vector d'entiers
    std::vector<Sweep> sweeps;
};




#endif // SWEEP_H
