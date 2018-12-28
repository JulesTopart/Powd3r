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
    void drawGrid(int grid_size);
    std::vector<std::vector<LineSegment>> *getLines(){ return &_lines;}
    void selectSlice(int n){
        activeSlice = n;
    }

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
     QTimer *t_Timer;
     QVector2D
     position      = QVector2D(0,0);
     QVector2D
     scale         = QVector2D(0.5,0.5);

     QVector2D
     mousePressPosition;
     bool
     leftMousePressed = false;

     size_t activeSlice = 0;
     std::vector<std::vector<LineSegment>> _lines;
};

#endif // GL2D_H

