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
    void loadModel(mesh::Model mdl);
    void drawAxis();
    void drawGrid(int grid_size);
    void TopView();
    void BottomView();
    void FrontView();
    void BackView();
    void LeftView();
    void RightView();
    void AxonView();


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

    QVector<mesh::Model> models;

    QVector3D mouseRotation;

    QVector2D mousePressPosition;
    bool rightMousePressed = false;

    QVector3D rotation = QVector3D(0,0,0);
    QVector3D rotationSpeed = QVector3D(0,0,0);
    QVector3D position = QVector3D(0,0,-50);
    QVector3D scale = QVector3D(0.5,0.5,0.5);

    BOOL    light=true;                   // Lighting ON / OFF

    GLfloat LightAmbient[4]= { 1.0f, 1.0f, 1.0f, 1.0f };

    GLfloat LightDiffuse[4]= { 0.6f, 0.6f, 0.55f, 1.0f };    // Diffuse Light Values ( NEW )
    // Ambient Light Values ( NEW )
    GLfloat LightPosition[4]= { 50.0f, 150.0f, 0.0f, 0.1f };

};

#endif // GLWIDGET_H
