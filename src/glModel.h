#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>
#include "GL/glu.h"
#include "mesh.h"

class GLModel : public QGLWidget
{
    Q_OBJECT
public:
    GLModel(QWidget *parent = nullptr) ;
    ~GLModel();

    void loadModel(Mesh *mdl);
    void unloadModel(int id);
    int modelCount() {return models.size();}

    void drawAxis();
    void drawGrid(QVector3D size);

    Mesh* get(int i){return &models[i];}
    QVector<Mesh> getModels(){return models;}
    void rotate(int i, QVector3D r){ models[i].rotate(r);}
    void select(int i){ selectedModel = i;}
    int getSelected(){ return this->selectedModel;}

    void toggleBoundaries(boolean state){ showBoundaries = state;}

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

    boolean showBoundaries = false;
};

#endif // GLWIDGET_H
