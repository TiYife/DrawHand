#ifndef RENDERMESH_H
#define RENDERMESH_H

#include "mesh.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector2D>

struct RenderVertex{
    QVector3D position;
    QVector3D normal;
    QVector2D texcoord;
};

class RenderMesh: protected QOpenGLFunctions
{
public:
    RenderMesh(Mesh * mesh);
    virtual ~RenderMesh();

    void initialize();
    void draw(QOpenGLShaderProgram * program);

    void update();
    Mesh * mesh_;
private:

    std::vector<RenderVertex> vertices_;
    QOpenGLBuffer arrayBuffer;
    QOpenGLBuffer indexBuffer;
};

#endif // RENDERMESH_H
