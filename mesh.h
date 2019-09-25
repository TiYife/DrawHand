#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include <iostream>
#include "transform.h"

class Mesh
{
public:
    Mesh();
    Mesh(std::string name);
    Mesh(const Mesh * mesh);
    ~Mesh();

    void Update(std::vector<Vec3> p, std::vector<Vec3> n);

    Mesh Transfrom(Transform t);
    Mesh operator * (Transform t);
    void operator +=(const Mesh& mesh);

    void SetName(std::string name);
    void SetMtl(std::string mtl);
    void SetMtlFile(std::string path);

    void AddVertice(Vec3 v);
    void AddNormal(Vec3 n);
    void AddUV(const Vec2 & uv);
    void AddFace(Face f);

    const std::string &GetName();
    const std::string &GetMtl();
    const std::string &GetMtlFilePath();

    const std::vector<Vec3>& GetVertices();
    const std::vector<Vec3>& GetNormals();
    const std::vector<Vec2>& GetUVs();
    const std::vector<Face>& GetFaces();

    std::vector<int> FindUselessVertices();
    std::vector<int> FindLostIndices(Mesh mesh);

    friend class MeshUnion;

private:

    Mesh Transfrom(const Eigen::Isometry3d& t);
    void CalculateNormal();

public:
    std::vector<Face> faces_;
    std::vector<Vec3> positions_;
    std::vector<Vec3> normals_;
    std::vector<Vec2> texcoords_;
    Transform transform_;
    std::string name_, mtl_file_path_, mtl_name_;
};

class MeshUnion
{
public:
    MeshUnion();
    ~MeshUnion();

    void operator +=(const Mesh& mesh);
    void clear();

public:
    std::vector<Mesh> meshes;
    size_t maxFaceId = 0;
};

#endif // MESH_H
