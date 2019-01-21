#ifndef GLPATH_H
#define GLPATH_H

#include <QtOpenGL>
#include <QGLWidget>
#include "GL/glu.h"
#include "mesh.h"

class glPath : public QGLWidget
{
    Q_OBJECT
public:
public:

    glPath(QWidget *parent = nullptr) ;
    ~glPath();

    void drawAxis();
    void drawGrid(int grid_size);
    QVector<Lines2D> *getLines(){ return &_subLines;}
    void selectSlice(int n){
        activeSlice = n;
    }
    void push(Lines2D subLines_){_subLines.push_back(subLines_);}
    void clear(){_subLines.clear();}
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
     QVector<Lines2D> _subLines;

};
#endif // GLPATH_H
