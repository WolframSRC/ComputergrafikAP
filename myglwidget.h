#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include <QWidget>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QOpenGLContext>
#include <stack>
#include <QOpenGLDebugLogger>
#include "terrain.h"

class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    MyGLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
public
    slots:  

    signals:

private:
    float aspect = 1;

    float deltaytrans=0;
    float deltaxtrans=0;
    QPointF lastPoint;
    bool mousepressed=false;

    float deltaxrot=0;
    float deltayrot=0;

    int viewy=0;
    int zoom=0;
    int viewx=0;

    QMatrix4x4 initMatrix();

    Terrain *terra=nullptr;

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MYGLWIDGET_H
