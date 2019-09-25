#include "rendermesh.h"

RenderMesh::RenderMesh(Mesh *mesh):
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

//    for(auto& v: vertices_){
//        std::cout<<v.position.x()<<" "<<v.position.y()<<" "<<v.position.z()<<std::endl;
//    }
//    if(mesh_!=nullptr){
//        for(auto & f : mesh_->faces_){
//            std::cout<<f.v0<<" "<<f.v1<<" "<<f.v2<<std::endl;
//        }

//    }


    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh_->faces_.size() * sizeof(Face)), GL_UNSIGNED_INT, 0);
}

void RenderMesh::update()
{
//    if(!mesh_)
//        return;
//    QVector3D p,n;
//    QVector2D t;
//    for(size_t i = 0; i < mesh_->positions_.size();i++){
//        p = QVector3D(mesh_->positions_[i].x(), mesh_->positions_[i].y(), mesh_->positions_[i].z());
//        n = QVector3D(mesh_->normals_[i].x(), mesh_->normals_[i].y(), mesh_->normals_[i].z());
//        t = QVector2D(mesh_->texcoords_[i].x(), mesh_->texcoords_[i].y());
//        vertices_.push_back({p,n,t});
//    }


    if(!mesh_)
        return;
    QVector3D p,n;
    QVector2D t;
    Vec3 position, normal;

    for(size_t i = 0; i < mesh_->positions_.size();i++){
        position = mesh_->transform_ * mesh_->positions_[i];
        normal = mesh_->transform_ * mesh_->normals_[i];
        p = QVector3D(position.x(), position.y(), position.z());
        n = QVector3D(normal.x(), normal.y(), normal.z());
        t = QVector2D(mesh_->texcoords_[i].x(), mesh_->texcoords_[i].y());
        std::cout<<position.x()<<" "<<position.y()<<" "<<position.z()<<std::endl;
        vertices_.push_back({p,n,t});
    }

}
