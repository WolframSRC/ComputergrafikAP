#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent):QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void MyGLWidget::initializeGL(){
    qDebug().nospace().noquote() << "Actual OpenGL version: " << QString::number(this->context()->format().version().first) + "." + QString::number(this->context()->format().version().second);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    QVector2D screen(this->width(), this->height());
    terra = new Terrain(screen);
}

void MyGLWidget::resizeGL(int width, int height)
{
    glViewport(0.0f, 0.0f, width, height);
    this->update();
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0.0f, 0.0f, this->width(), this->height());
    QMatrix4x4 matrix;
    QVector3D pos;
    pos.setX(viewx+deltaxtrans*0.2f);
    pos.setY(viewy+deltaytrans*0.2f);
    pos.setZ(zoom);
    matrix = initMatrix();
    terra->draw(matrix,pos);
    update();
}

QMatrix4x4 MyGLWidget::initMatrix(){
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(viewx+deltaxtrans*0.2f, viewy+deltaytrans*0.2f, zoom) ;
    matrix.rotate(deltaxrot, 1.0f,0.0f,0.0f);
    matrix.rotate(deltayrot,0.0f,1.0f,0.0f);
    return matrix;
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_D) {viewx +=5;}
    else if(event->key() == Qt::Key_A) {viewx -=5;}
    else if (event->key() == Qt::Key_W){ viewy +=5;}
    else if(event->key() == Qt::Key_S){ viewy -=5;}
    else {QOpenGLWidget::keyPressEvent(event);}
    this->update();
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
   if(event->delta()==0)return;
   int step = event->delta()/(8*15);
   zoom+=step*5;
   if(zoom < -900)zoom = -900;
   this->update();
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
        lastPoint = event->localPos();
        mousepressed = true;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) && mousepressed){
       deltaxtrans += lastPoint.x()-event->localPos().x();
       deltaytrans += lastPoint.y()-event->localPos().y();
       lastPoint= event->localPos();
    }
    if ((event->buttons() == Qt::RightButton) && mousepressed){
       deltayrot += lastPoint.x()-event->localPos().x();
       deltaxrot += lastPoint.y()-event->localPos().y();
       lastPoint= event->localPos();
    }
    if(deltaxrot>360)deltaxrot -=360;
    if(deltayrot>360)deltayrot -=360;
    if(deltaxrot<-360)deltaxrot +=360;
    if(deltayrot<-360)deltayrot +=360;
    update();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && mousepressed) {
        deltaxtrans += lastPoint.x()-event->localPos().x();
        deltaytrans += lastPoint.y()-event->localPos().y();
        mousepressed = false;
    }
    if (event->button() == Qt::RightButton && mousepressed){
       deltayrot += lastPoint.x()-event->localPos().x();
       deltaxrot += lastPoint.y()-event->localPos().y();
       lastPoint= event->localPos();
       mousepressed = false;
    }
    if(deltaxrot>360)deltaxrot -=360;
    if(deltayrot>360)deltayrot -=360;
    if(deltaxrot<-360)deltaxrot +=360;
    if(deltayrot<-360)deltayrot +=360;
    update();
}
