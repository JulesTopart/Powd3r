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
     void timerEvent(QTimerEvent *);

     void keyPressEvent( QKeyEvent* );
     void scrollEvent( QWheelEvent *wheelEvent);


public slots:
     void timeOutSlot();

private:
    QTimer *t_Timer;

    QVector<mesh::Model> models;

    QVector3D rotationAxis;

    float angularSpeed;
    QVector2D mousePressPosition;

    GLfloat xRot=0.0;                     // X Rotation
    GLfloat yRot=0.0;                     // Y Rotation
    GLfloat zRot=0.0;                     // Z Rotation

    QQuaternion position;
    QQuaternion rotation;

    GLfloat xSpeed=0.0;                   // X Rotation Speed
    GLfloat ySpeed=0.0;                   // Y Rotation Speed
    GLfloat zSpeed=0.0;

    GLfloat x = 0.0f;                     // Width Into The Screen
    GLfloat y = 0.0f;                     // Height Into The Screen
    GLfloat z = -50.0f;                  // Depth Into The Screen

    BOOL    light=true;                   // Lighting ON / OFF

    GLfloat LightAmbient[4]= { 1.0f, 1.0f, 1.0f, 1.0f };

    GLfloat LightDiffuse[4]= { 0.6f, 0.6f, 0.55f, 1.0f };    // Diffuse Light Values ( NEW )
    // Ambient Light Values ( NEW )
    GLfloat LightPosition[4]= { 0.8f, 0.3f, 2.0f, 0.1f };

};

#endif // GLWIDGET_H
