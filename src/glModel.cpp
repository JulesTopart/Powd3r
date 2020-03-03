#include "glModel.h"


GLModel::~GLModel()
{

}

GLModel::GLModel(QWidget *parent) :
    QGLWidget(parent)
{
    int seconde = 1000; // 1 seconde = 1000 ms
    int timerInterval = seconde / 60;
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    t_Timer->start( timerInterval );
}

void GLModel::initializeGL(){

    glClearColor(0.9f, 0.9f, 0.9f, 1);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_LINE_SMOOTH);

    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);        // Enable Lighting
    glEnable(GL_LIGHT2);        // Enable Lighting
}

void GLModel::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //Camera pos
    glTranslatef(position.x(), position.y(), position.z());

    glRotatef(rotation.x(), 1, 0, 0);
    glRotatef(rotation.y(), 0, 1, 0);
    glRotatef(rotation.z(), 0, 0, 1);

    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glEnable(GL_LIGHT1);        // Enable Lighting
    glLightfv(GL_LIGHT1, GL_AMBIENT,  LightAmbient);      // Setup The Ambient Light
    //Draw grid
    glScalef(scale.x(), scale.y(), scale.z());
    drawGrid(plateDim);

    glColor3f(0.5,0.5,0.48f); //retablish default color

    glRotatef(-90, 1, 0, 0); //mê !

    glPushMatrix();
    glTranslatef(originOffset.x() - plateDim.x()/2, originOffset.y() - plateDim.y()/2, originOffset.z());
    drawAxis();
    glPopMatrix();

    glColor3f(0.5,0.5,0.48f);



    int index(0);
    for(QVector<Mesh>::Iterator model = models.begin(); model != models.end(); model ++, index++){
        if(showBoundaries){
            glPushMatrix();
            model->drawBB();
            glPopMatrix();
        }

        glPushMatrix();
        if(index == selectedModel) glColor3f(1,0.8,0.0);
        else glColor3f(0.8,0.8,0.78f);

        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT2);        // Enable Lighting
        glEnable(GL_LIGHT3);
        GLfloat ambient[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
        glLightfv(GL_LIGHT2, GL_DIFFUSE,  LightDiffuse);
        glLightfv(GL_LIGHT3, GL_AMBIENT,  ambient);
        model->draw();
        glPopMatrix();
    }

    rotation += rotationSpeed;     // Add speed To rotation
}

void GLModel::drawGrid(QVector3D size)
{
    float HALF_GRID_X = size.y()/2;
    float HALF_GRID_Y = size.x()/2;
    float GRID_Z = size.z();

    glBegin(GL_LINES);
    glColor3f(0.65f, 0.65f, 0.65f);

    for(int i=-HALF_GRID_Y;i<=HALF_GRID_Y;i+=10)
    {
        glVertex3f((float)i,0,(float)-HALF_GRID_X);
        glVertex3f((float)i,0,(float)HALF_GRID_X);
    }
    for(int i=-HALF_GRID_X;i<=HALF_GRID_X;i+=10){
        glVertex3f((float)-HALF_GRID_Y,0,(float)i);
        glVertex3f((float)HALF_GRID_Y,0,(float)i);
    }

    glColor3f(0.5f, 0.8f, 1.0f);
    glVertex3f(HALF_GRID_Y,0,HALF_GRID_X);
    glVertex3f(HALF_GRID_Y,GRID_Z,HALF_GRID_X);

    glVertex3f(-HALF_GRID_Y,0,HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,GRID_Z,HALF_GRID_X);

    glVertex3f(HALF_GRID_Y,0,-HALF_GRID_X);
    glVertex3f(HALF_GRID_Y,GRID_Z,-HALF_GRID_X);

    glVertex3f(-HALF_GRID_Y,0,-HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,GRID_Z,-HALF_GRID_X);

    //Square :
    glVertex3f(HALF_GRID_Y,GRID_Z,HALF_GRID_X);
    glVertex3f(HALF_GRID_Y,GRID_Z,-HALF_GRID_X);

    glVertex3f(-HALF_GRID_Y,GRID_Z,HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,GRID_Z,-HALF_GRID_X);

    glVertex3f(HALF_GRID_Y,GRID_Z,HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,GRID_Z,HALF_GRID_X);

    glVertex3f(HALF_GRID_Y,GRID_Z,-HALF_GRID_X);
    glVertex3f(-HALF_GRID_Y,GRID_Z,-HALF_GRID_X);


    glEnd();
}

void GLModel::drawAxis(){
    glPushMatrix();
    glLineWidth(2.2);
    //glTranslatef(0.1f,0.1f,0.1f);
    glBegin(GL_LINES);
      glColor3f(1,0,0);
      glVertex2i(0,0);glVertex2i(20,0);
      glColor3f(0,1,0);
      glVertex2i(0,0);glVertex2i(0,20);
      glColor3f(0,0,1);
      glVertex2i(0,0);glVertex3i(0,0,20);
    glEnd();
    glLineWidth(1);
    glPopMatrix();
}

void GLModel::resizeGL(int width, int height){
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void GLModel::keyPressEvent(QKeyEvent *keyEvent)
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


void GLModel::wheelEvent( QWheelEvent *event){
    float t = event->angleDelta().y() * 0.0002f;
    if((this->scale + QVector3D(t,t,t)).length() >= 0.1) this->scale += QVector3D(t,t,t);
    event->accept();
}


void GLModel::timeOutSlot()
{
    updateGL();
}

void GLModel::loadModel(Mesh *mdl){
    mdl->applyChange();
    models.push_back(*mdl);
    models[models.size() - 1].setId(models.size() - 1);
}

void GLModel::unloadModel(int id){
    models.remove(id);
    for(int i(0); i < models.size(); i++){
        models[i].setId(i);
    }
}

void GLModel::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        leftMousePressed = true;
        mousePressPosition = QVector2D(event->x(), event->y());
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }else if(event->button() == Qt::RightButton){
        rightMousePressed = true;
        mousePressPosition = QVector2D(event->x(), event->y());
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void GLModel::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        leftMousePressed = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
        return;
    }else if(event->button() == Qt::RightButton){
        rightMousePressed = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void GLModel::mouseMoveEvent(QMouseEvent *event){
    if (leftMousePressed)
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
    if (rightMousePressed)
    {
        // Mouse release position - mouse press position
        QVector2D diff = QVector2D(event->localPos()) - mousePressPosition;
        diff *= 0.1;
        // Rotation axis is perpendicular to the mouse position difference
        // vector
        QVector3D n = QVector3D(diff.x(), - diff.y(), 0.0);
        position += n;

        mousePressPosition = QVector2D(event->x(), event->y());
        event->accept();
        return;
    }
    event->ignore();

}




void GLModel::timerEvent(QTimerEvent *)
{

}



void GLModel::centerOnModel(int i){
    QVector3D pos = models[i].getPosition();
    QVector3D bb  = models[i].getBBSize();
    position = pos + (bb / 2);
}

void GLModel::TopView()
{
    // TODO: Add your command handler code here
    position = QVector3D(0,0,-50);
    rotation = QVector3D(90,0,0);

}

void GLModel::BottomView()
{
    // TODO: Add your command handler code here
    position = QVector3D(0,0,-50);
    rotation = QVector3D(-90,0,0);
}

void GLModel::FrontView()
{
    // TODO: Add your command handler code here
    position = QVector3D(0,-10,-50);
    rotation = QVector3D(0,0,0);
    scale    = QVector3D(0.5,0.5,0.5);
}

void GLModel::BackView()
{
    // TODO: Add your command handler code here
    position = QVector3D(0,0,-50);
    rotation = QVector3D(0,0,-180);
}

void GLModel::LeftView()
{
    position = QVector3D(0,0,-50);
    rotation = QVector3D(0,0,-90);
}

void GLModel::RightView()
{
    position = QVector3D(0,0,-50);
    rotation = QVector3D(0,0,90);
}

void GLModel::AxonView()
{
    position = QVector3D(0,0,-50);
    rotation = QVector3D(-45,0,-45);
}

