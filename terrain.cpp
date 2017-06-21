#include "terrain.h"

Terrain::Terrain(QVector2D screen){
    vbo= new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    ibo= new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    bool temptexCoord = initModel();
    this->initBuffer();
    this->initShaders();
    initProjectionmatrix();
    if(temptexCoord)this->inittex();
    initGrass(screen);
    oldPos=QVector3D(0,0,INIT_TERRAIN_ZOOM);
}

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
    this->shader->addShaderFromSourceFile(QOpenGLShader::Vertex, SHADER_PATH + TESSELATION_VERTEX_SHADER);    
    this->shader->addShaderFromSourceFile(QOpenGLShader::Fragment,  SHADER_PATH + TESSELATION_FRAGMENT_SHADER);
    //this->shader->addShaderFromSourceFile(QOpenGLShader::TessellationControl,  SHADER_PATH + TESSELATION_TESSELATIONCONTROL_SHADER);
    //this->shader->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation,  SHADER_PATH + TESSELATION_TESSELATIONEVALUATION_SHADER);
    //this->shader->addShaderFromSourceFile(QOpenGLShader::Geometry, SHADER_PATH+ TESSELATION_GEOMENTRY_SHADER);
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

void Terrain::initGrass(QVector2D screen){
   anzahlgrass= (screen.x()*screen.y())/3000;
   anzahlgrass*=30;
   this->grassarray = new Grass[anzahlgrass];
    for(int i =0;i<anzahlgrass;i++){
       this->grassarray[i].setShader(shader);
    }
    float x =-screen.x()/4;
    float y =0;
    float z=INIT_TERRAIN_ZOOM;
    int counter = anzahlgrass/30;
    for(int i = 0;i<anzahlgrass;i+=counter){
        for(int j =0;j<counter;j++){
            this->grassarray[i+j].setPos(x,y,z);
            x+=1.25*2;
        }
        x=-screen.x()/4;
        z+=1.25*2;
    }
}

void Terrain::setGrassPos(QVector3D pos){
    float verschiebungx = oldPos.x()-pos.x();
    float verschiebungz = oldPos.z()-pos.z();
    if(verschiebungz==0&&verschiebungx==0)return;

    if(verschiebungx > 0){
        int offset = anzahlgrass/4;
        for(int i=0;i<anzahlgrass-offset;i++){
            grassarray[i]=grassarray[i+offset];
        }
        for(int i=anzahlgrass-offset;i<anzahlgrass;i++){
            QVector3D temp = grassarray[i].getPos();
            grassarray[i].setPos(temp.x()+verschiebungx,temp.y(),temp.z());
        }
    }
    if(verschiebungx < 0){
        int offset = anzahlgrass/4;
        for(int i=anzahlgrass;i>offset;i--){
            grassarray[i]=grassarray[i-offset];
        }
        for(int i=offset;i>=0;i--){
            QVector3D temp = grassarray[i].getPos();
            grassarray[i].setPos(temp.x()+verschiebungx,temp.y(),temp.z());
        }
    }

    if(verschiebungz > 0){
        int offset = anzahlgrass/4;
        for(int i=0;i<anzahlgrass-offset;i++){
            grassarray[i]=grassarray[i+offset];
        }
        for(int i=anzahlgrass-offset;i<anzahlgrass;i++){
            QVector3D temp = grassarray[i].getPos();
            grassarray[i].setPos(temp.x(),temp.y(),temp.z()+verschiebungz);
        }
    }
    if(verschiebungz <0){
        int offset = anzahlgrass/4;
        for(int i=anzahlgrass;i>offset;i--){
            grassarray[i]=grassarray[i-offset];
        }
        for(int i=offset;i>=0;i--){
            QVector3D temp = grassarray[i].getPos();
            grassarray[i].setPos(temp.x(),temp.y(),temp.z()+verschiebungz);
        }
    }

}

void Terrain::draw(QMatrix4x4 matrix, QVector3D pos){
    shader->bind();
    vbo->bind();
    ibo->bind();
    qTex->bind();
    setGrassPos(pos);
    for(int i =0;i<anzahlgrass;i++){
        this->grassarray[i].draw(matrix,matrixprojection,this->iboLength);
    }

    qTex->release();
    shader->release();
    vbo->release();
    ibo->release();
    oldPos =pos;
}
