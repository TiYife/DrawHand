#include "render.h"

Render::Render()
{
    clear();
}

Render::~Render()
{
    clear();
}

void Render::clear()
{
    mesh_map_.clear();
//    for(auto& it = mesh_map_.begin(); it!=mesh_map_.end();){
//        it = mesh_map_.erase(it);
//    }
}

void Render::addMesh(Mesh *mesh, QString textrue_path)
{
    unique_ptr<QOpenGLShaderProgram> shader(new QOpenGLShaderProgram());
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resource/shaders/hand.vert"))
        return;
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resource/shaders/hand.frag"))
        return;
    if (!shader->link())
        return;


    unique_ptr<QOpenGLTexture> texture(new QOpenGLTexture(QImage(textrue_path).mirrored()));
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    mesh_map_[mesh] = unique_ptr<RenderMesh>(new RenderMesh(mesh, shader, texture));
}

void Render::addMesh(Mesh *mesh, QColor color)
{
    unique_ptr<QOpenGLShaderProgram> shader(new QOpenGLShaderProgram());
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resource/shaders/default.vert"))
        return;
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resource/shaders/default.frag"))
        return;
    if (!shader->link())
        return;
    mesh_map_[mesh] = unique_ptr<RenderMesh>(new RenderMesh(mesh, shader, color));
}

void Render::drawAll(QMatrix4x4 mat)
{
    for(auto& it = mesh_map_.begin(); it != mesh_map_.end();it++){
        it->second->draw(mat);
    }
}
