#ifndef MESHBUILDERS_H
#define MESHBUILDERS_H
#include "mesh.h"
using namespace std;

class MeshBuilders
{
public:
    static unique_ptr<Mesh> CreateSphere(Vec3 pos, real_t r);
};

#endif // MESHBUILDERS_H
