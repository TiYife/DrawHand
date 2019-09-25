#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include <iostream>
#include "commontype.h"

class Mesh
{
public:
    Mesh();
    Mesh(std::string name);
    Mesh(const Mesh * obj);
    ~Mesh();

    void Update(std::vector<Eigen::Vector3d> p, std::vector<Eigen::Vector3d> n);
    Mesh Transfrom(const Pose& p);
    Mesh operator * (const Eigen::Isometry3d& t);
    void operator +=(const Mesh& obj);

    void SetName(std::string name);
    void SetMtl(std::string mtl);
    void SetMtlFile(std::string path);

    void AddVertice(Eigen::Vector3d v);
    void AddNormal(Eigen::Vector3d n);
    void AddUV(const Vec2 & uv);
    void AddFace(Face f);

    const std::string &GetName();
    const std::string &GetMtl();
    const std::string &GetMtlFilePath();

    const std::vector<Eigen::Vector3d>& GetVertices();
    const std::vector<Eigen::Vector3d>& GetNormals();
    const std::vector<Vec2>& GetUVs();
    const std::vector<Face>& GetFaces();

    std::vector<int> FindUselessVertices();
    std::vector<int> FindLostIndices(Mesh obj);

    friend class ObjUnion;

private:

    Mesh Transfrom(const Eigen::Isometry3d& t);

public:
    std::vector<Face> faces_;
    std::vector<Eigen::Vector3d> vertices_;
    std::vector<Eigen::Vector3d> normals_;
    std::vector<Vec2> UVs_;
    std::string name_, mtl_file_path_, mtl_name_;
};

class ObjUnion
{
public:
    ObjUnion();
    ~ObjUnion();

    void operator +=(const Mesh& obj);
    void clear();

public:
    std::vector<Mesh> objs;
    int maxFaceId = 0;
};

#endif // MESH_H
