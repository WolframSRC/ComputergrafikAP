#include "grass.h"
Grass::Grass(){
    pos.setX(1);
    pos.setY(1);
    pos.setZ(1);
    pos.setW(1);
}

Grass::Grass(float x, float y, float z, float w, QOpenGLShaderProgram *shader){
    pos.setX(x);
    pos.setY(y);
    pos.setZ(z);
    pos.setW(w);
    this->shader = shader;
}

void Grass::draw(QMatrix4x4 matrix,QMatrix4x4 matrixprojection,int ibolength){
    int attrVertices = shader->attributeLocation("vert"); // #version 130
    shader->enableAttributeArray(attrVertices);

    int attrTexCoords = shader->attributeLocation("texCoord");
    shader->enableAttributeArray(attrTexCoords);

    int unifTexture = this->shader->uniformLocation("texture");
    this->shader->setUniformValue(unifTexture, 0);

    int unifMatrix = this->shader->uniformLocation("matrix");
    this->shader->setUniformValue(unifMatrix,matrixprojection* matrix);

    int offset = 0;
    int stride = 8 * sizeof(GLfloat);
    this->shader->setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
    offset = 4 * sizeof(GLfloat);
    this->shader->setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);

    glDrawElements(GL_TRIANGLES, ibolength, GL_UNSIGNED_INT, 0);

    this->shader->disableAttributeArray(attrVertices);
    this->shader->disableAttributeArray(attrTexCoords);
}
