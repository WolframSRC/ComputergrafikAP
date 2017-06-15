#ifndef GRASS_H
#define GRASS_H
#include <QMatrix4x4>
#include <QVector4D>
#include <QOpenGLShaderProgram>
class Grass{
public:
    void draw(QMatrix4x4 matrix, QMatrix4x4 matrixprojection, int ibolength);
    Grass(float x, float y, float z, float w, QOpenGLShaderProgram *shader);
    Grass();
private:
    QVector4D pos;
    QOpenGLShaderProgram *shader=nullptr;
};

#endif // GRASS_H
