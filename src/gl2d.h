#ifndef GL2D_H
#define GL2D_H

#include <QtOpenGL>
#include <QGLWidget>
#include "GL/glu.h"
#include "mesh.h"

class gl2D : public QGLWidget
{
    Q_OBJECT
public:
public:

    gl2D(QWidget *parent = nullptr) ;
    ~gl2D();

    void drawAxis();
    void drawGrid(QVector2D grid_size);

    int sliceCount(){
        return slices.size();
    }

    void selectSlice(int n){
        activeSlice = n;
    }

    Slices* getSlice(){return &slices;}

    QVector2D
    originOffset = QVector2D(0,0);

    QVector2D
    plateDim      = QVector2D(200,200);

protected:

     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);

     void mouseMoveEvent( QMouseEvent* );
     void mousePressEvent(QMouseEvent *e);
     void mouseReleaseEvent(QMouseEvent *e);
     void wheelEvent( QWheelEvent* );

public slots:
     void timeOutSlot();

private:

     //OpenGL
     QTimer *t_Timer;
     QVector2D
     position      = QVector2D(0,0);
     QVector2D
     scale         = QVector2D(0.5,0.5);

     //Input
     QVector2D
     mousePressPosition;
     bool
     leftMousePressed = false;

     //Slices
     int activeSlice = 0;
     Slices slices;
};

#endif // GL2D_H

