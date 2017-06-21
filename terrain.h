#ifndef TERRAIN_H
#define TERRAIN_H
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include "modelloader.h"
#include "konstanten.h"
#include "grass.h"

class Terrain{
public:
    Terrain();
    Terrain(QVector2D screen);
    void draw(QMatrix4x4 matrix, QVector3D pos);

private:
    QOpenGLBuffer* vbo; // Typ festlegen
    QOpenGLBuffer* ibo;
    QOpenGLTexture* qTex;

    void initBuffer();
    bool initModel();
    void initShaders();
    void inittex();
    void initProjectionmatrix();
    void initGrass(QVector2D screen);
    void setGrassPos(QVector3D pos);
    GLfloat *vboData=nullptr;
    GLuint *indexData=nullptr;
    QOpenGLShaderProgram *shader = nullptr;
    int anzahlgrass=0;
    float aspect=1;
    QMatrix4x4 matrixprojection;
    unsigned int vboLength;
    unsigned int iboLength;
    Grass* grassarray;
    QVector4D oldPos;
};

#endif // TERRAIN_H
