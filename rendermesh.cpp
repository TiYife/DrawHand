#include "rendermesh.h"

RenderMesh::RenderMesh(Mesh *mesh, unique_ptr<QOpenGLShaderProgram>& shader, QColor color):
    mesh_(mesh),
    indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    mesh_=mesh;
    shader_ = move(shader);
    color_ = color;
    initialize();
}

RenderMesh::RenderMesh(Mesh *mesh, unique_ptr<QOpenGLShaderProgram>& shader, unique_ptr<QOpenGLTexture>& texture):
    mesh_(mesh),
    indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    shader_ = move(shader);
    texture_ = move(texture);
    initialize();
}

//RenderMesh::RenderMesh(const RenderMesh &render_mesh)
//{
//    shader_ = move(render_mesh.shader_);
//    texture_ = move(render_mesh.texture_);
//    color_ = render_mesh.color_;
//    vertices_ = render_mesh.vertices_;
//    arrayBuffer = render_mesh.arrayBuffer;
//    indexBuffer = render_mesh.indexBuffer;
//}

RenderMesh::~RenderMesh(){
    arrayBuffer.destroy();
    indexBuffer.destroy();
}

void RenderMesh::initialize()
{
    //todo init faces and vertexes

    update();

    arrayBuffer.create();
    arrayBuffer.bind();
    arrayBuffer.allocate(&vertices_[0], static_cast<int>(vertices_.size() * sizeof(RenderVertex)));

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(&mesh_->faces_[0],static_cast<int>(mesh_->faces_.size() * sizeof(Face)));
}

void RenderMesh::draw(QMatrix4x4 mat)
{
    if (!shader_->bind())
        return;

    shader_->setUniformValue("mvp_matrix", mat);
    if(texture_){
        texture_->bind();
        shader_->setUniformValue("texture", 0);//todo ? 0
    }
    else{
        shader_->setUniformValue("color", color_);
    }

    arrayBuffer.bind();
    indexBuffer.bind();

//    mesh_->Update();
    //mesh_->CalculateNormal();

    quintptr offset = 0;
    int vertex_location = shader_->attributeLocation("a_position");
    shader_ -> enableAttributeArray(vertex_location);
    shader_ -> setAttributeBuffer(vertex_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

    offset += sizeof(QVector3D);
    int normal_location = shader_->attributeLocation("a_normal");
    shader_ -> enableAttributeArray(normal_location);
    shader_ -> setAttributeBuffer(normal_location, GL_FLOAT, offset, 3, sizeof(RenderVertex));

    offset += sizeof(QVector3D);
    int texcoord_location = shader_ -> attributeLocation("a_texcoord");
    shader_ -> enableAttributeArray(texcoord_location);
    shader_ -> setAttributeBuffer(texcoord_location, GL_FLOAT, offset, 2, sizeof(RenderVertex));

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
    Vec3 position;
    Vec3 normal = Vec3(0,0,0);
    Vec2 texcoords = Vec2(0,0);
    for(size_t i = 0; i < mesh_->positions_.size();i++){
        position = mesh_->transform_ * mesh_->positions_[i];
        if(mesh_->normals_.size()!=0)
            normal = mesh_->transform_ * mesh_->normals_[i];
        if(texture_ && mesh_->texcoords_.size()!=0)
            texcoords = mesh_->texcoords_[i];
        p = QVector3D(position.x(), position.y(), position.z());
        n = QVector3D(normal.x(), normal.y(), normal.z());
        t = QVector2D(texcoords.x(), texcoords.y());
//        std::cout<<position.x()<<" "<<position.y()<<" "<<position.z()<<std::endl;
        vertices_.push_back({p,n,t});
    }

}
