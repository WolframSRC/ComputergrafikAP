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
    void draw(QMatrix4x4 matrix, QVector4D pos);
private:
    QOpenGLBuffer* vbo; // Typ festlegen
    QOpenGLBuffer* ibo;
    QOpenGLTexture* qTex;

    void initBuffer();
    bool initModel();
    void initShaders();
    void inittex();
    void initProjectionmatrix();
    void initGrass(QVector4D pos);

    GLfloat *vboData=nullptr;
    GLuint *indexData=nullptr;
    QOpenGLShaderProgram *shader = nullptr;

    float aspect=1;
    QMatrix4x4 matrixprojection;
    unsigned int vboLength;
    unsigned int iboLength;
    Grass* grassarray[anzahlgrass];
};

#endif // TERRAIN_H
