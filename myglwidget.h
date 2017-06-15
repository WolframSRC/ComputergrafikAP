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

};

#endif // MYGLWIDGET_H
