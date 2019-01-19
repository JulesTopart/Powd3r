#include "gl2d.h"


gl2D::gl2D(QWidget *parent){
    int seconde = 1000; // 1 seconde = 1000 ms
    int timerInterval = seconde / 60;
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    t_Timer->start( timerInterval );
}

gl2D::~gl2D(){

}

void gl2D::drawAxis(){
    glEnable(GL_LINE_SMOOTH);
    glPushMatrix();
    glBegin(GL_LINES);
      glColor3f(1,0,0);
      glVertex2i(0,0);glVertex2i(10,0);
      glColor3f(0,1,0);
      glVertex2i(0,0);glVertex2i(0,10);
    glEnd();
    glPopMatrix();
    glDisable(GL_LINE_SMOOTH);
}

void gl2D::drawGrid(int grid_size){
    int HALF_GRID_SIZE = grid_size/2;
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f);
    for(int i=-HALF_GRID_SIZE;i<=HALF_GRID_SIZE;i+=10)
    {
        glVertex2f((float)i,(float)-HALF_GRID_SIZE);
        glVertex2f((float)i,(float)HALF_GRID_SIZE);

        glVertex2f((float)-HALF_GRID_SIZE,(float)i);
        glVertex2f((float)HALF_GRID_SIZE,(float)i);
    }
    glEnd();
}

void gl2D::initializeGL(){
    glShadeModel(GL_SMOOTH);
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
}

void gl2D::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glTranslatef(position.x(), position.y(), -25);
    glScalef(scale.x(), scale.y(), 0);
    //Draw axis
    drawAxis();
    glColor3f(0.5,0.5,0.48f);

    //Draw grid
    //drawGrid(200);
    glColor3f(0.5,0.5,0.48f); //retablish default color

    if(activeSlice < slices.size() && slices.size() > 0){
        LineSegment2Ds lines;
        lines = slices[activeSlice].asLines();
        glBegin(GL_LINES);
        glColor3f(0.5f, 0.5f, 0.5f);
        for(int i(0); i < lines.size(); i++){
            glVertex3f(lines[i].A().x(), lines[i].A().y(), 0);
            glVertex3f(lines[i].B().x(), lines[i].B().y(), 0);
        }
        glEnd();
    }
}

void gl2D::resizeGL(int width, int height){
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void gl2D::timeOutSlot()
{
    updateGL();
}

void gl2D::mouseMoveEvent( QMouseEvent* event){
    if (leftMousePressed){
        // Mouse release position - mouse press position
        QVector2D diff = QVector2D(event->localPos()) - mousePressPosition;
        diff *= 0.05;
        // Rotation axis is perpendicular to the mouse position difference
        // vector
        QVector2D n = QVector2D(diff.x(), - diff.y());
        position += n;

        mousePressPosition = QVector2D(event->x(), event->y());
        event->accept();
        return;
    }
    event->ignore();
}

void gl2D::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        leftMousePressed = true;
        mousePressPosition = QVector2D(event->x(), event->y());
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void gl2D::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        leftMousePressed = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void gl2D::wheelEvent(QWheelEvent* event){
    float t = event->angleDelta().y() * 0.0005f;
    if((this->scale + QVector2D(t,t)).length() >= 0.1) this->scale += QVector2D(t,t);
    event->accept();
}
