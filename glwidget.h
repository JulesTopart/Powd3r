#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>
#include "GL/glu.h"
#include "mesh.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = nullptr) ;
    ~GLWidget();

    void loadModel(Mesh *mdl);
    void unloadModel(int id);
    int modelCount() {return models.size();}

    void drawAxis();
    void drawGrid(QVector3D size);

    Mesh* get(int i){return &models[i];}
    void rotate(int i, QVector3D r){ models[i].rotate(r);}
    void select(int i){ selectedModel = i;}

    void TopView();
    void BottomView();
    void FrontView();
    void BackView();
    void LeftView();
    void RightView();
    void AxonView();

    void centerOnModel(int i);

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

    QVector<Mesh>
    models;

    int selectedModel = -1;

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
    position      = QVector3D(0,-5,-50);
    QVector3D
    scale         = QVector3D(0.5,0.5,0.5);


    BOOL
    light = true;

    GLfloat
    LightAmbient[4]  = { 0.5f, 0.5f, 0.5f, 1.0f };

    GLfloat
    LightDiffuse[4]  = { 0.01f, 0.01f, 0.01f, 1.0f };

    GLfloat
    LightPosition[4] = { 5.0f, 5.0f, 5.0f, 1.0f };

    GLfloat
    selectedEmission[4] = { 1.0, 1.0, 1.0, 1.0 };
};

#endif // GLWIDGET_H
