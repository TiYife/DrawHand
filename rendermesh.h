#ifndef RENDERMESH_H
#define RENDERMESH_H

#include "mesh.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
using namespace std;

struct RenderVertex{
    QVector3D position;
    QVector3D normal;
    QVector2D texcoord;
};

class RenderMesh: protected QOpenGLFunctions
{
public:
    RenderMesh(Mesh * mesh, unique_ptr<QOpenGLShaderProgram>& shader, QColor color);
    RenderMesh(Mesh * mesh, unique_ptr<QOpenGLShaderProgram>& shader, unique_ptr<QOpenGLTexture>& texture);
//    RenderMesh(const RenderMesh & render_mesh);
    virtual ~RenderMesh();

    void initialize();
    void draw(QMatrix4x4 mat);

    void update();
    Mesh * mesh_;
private:
    unique_ptr<QOpenGLShaderProgram> shader_;
    unique_ptr<QOpenGLTexture> texture_;
    QColor color_;
    std::vector<RenderVertex> vertices_;
    QOpenGLBuffer arrayBuffer;
    QOpenGLBuffer indexBuffer;
};

#endif // RENDERMESH_H
