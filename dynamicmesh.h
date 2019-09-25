#ifndef DYNAMICMESH_H
#define DYNAMICMESH_H

#include "mesh.h"

class DynamicMesh
{
public:
    DynamicMesh();
    DynamicMesh(Mesh * obj);

    void CalculateNormal();
    void Update();
    friend class RenderMesh;
private:
    std::vector<Face> faces_;
    std::vector<Vertex> vertices_;
};

#endif // DYNAMICMESH_H
