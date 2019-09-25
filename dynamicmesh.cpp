#include "dynamicmesh.h"

DynamicMesh::DynamicMesh()
{

}

DynamicMesh::DynamicMesh(Mesh *obj)
{
//    this->faces_ = obj->faces_;
    for(int i = 0;i<obj->vertices_.size();i++){
//        this->vertices_.push_back({obj->vertices_[i], obj->normals_[i],obj->UVs_[i]});
        this->vertices_.push_back({obj->vertices_[i], obj->vertices_[i] ,obj->UVs_[i]});

    }

    for(auto& f: obj->faces_){
        faces_.push_back({unsigned short(f.v0-1), unsigned short(f.v1 - 1), unsigned short(f.v2 - 1)});
    }
}

float CalcAngleBetween(const Vec3 &dir_a, const Vec3 &dir_b) {
    float length_dot = dir_a.norm() * dir_b.norm();

    if (length_dot < 0.0001f) {
        length_dot = 0.0001f;
    }
    float f = dir_a.dot(dir_b) / length_dot;
//    f = std::clamp(f, -1.0f, 1.0f);
    f = f < -1.0 ? -1.0 : (f > 1.0 ? 1.0 : f);

    return acosf(f);
}

void DynamicMesh::CalculateNormal()
{
    for(auto & v:vertices_){
        v.normal=Vec3(0);
    }
    Vec3 pos[3];
    Vec3 dir10,dir20;
    Vec3 norm;

    for(int i = 0;i < faces_.size()/3;i++ ){
        pos[0] = vertices_[faces_[i].v0].position;
        pos[1] = vertices_[faces_[i].v1].position;
        pos[2] = vertices_[faces_[i].v2].position;

        dir10 = pos[1] - pos[0];
        dir20 = pos[2] - pos[0];

        norm = dir20.cross(dir10);
//        std::cout<<"norm:\t"<<norm.x<<"\t"<<norm.y<<"\t"<<norm.z<<"\n";

        for (int j = 0; j < 3; j++) {
            // weight by angle to fix the L-Shape problem
            float weight = CalcAngleBetween(pos[(j + 1) % 3] - pos[j], pos[(j + 2) % 3] - pos[j]);
            if (weight <= 0.0f) {
                weight = 0.0001f;
            }
//            std::cout<<"weight_"<<faces_[3*i+j]<<":\t"<<weight<<"\t";
            if(j == 0)
                vertices_[faces_[i].v0].normal += norm * weight;
            else if(j == 1)
                vertices_[faces_[i].v1].normal += norm * weight;
            else
                vertices_[faces_[i].v2].normal += norm * weight;
        }
//        std::cout<<"\n";
    }
}
