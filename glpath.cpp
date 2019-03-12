#include "glpath.h"


glPath::glPath(QWidget *parent){
    int seconde = 1000; // 1 seconde = 1000 ms
    int timerInterval = seconde / 60;
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    t_Timer->start( timerInterval );
}

glPath::~glPath(){

}

void glPath::drawAxis(){
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

void glPath::drawGrid(QVector2D size)
{
    float HALF_GRID_X = size.y()/2;
    float HALF_GRID_Y = size.x()/2;

    glBegin(GL_LINES);
    for(int i =- HALF_GRID_Y; i <= HALF_GRID_Y; i += 40 )
    {
        glVertex2f((float)i,(float)-HALF_GRID_X);
        glVertex2f((float)i,(float)HALF_GRID_X);
    }

    for(int i=-HALF_GRID_X;i<=HALF_GRID_X;i += 40){
        glVertex2f((float)-HALF_GRID_Y,(float)i);
        glVertex2f((float)HALF_GRID_Y,(float)i);
    }

    glEnd();
}


void glPath::initializeGL(){
    glShadeModel(GL_SMOOTH);
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
}

void glPath::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glTranslatef(position.x(), position.y(), -25);
    glScalef(scale.x(), scale.y(), 0);
    //Draw axis

    glPushMatrix();
    glTranslatef(originOffset.x() - plateDim.x()/2, originOffset.y() - plateDim.y()/2, 0);
    drawAxis();
    glPopMatrix();

    glColor3f(0.3f,0.3f,0.28f);
    drawGrid(plateDim);


    //Draw grid
    //drawGrid(200);
    glColor3f(0.5,0.5,0.48f); //retablish default color


    if(activeSlice < _subLines.size() && _subLines.size() > 0){
        glBegin(GL_LINES);

        for(size_t i(0); i < _subLines[activeSlice].size(); i++){
            if((i / nozzleCount) % 2 == 0) glColor3f(0.7f, 1, 0.7f);
            else glColor3f(1, 0.7f, 0.7f);
            glVertex3f(_subLines[activeSlice][i].A().x(), _subLines[activeSlice][i].A().y(), 0);
            glVertex3f(_subLines[activeSlice][i].B().x(), _subLines[activeSlice][i].B().y(), 0);
        }
        glEnd();
    }

}

void glPath::resizeGL(int width, int height){
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glPath::timeOutSlot()
{
    updateGL();
}

void glPath::mouseMoveEvent( QMouseEvent* event){
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

void glPath::mousePressEvent(QMouseEvent *event){
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

void glPath::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        leftMousePressed = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void glPath::wheelEvent(QWheelEvent* event){
    float t = event->angleDelta().y() * 0.0005f;
    if((this->scale + QVector2D(t,t)).length() >= 0.1) this->scale += QVector2D(t,t);
    event->accept();
}


void subSlice(std::vector<std::vector<Line3D>> lines){

}
