#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>
#include "GL/glu.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget();
    explicit GLWidget(QWidget *parent = nullptr);
    virtual ~GLWidget();

protected:
     virtual void initializeGL();
     virtual void paintGL();
     virtual void resizeGL(int width, int height);

     virtual void mousePressEvent( QMouseEvent*);
     virtual void mouseMoveEvent( QMouseEvent* );
     virtual void keyPressEvent( QKeyEvent* );

public slots:
     void timeOutSlot();

private:
    QTimer *t_Timer;
    float rx, ry;
};

#endif // GLWIDGET_H
