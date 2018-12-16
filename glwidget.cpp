#include "glwidget.h"


GLWidget::~GLWidget()
{

}

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    int seconde = 1000; // 1 seconde = 1000 ms
    int timerInterval = seconde / 60;
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    t_Timer->start( timerInterval );
}

void GLWidget::initializeGL(){
    glShadeModel(GL_SMOOTH);
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,.01f);
    glEnable(GL_LIGHT1);      // Enable Lighting
}

void GLWidget::paintGL(){
    if (!light) glDisable(GL_LIGHTING);     // Disable Lighting
    else        glEnable(GL_LIGHTING);      // Enable Lighting

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glTranslatef(0.0f, 0.0f, z);
    glRotatef(rotation.x(), 1, 0, 0);
    glRotatef(rotation.y(), 0, 1, 0);
    glRotatef(rotation.z(), 0, 0, 1);

    //rotation.
    glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);            // Position The Light


    glBegin(GL_LINES);
      glColor3f(1,0,0);
      glVertex2i(0,0);glVertex2i(0,10);
      glColor3f(0,1,0);
      glVertex2i(0,0);glVertex2i(10,0);
      glColor3f(0,0,1);
      glVertex2i(0,0);glVertex3i(0,0,10);
      glColor3f(0.5,0.5,0.48);
    glEnd();

    glScalef(0.5, 0.5, 0.5);
    //glTranslatef(0.0f, -50.0f, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    for(QVector<mesh::Model>::Iterator model = models.begin(); model != models.end(); model ++){
        model->draw();
    }
    xRot+=xSpeed;              // Add xspeed To xrot
    yRot+=ySpeed;              // Add yspeed To yrot
    zRot+=zSpeed;              // Add yspeed To yrot
}

void GLWidget::resizeGL(int width, int height){
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_L:
            light = !light;
            break;
        case Qt::Key_Up:
            z-=1.0f;
            break;
        case Qt::Key_Down:
            z+=1.0f;
            break;
        case Qt::Key_Q:
            ySpeed-=1.0f;
            break;
        case Qt::Key_D:
            ySpeed+=1.0f;
            break;
        case Qt::Key_Z:
            xSpeed-=1.0f;
            break;
        case Qt::Key_S:
            xSpeed+=1.0f;
            break;
        case Qt::Key_Space:
            xSpeed = 0.0f;
            ySpeed = 0.0f;
            break;
    }
}


void GLWidget::mouseMoveEvent( QMouseEvent *mouseEvent){

}

void GLWidget::scrollEvent( QWheelEvent *wheelEvent){

}


void GLWidget::timeOutSlot()
{
    updateGL();
}


void GLWidget::loadModel(mesh::Model mdl){
    models.push_back(mdl);
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}

void GLWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.90;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
        updateGL();
    }
}

void GLWidget::TopView()
{
    // TODO: Add your command handler code here
    xRot = 0;
    yRot = 0;
    zRot = 0;
}

void GLWidget::BottomView()
{
    // TODO: Add your command handler code here
    xRot = -180;
    yRot = 0;
    zRot = 0;
}

void GLWidget::FrontView()
{
    // TODO: Add your command handler code here
    xRot = -90;
    yRot = 0;
    zRot = 0;
}

void GLWidget::BackView()
{
    // TODO: Add your command handler code here
    xRot = -90;
    yRot = 0;
    zRot = -180;
}

void GLWidget::LeftView()
{
    // TODO: Add your command handler code here
    xRot = -90;
    yRot = 0;
    zRot = -90;
}

void GLWidget::RightView()
{
    // TODO: Add your command handler code here
    xRot = -90;
    yRot = 0;
    zRot = 90;
}

void GLWidget::AxonView()
{
    // TODO: Add your command handler code here
    xRot = -45;
    yRot = 0;
    zRot = -45;
}
