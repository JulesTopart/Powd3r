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
    //glEnable(GL_LINE_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,.01f);
    glEnable(GL_LIGHT0);      // Enable Lighting
}

void GLWidget::paintGL(){
    if (!light) glDisable(GL_LIGHTING);     // Disable Lighting
    else        glEnable(GL_LIGHTING);      // Enable Lighting

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);



    glTranslatef(position.x(), position.y(), position.z());
    //glRotatef(rotation.length(), rotation.normalized().x(), rotation.normalized().y(), rotation.normalized().z());
    glRotatef(rotation.x(), 1, 0, 0);
    glRotatef(rotation.y(), 0, 1, 0);
    glRotatef(rotation.z(), 0, 0, 1);

    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);             // Setup The Diffuse Light

    glScalef(scale.x(), scale.y(), scale.z());
    //Draw axis
    drawAxis();
    glColor3f(0.5,0.5,0.48);

    //Draw grid
    drawGrid(200);
    glColor3f(0.5,0.5,0.48f); //retablish default color


    for(QVector<mesh::Model>::Iterator model = models.begin(); model != models.end(); model ++){
        glPushMatrix();
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //WireFrame
        glScalef(model->getScale().x(), model->getScale().y(), model->getScale().z());
        glTranslatef(model->getPosition().x(), model->getPosition().y(), model->getPosition().z());
        glRotatef(model->getRotation().x(), 1, 0, 0);
        glRotatef(model->getRotation().y(), 0, 1, 0);
        glRotatef(model->getRotation().z(), 0, 0, 1);
        model->draw();
        glPopMatrix();
    }

    rotation += rotationSpeed;     // Add speed To rotation
}

void GLWidget::drawGrid(int grid_size)
{
    int HALF_GRID_SIZE = grid_size/2;
    glBegin(GL_LINES);
    glColor3f(0.75f, 0.75f, 0.75f);
    for(int i=-HALF_GRID_SIZE;i<=HALF_GRID_SIZE;i+=10)
    {
        glVertex3f((float)i,0,(float)-HALF_GRID_SIZE);
        glVertex3f((float)i,0,(float)HALF_GRID_SIZE);

        glVertex3f((float)-HALF_GRID_SIZE,0,(float)i);
        glVertex3f((float)HALF_GRID_SIZE,0,(float)i);
    }
    glEnd();
}

void GLWidget::drawAxis(){
    glEnable(GL_LINE_SMOOTH);
    glPushMatrix();
    glTranslatef(0,0.06,0);
    glBegin(GL_LINES);
      glColor3f(1,0,0);
      glVertex2i(0,0);glVertex2i(0,10);
      glColor3f(0,1,0);
      glVertex2i(0,0);glVertex2i(10,0);
      glColor3f(0,0,1);
      glVertex2i(0,0);glVertex3i(0,0,10);
    glEnd();
    glPopMatrix();
    glDisable(GL_LINE_SMOOTH);
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

            position += QVector3D(0,1,0);
            break;
        case Qt::Key_Down:
            position -= QVector3D(0,1,0);
            break;
        case Qt::Key_Right:
            position += QVector3D(1,0,0);
            break;
        case Qt::Key_Left:
            position -= QVector3D(1,0,0);
            break;
        case Qt::Key_Q:
            rotationSpeed -= QVector3D(0,1.0f,0);
            break;
        case Qt::Key_D:
            rotationSpeed += QVector3D(0,1.0f,0);
            break;
        case Qt::Key_Z:
            rotationSpeed -= QVector3D(1.0f,0,0);;
            break;
        case Qt::Key_S:
            rotationSpeed += QVector3D(1.0f,0,0);;
            break;
        case Qt::Key_Space:
            rotationSpeed = QVector3D(0,0,0);
            break;
    }
}


void GLWidget::wheelEvent( QWheelEvent *event){
    float t = event->angleDelta().y() * 0.0005;
    if((this->scale + QVector3D(t,t,t)).length() >= 0.1) this->scale += QVector3D(t,t,t);
    event->accept();
}


void GLWidget::timeOutSlot()
{
    updateGL();
}


void GLWidget::loadModel(mesh::Model *mdl){
    mdl->setId(models.size());
    models.push_back(*mdl);
}

void GLWidget::unloadModel(int id){
    models.remove(id);
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        rightMousePressed = true;
        mousePressPosition = QVector2D(event->x(), event->y());
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        rightMousePressed = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    if (rightMousePressed)
    {
        // Mouse release position - mouse press position
        QVector2D diff = QVector2D(event->localPos()) - mousePressPosition;

        // Rotation axis is perpendicular to the mouse position difference
        // vector
        QVector3D n = QVector3D(diff.y(), diff.x(), 0.0);
        rotation += n;

        mousePressPosition = QVector2D(event->x(), event->y());
        event->accept();
        return;
    }
    event->ignore();

}


void GLWidget::timerEvent(QTimerEvent *)
{

}

void GLWidget::TopView()
{
    // TODO: Add your command handler code here
    rotation = QVector3D(0,0,0);
}

void GLWidget::BottomView()
{
    // TODO: Add your command handler code here
    rotation = QVector3D(-180,0,0);
}

void GLWidget::FrontView()
{
    // TODO: Add your command handler code here
    rotation = QVector3D(-90,0,0);
}

void GLWidget::BackView()
{
    // TODO: Add your command handler code here
    rotation = QVector3D(-90,0,-180);
}

void GLWidget::LeftView()
{
    rotation = QVector3D(-90,0,-90);
}

void GLWidget::RightView()
{
    rotation = QVector3D(-90,0,90);
}

void GLWidget::AxonView()
{
rotation = QVector3D(-45,0,-45);
}

