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

    virtual void update() = 0;
    virtual void draw(QMat4 view, QMat4 projection, bool mask = 0) = 0;


protected:
    void initialize();
    void MaskMode(bool mask_mode_);
    virtual void initColorShader() = 0;
    virtual void initMaskShader() = 0;

protected:
    Mesh * mesh_;
    std::vector<RenderVertex> vertices_;
    QOpenGLBuffer arrayBuffer;
    QOpenGLBuffer indexBuffer;
    unique_ptr<QOpenGLShaderProgram> shader_;
};


class TextureRenderMesh : public RenderMesh
{
public:
    TextureRenderMesh(Mesh * mesh, QString textrue_path);
    virtual ~TextureRenderMesh();


    void initTexture(QString textrue_path);

    virtual void update() override;
    virtual void initColorShader() override;
    virtual void initMaskShader() override;
    virtual void draw(QMat4 view, QMat4 projection, bool mask = 0) override;

private:
    unique_ptr<QOpenGLTexture> texture_;
};


class SimpleRenderMesh : public RenderMesh
{
public:
    SimpleRenderMesh(Mesh * mesh, QColor color);
    virtual ~SimpleRenderMesh();

    virtual void update() override;

    virtual void initColorShader() override;
    virtual void initMaskShader() override;
    virtual void draw(QMat4 view, QMat4 projection, bool mask = 0) override;

private:
    QColor color_;
};

#endif // RENDERMESH_H
