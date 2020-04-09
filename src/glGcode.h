#ifndef GLGCODE_H
#define GLGCODE_H

#include <QtOpenGL>
#include <QGLWidget>
#include "GL/glu.h"
#include "geometry.h"

QString codeToArray(QString code);
float lookForValue(char key, QString msg);

struct NState{
    NState() : x(0) {}
    NState(float _x, QString code) : x(_x) {
        code = codeToArray(code);
        if(code.length() == 12)
        for (int i = 0; i < code.length(); i++) {
            nozzles[i] = (code[i] == '1') ? true : false;
        }
    }
    NState(float _x, int _code) : x(_x){
        QString code = codeToArray(QString::number(_code));
        if(code.length() == 12)
        for (int i = 0; i < code.length(); i++) {
            nozzles[i] = (code[i] == '1') ? true : false;
        }
    }

    float x;
    bool direction;
    bool nozzles[12];
};
typedef QVector<NState> NStates;

struct NSweep{
    NSweep() : x(0), y(0){}
    NSweep(float _x, float _y, NStates _states) : x(_x), y(_y){
        states = _states;
    }

    void draw(float z);
    float x, y;
    NStates states;
};
typedef QVector<NSweep> NSweeps;

struct NLayer{
    NLayer() : z(0){}
    NLayer(float _z) : z(_z){}
    NLayer(float _z, NSweeps _sweeps) : z(_z){
        sweeps = _sweeps;
    }


    void draw();


    float z;

    NSweeps sweeps;
};
typedef QVector<NLayer> NLayers;

class glGcode : public QGLWidget
{
    Q_OBJECT
public:
    glGcode(QWidget *parent = nullptr) ;
    ~glGcode();

    void generatePath(QString code);
    void selectSlice(int n){if(n < _layers.size()) activeLayer = n;};
    void drawAxis();
    void drawGrid(QVector2D size);

    void TopView();
    void BottomView();
    void FrontView();
    void BackView();
    void LeftView();
    void RightView();
    void AxonView();

    QVector3D
    plateDim      = QVector3D(200,200,200);

    QVector3D
    originOffset = QVector3D(0,0,0);

protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);

     void mouseMoveEvent( QMouseEvent* );
     void mousePressEvent(QMouseEvent *e);
     void mouseReleaseEvent(QMouseEvent *e);
     void timerEvent(QTimerEvent*);

     void keyPressEvent( QKeyEvent* );
     void wheelEvent( QWheelEvent* );


public slots:
     void timeOutSlot();

private:
    QTimer *t_Timer;

    QVector3D
    mouseRotation;

    QVector2D
    mousePressPosition;
    bool
    leftMousePressed = false;
    bool
    rightMousePressed = false;

    QVector3D
    rotation      = QVector3D(0,0,0);
    QVector3D
    rotationSpeed = QVector3D(0,0,0);
    QVector3D
    position      = QVector3D(0,-10,-50);
    QVector3D
    scale         = QVector3D(0.5,0.5,0.5);


    BOOL
    light = true;

    GLfloat
    LightAmbient[4]  = { 0.7f, 0.7f, 0.7f, 1.0f };

    GLfloat
    LightDiffuse[4]  = { 0.4f, 0.4f, 0.4f, 1.0f };

    GLfloat
    LightPosition[4] = { 8.0f, 8.0f, 8.0f, 1.0f };

    GLfloat
    selectedEmission[4] = { 1.0, 1.0, 1.0, 1.0 };

    unsigned int activeLayer = 0;
    NLayers _layers;
};

#endif // GLGCODE_H
