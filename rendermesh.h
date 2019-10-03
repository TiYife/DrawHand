#ifndef RENDERMESH_H
#define RENDERMESH_H

#include "mesh.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
using namespace std;

struct RenderVertex{
    QVec3 position;
    QVec3 normal;
    QVec2 texcoord;
};

class RenderMesh: protected QOpenGLFunctions
{
public:
    RenderMesh(Mesh * mesh);
    virtual ~RenderMesh();

    virtual void draw(QMat4 view, QMat4 projection) = 0;

    void initialize();


protected:
    Mesh * mesh_;
    std::vector<RenderVertex> vertices_;
    QOpenGLBuffer arrayBuffer;
    QOpenGLBuffer indexBuffer;
};


class TextureRenderMesh : public RenderMesh
{
public:
    TextureRenderMesh(Mesh * mesh, QString textrue_path);
    virtual ~TextureRenderMesh();

    void initshader();
    void inittexture(QString textrue_path);
    void update();
    virtual void draw(QMat4 view, QMat4 projection) override;

private:
    unique_ptr<QOpenGLTexture> texture_;
    unique_ptr<QOpenGLShaderProgram> shader_;
};


class SimpleRenderMesh : public RenderMesh
{
public:
    SimpleRenderMesh(Mesh * mesh, QColor color);
    virtual ~SimpleRenderMesh();

    void initshader();
    void update();
    virtual void draw(QMat4 view, QMat4 projection) override;

private:
    QColor color_;
    unique_ptr<QOpenGLShaderProgram> shader_;
};

#endif // RENDERMESH_H
