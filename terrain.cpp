#include "terrain.h"

Terrain::Terrain(){
    vbo= new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    ibo= new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    bool temptexCoord = initModel();
    this->initBuffer();
    this->initShaders();
    initProjectionmatrix();
    if(temptexCoord)this->inittex();
}

void Terrain::initBuffer(){
    //Erzeuge Vertex-Buffer
    vbo->create();
    vbo->bind();
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo->allocate(vboData, sizeof(GLfloat) * vboLength);
    vbo->release();
    // Erzeuge Index-Buffer
    ibo->create();
    ibo->bind();
    ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo->allocate(indexData, sizeof(GLuint) * iboLength);
    ibo->release();
}

bool Terrain::initModel(){
    ModelLoader model;
    QString path = MODEL_PATH+MODEL_GRASS;
    bool res = model.loadObjectFromFile(path.toStdString());
    if (res) {
        vboLength = model.lengthOfVBO(0.0f,false,true);
        iboLength = model.lengthOfIndexArray();

        this->vboData = new GLfloat[vboLength];
        this->indexData = new GLuint[iboLength];

        model.genVBO(vboData,0.0f,false,true);
        model.genIndexArray(indexData);
    }
    else{
        qDebug().nospace() << "Fehler beim Laden des Model";
    }
    return model.hasTextureCoordinates();
}

void Terrain::initShaders() {
    this->shader = new QOpenGLShaderProgram();
    this->shader->removeAllShaders();
    this->shader->addShaderFromSourceFile(QOpenGLShader::Vertex, SHADER_PATH + TEXTURE_VERTEX_SHADER);
    this->shader->addShaderFromSourceFile(QOpenGLShader::Fragment,  SHADER_PATH + TEXTURE_FRAGMENT_SHADER);
    this->shader->link();
}

void Terrain::inittex(){
    QString path = TEXTURE_PATH+TEXTURE_GRASS;
    qTex = new QOpenGLTexture(QImage(path).mirrored());
    qTex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    qTex->setMagnificationFilter(QOpenGLTexture::Linear);
    if( qTex->textureId() == 0 ){
        qDebug().nospace() << "Fehler beim Laden der Textur";
    }
}

void Terrain::initProjectionmatrix(){
    matrixprojection.setToIdentity();
    matrixprojection.perspective(60.0f,aspect,0.1,1000);
}

void Terrain::initGrass(QVector4D pos){
    float x = pos.x()-10*anzahlgrass;
    float y = pos.y();
    float z= pos.z()-10*anzahlgrass;
    float w = pos.w();
    for(int i = 0;i<anzahlgrass;i+=3){
        this->grassarray[i+0]= new Grass(x,y,z,w,shader);
        this->grassarray[i+1]= new Grass(x+10,y,z,w,shader);
        this->grassarray[i+2]= new Grass(x,y,z+10,w,shader);
        this->grassarray[i+3]= new Grass(x+10,y,z+10,w,shader);
        x+=10;
        z+=10;
    }
}

void Terrain::draw(QMatrix4x4 matrix,QVector4D pos){
    shader->bind();
    vbo->bind();
    ibo->bind();
    qTex->bind();
    initGrass(pos);
    for(int i =0;i<anzahlgrass;i++){
        this->grassarray[i]->draw(matrix,matrixprojection,this->iboLength);
    }

    qTex->release();
    shader->release();
    vbo->release();
    ibo->release();
}
