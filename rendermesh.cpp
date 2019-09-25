#include "rendermesh.h"

RenderMesh::RenderMesh(DynamicMesh *mesh):
    mesh_(mesh),
    indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    arrayBuffer.create();
    indexBuffer.create();
    initialize();
}

RenderMesh::~RenderMesh(){
    arrayBuffer.destroy();
    indexBuffer.destroy();
    delete mesh_;
}

void RenderMesh::initialize()
{
    //todo init faces and vertexes

    update();
    for(auto& v: vertices_){
        std::cout<<v.position.x()<<" "<<v.position.y()<<" "<<v.position.z()<<std::endl;
    }
    for(auto & f : mesh_->faces_){
        std::cout<<f.v0<<" "<<f.v1<<" "<<f.v2<<std::endl;
    }
    arrayBuffer.bind();
    arrayBuffer.allocate(&vertices_[0], static_cast<int>(vertices_.size() * sizeof(RenderVertex)));

    indexBuffer.bind();
    indexBuffer.allocate(&mesh_->faces_[0],static_cast<int>(mesh_->faces_.size() * sizeof(Face)));
}

void RenderMesh::draw(QOpenGLShaderProgram * program)
{
    arrayBuffer.bind();
    indexBuffer.bind();

//    mesh_->Update();
    //mesh_->CalculateNormal();

    quintptr offset = 0;
    int vertex_location = program->attributeLocation("a_position");
    program -> enableAttributeArray(vertex_location);
    program -> setAttributeBuffer(vertex_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

    offset += sizeof(QVector3D);
    int normal_location = program->attributeLocation("a_normal");
    program -> enableAttributeArray(normal_location);
    program -> setAttributeBuffer(normal_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

    offset += sizeof(QVector3D);
    int texcoord_location = program -> attributeLocation("a_texcoord");
    program -> enableAttributeArray(texcoord_location);
    program -> setAttributeBuffer(texcoord_location, GL_FLOAT, offset, 2, sizeof(RenderVertex));

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh_->faces_.size() * sizeof(Face)), GL_UNSIGNED_SHORT, 0);
}

void RenderMesh::update()
{
    if(!mesh_)
        return;
    QVector3D p,n;
    QVector2D t;
    for(auto & v: mesh_->vertices_){
        p = QVector3D(v.position.x(), v.position.y(), v.position.z());
        n = QVector3D(v.normal.x(), v.normal.y(), v.normal.z());
        t = QVector2D(v.texcoord.x(), v.texcoord.y());
        vertices_.push_back({p,n,t});
    }
}
