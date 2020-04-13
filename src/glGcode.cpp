#include "glGcode.h"

QString codeToArray(QString code){
    int codeInt = code.toInt();
    QString result = "000000000000";
    for(int i=0; codeInt>0; i++)
    {
        result[i]=QString::number(codeInt%2)[0];
        codeInt = codeInt/2;
    }
    return result;
}

float lookForValue(char key, QString msg){
    QStringList pack = msg.split(' ');
    for (int i(0); i < pack.length();i++) {
        if(pack[i].contains(key)){
            float result = pack[i].right(pack[i].length() - 1).toFloat();
            return result;
        }
    }
    return 0;
}




glGcode::~glGcode()
{

}

glGcode::glGcode(QWidget *parent) :
    QGLWidget(parent)
{
    int seconde = 1000; // 1 seconde = 1000 ms
    int timerInterval = seconde / 60;
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    t_Timer->start( timerInterval );
}

void NLayer::draw(){
    foreach (NSweep s, sweeps) {
        s.draw(z);
    }
}

void NSweep::draw(float z){
    float previousX = x;
    foreach (NState s, states) {
        for (int i(0); i < 12; i++) {
            if(s.nozzles[i]){
                glColor3f(1,0.8f,0.0f);
                glBegin(GL_LINES);
                    glVertex3f(previousX,y+(i*0.2645),z);
                    glVertex3f(s.x,y+(i*0.2645),z);
                glEnd();
            }
        }
        previousX = s.x;
    }
    glPointSize(4.0);
    glColor3f(1,0.2f,0.2f);
    glBegin(GL_POINTS);
        glVertex2f(x,y);
    glEnd();
}


void glGcode::initializeGL(){
    glShadeModel(GL_SMOOTH);
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
}

void glGcode::generatePath(QString code){
    QStringList Lines = code.split('\n');
    float z = 0.0;
    NLayers layers;
    NSweeps sweeps;
    NStates states;

    foreach (QString line, Lines) {
        if(line.contains(';'))line.chop(line.length() - line.indexOf(';'));
        if(line[0] == ';' || line[0] == '\n'){

        }else{
            QString cmd = line.split(' ')[0];
            QString parameters = line.right(line.length() - cmd.length());
            if(cmd == "G1"){
                if(parameters.contains("Z")){
                    z += lookForValue('Z', parameters);
                    if(sweeps.size() > 0){
                        NLayer l(z,sweeps);
                        layers.push_back(l);
                        sweeps.clear();
                        states.clear();
                    }
                }else if(parameters.contains("S")){
                    float x = lookForValue('X', parameters);
                    int code = lookForValue('S', parameters);
                    NState s(x, code);
                    states.push_back(s);
                }else if(parameters.contains("X") && parameters.contains("Y")){
                    if(states.size() > 0 && sweeps.size() > 0){
                        sweeps.last().states = states;
                    }
                    states.clear();
                    float x = lookForValue('X', parameters);
                    float y = lookForValue('Y', parameters);
                    NSweep s(x, y, states);
                    sweeps.push_back(s);
                }
            }
        }
        if(states.size() > 0 && sweeps.size() > 0){
            sweeps.last().states = states;
        }
    }

    _layers = layers;
}

void glGcode::paintGL(){

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
    drawGrid(QVector2D(plateDim.x(),plateDim.y()));


    //Draw grid
    //drawGrid(200);
    glColor3f(0.5,0.5,0.48f); //retablish default color

    glPushMatrix();
    glColor3f(1,0.8f,0.0f);
    glTranslatef(originOffset.x() - plateDim.x()/2, originOffset.y() - plateDim.y()/2, 0);
    glDisable(GL_LIGHT1);
    glEnable(GL_LIGHT2);        // Enable Lighting
    glEnable(GL_LIGHT3);
    GLfloat ambient[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  LightDiffuse);
    glLightfv(GL_LIGHT3, GL_AMBIENT,  ambient);
    if(_layers.size() > 0) _layers[activeLayer].draw();
    glPopMatrix();

    /*
    unsigned int index = 0;
    foreach (NLayer l, _layers) {
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT2);        // Enable Lighting
        glEnable(GL_LIGHT3);
        GLfloat ambient[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
        glLightfv(GL_LIGHT2, GL_DIFFUSE,  LightDiffuse);
        glLightfv(GL_LIGHT3, GL_AMBIENT,  ambient);
        if(index == activeLayer)l.draw();
        glPopMatrix();
    }
    */
    rotation += rotationSpeed;     // Add speed To rotation
}

void glGcode::drawAxis(){
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

void glGcode::drawGrid(QVector2D size)
{
    float HALF_GRID_X = size.y()/2;
    float HALF_GRID_Y = size.x()/2;

    glBegin(GL_LINES);
    for(int i =- HALF_GRID_Y; i <= HALF_GRID_Y; i += 10 )
    {
        glVertex2f((float)i,(float)-HALF_GRID_X);
        glVertex2f((float)i,(float)HALF_GRID_X);
    }

    for(int i=-HALF_GRID_X;i<=HALF_GRID_X;i += 10){
        glVertex2f((float)-HALF_GRID_Y,(float)i);
        glVertex2f((float)HALF_GRID_Y,(float)i);
    }

    glEnd();
}

void glGcode::resizeGL(int width, int height){
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void glGcode::keyPressEvent(QKeyEvent *keyEvent)
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


void glGcode::wheelEvent( QWheelEvent *event){
    float t = event->angleDelta().y() * 0.0002f;
    if((this->scale + QVector3D(t,t,t)).length() >= 0.1) this->scale += QVector3D(t,t,t);
    event->accept();
}


void glGcode::timeOutSlot()
{
    updateGL();
}

void glGcode::mousePressEvent(QMouseEvent *event){
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

void glGcode::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        leftMousePressed = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void glGcode::mouseMoveEvent( QMouseEvent* event){
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




void glGcode::timerEvent(QTimerEvent *)
{

}



void glGcode::TopView()
{
    // TODO: Add your command handler code here
    position = QVector3D(0,0,-50);
    rotation = QVector3D(90,0,0);

}

void glGcode::BottomView()
{
    // TODO: Add your command handler code here
    position = QVector3D(0,0,-50);
    rotation = QVector3D(-90,0,0);
}

void glGcode::FrontView()
{
    // TODO: Add your command handler code here
    position = QVector3D(0,-10,-50);
    rotation = QVector3D(0,0,0);
    scale    = QVector3D(0.5,0.5,0.5);
}

void glGcode::BackView()
{
    // TODO: Add your command handler code here
    position = QVector3D(0,0,-50);
    rotation = QVector3D(0,0,-180);
}

void glGcode::LeftView()
{
    position = QVector3D(0,0,-50);
    rotation = QVector3D(0,0,-90);
}

void glGcode::RightView()
{
    position = QVector3D(0,0,-50);
    rotation = QVector3D(0,0,90);
}

void glGcode::AxonView()
{
    position = QVector3D(0,0,-50);
    rotation = QVector3D(-45,0,-45);
}

