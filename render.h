#ifndef RENDER_H
#define RENDER_H
#include <unordered_map>
#include "rendermesh.h"
using namespace std;

class Render
{
public:
    Render();
    ~Render();

    void addMesh(Mesh* mesh, QString textrue_path);
    void addMesh(Mesh *mesh, QColor color);
    void drawAll(QMatrix4x4 mat);
private:
    void clear();

    unordered_map<Mesh*, unique_ptr<RenderMesh>> mesh_map_;
};

#endif // RENDER_H
