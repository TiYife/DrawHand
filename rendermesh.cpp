#include "rendermesh.h"

RenderMesh::RenderMesh(Mesh *mesh):
    mesh_(mesh),
    indexBuffer(QOpenGLBuffer::IndexBuffer)

{
    initializeOpenGLFunctions();
    arrayBuffer.create();
    indexBuffer.create();
}

RenderMesh::~RenderMesh(){
    arrayBuffer.destroy();
    indexBuffer.destroy();
}

void RenderMesh::initialize()
{
    //todo init faces and vertexes
    arrayBuffer.bind();
    arrayBuffer.allocate(&vertices_[0], static_cast<int>(vertices_.size() * sizeof(RenderVertex)));

    indexBuffer.bind();
    indexBuffer.allocate(&mesh_->faces_[0],static_cast<int>(mesh_->faces_.size() * sizeof(Face)));
}

//void RenderMesh::draw(QMat4 view, QMat4 projection)
//{
//    if (!shader_->bind())
//        return;

//    shader_->setUniformValue("model", mesh_->GetTransform().toQMat4());
//    shader_->setUniformValue("view", view);
//    shader_->setUniformValue("projection", projection);
//    if(texture_){
//        texture_->bind();
//        shader_->setUniformValue("texture", 0);//todo ? 0
//    }
//    else{
//        shader_->setUniformValue("objectColor", color_);
//    }

//    arrayBuffer.bind();
//    indexBuffer.bind();

////    mesh_->Update();
//    //mesh_->CalculateNormal();

//    quintptr offset = 0;
//    int vertex_location = shader_->attributeLocation("a_position");
//    shader_ -> enableAttributeArray(vertex_location);
//    shader_ -> setAttributeBuffer(vertex_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

//    offset += sizeof(QVec3);
//    int normal_location = shader_->attributeLocation("a_normal");
//    shader_ -> enableAttributeArray(normal_location);
//    shader_ -> setAttributeBuffer(normal_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

//    offset += sizeof(QVec3);
//    int texcoord_location = shader_ -> attributeLocation("a_texcoord");
//    shader_ -> enableAttributeArray(texcoord_location);
//    shader_ -> setAttributeBuffer(texcoord_location, GL_FLOAT, offset, 2, sizeof(RenderVertex));

////    for(auto& v: vertices_){
////        std::cout<<v.position.x()<<" "<<v.position.y()<<" "<<v.position.z()<<std::endl;
////    }
////    if(mesh_!=nullptr){
////        for(auto & f : mesh_->faces_){
////            std::cout<<f.v0<<" "<<f.v1<<" "<<f.v2<<std::endl;
////        }

////    }


//    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh_->faces_.size() * sizeof(Face)), GL_UNSIGNED_INT, 0);
//}


TextureRenderMesh::TextureRenderMesh(Mesh * mesh, QString textrue_path):
    RenderMesh(mesh)
{
    update();
    initialize();
    inittexture(textrue_path);
    initshader();
}

TextureRenderMesh::~TextureRenderMesh()
{

}

void TextureRenderMesh::initshader()
{
    shader_ = unique_ptr<QOpenGLShaderProgram>(new QOpenGLShaderProgram());
    if (!shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resource/shaders/hand.vert"))
        return;
    if (!shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resource/shaders/hand.frag"))
        return;
    if (!shader_->link())
        return;
}

void TextureRenderMesh::inittexture(QString textrue_path)
{
    texture_ = unique_ptr<QOpenGLTexture>(new QOpenGLTexture(QImage(textrue_path).mirrored()));
    texture_->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_->setWrapMode(QOpenGLTexture::Repeat);
}

void TextureRenderMesh::update()
{
    if(!mesh_)
        return;
    QVec3 p,n;
    QVec2 t;
    Vec3 position;
    Vec3 normal = Vec3(0,0,0);
    Vec2 texcoords = Vec2(0,0);
    for(size_t i = 0; i < mesh_->positions_.size();i++){
        position = mesh_->positions_[i];
//        normal = mesh_->transform_ * mesh_->normals_[i];
        texcoords = mesh_->texcoords_[i];
        p = QVec3(position.x(), position.y(), position.z());
        n = QVec3(normal.x(), normal.y(), normal.z());
        t = QVec2(texcoords.x(), texcoords.y());
        vertices_.push_back({p,n,t});
    }

}

void TextureRenderMesh::draw(QMatrix4x4 view, QMatrix4x4 projection)
{
    if (!shader_->bind())
        return;

    texture_->bind();
    Transform t = mesh_->GetTransform();
    QMat4 model = t.toQMat4();

    shader_->setUniformValue("model", model);
    shader_->setUniformValue("view", view);
    shader_->setUniformValue("projection", projection);
    shader_->setUniformValue("texture", 0);//todo ? 0

    arrayBuffer.bind();
    indexBuffer.bind();

    quintptr offset = 0;
    int vertex_location = shader_->attributeLocation("a_position");
    shader_ -> enableAttributeArray(vertex_location);
    shader_ -> setAttributeBuffer(vertex_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

    offset += sizeof(QVec3);
    int normal_location = shader_->attributeLocation("a_normal");
    shader_ -> enableAttributeArray(normal_location);
    shader_ -> setAttributeBuffer(normal_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

    offset += sizeof(QVec3);
    int texcoord_location = shader_ -> attributeLocation("a_texcoord");
    shader_ -> enableAttributeArray(texcoord_location);
    shader_ -> setAttributeBuffer(texcoord_location, GL_FLOAT, offset, 2, sizeof(RenderVertex));

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh_->faces_.size() * sizeof(Face)), GL_UNSIGNED_INT, 0);

    texture_->release();
    shader_->release();
}

SimpleRenderMesh::SimpleRenderMesh(Mesh * mesh, QColor color):
    RenderMesh(mesh)
{
    color_ = color;
    update();
    initialize();
    initshader();
}

SimpleRenderMesh::~SimpleRenderMesh()
{

}

void SimpleRenderMesh::initshader()
{
    shader_ = unique_ptr<QOpenGLShaderProgram>(new QOpenGLShaderProgram());
    if (!shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resource/shaders/default.vert"))
        return;
    if (!shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resource/shaders/default.frag"))
        return;
    if (!shader_->link())
        return;
}

void SimpleRenderMesh::update()
{
    if(!mesh_)
        return;
    mesh_->CalculateNormal();
    QVec3 p,n;
    QVec2 t;
    Vec3 position;
    Vec3 normal = Vec3(0,0,0);
    Vec2 texcoords = Vec2(0,0);
    for(size_t i = 0; i < mesh_->positions_.size();i++){
        position = mesh_->positions_[i];
        normal = mesh_->normals_[i];
//        texcoords = mesh_->texcoords_[i];
        p = QVec3(position.x(), position.y(), position.z());
        n = QVec3(normal.x(), normal.y(), normal.z());
        t = QVec2(texcoords.x(), texcoords.y());
        vertices_.push_back({p,n,t});
    }
}

void SimpleRenderMesh::draw(QMatrix4x4 view, QMatrix4x4 projection)
{
    if (!shader_->bind())
        return;
    Transform t = mesh_->GetTransform();
    QMat4 model = t.toQMat4();

    shader_->setUniformValue("model", model);
    shader_->setUniformValue("view", view);
    shader_->setUniformValue("projection", projection);
    shader_->setUniformValue("objectColor", color_);


    arrayBuffer.bind();
    indexBuffer.bind();

    quintptr offset = 0;
    int vertex_location = shader_->attributeLocation("a_position");
    shader_ -> enableAttributeArray(vertex_location);
    shader_ -> setAttributeBuffer(vertex_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

    offset += sizeof(QVec3);
    int normal_location = shader_->attributeLocation("a_normal");
    shader_ -> enableAttributeArray(normal_location);
    shader_ -> setAttributeBuffer(normal_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

    offset += sizeof(QVec3);
    int texcoord_location = shader_ -> attributeLocation("a_texcoord");
    shader_ -> enableAttributeArray(texcoord_location);
    shader_ -> setAttributeBuffer(texcoord_location, GL_FLOAT, offset, 2, sizeof(RenderVertex));

//    glPolygonMode(GL_BACK, GL_LINE);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh_->faces_.size() * sizeof(Face)), GL_UNSIGNED_INT, 0);

    shader_->release();
}
