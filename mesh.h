#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "transform.h"

struct Face
{
    unsigned v0, v1, v2;
};

struct Vertex
{
    Vec3 position;
    Vec3 normal;
    Vec2 texcoord;
};

class Mesh
{
public:
    Mesh();
    Mesh(std::string name);
    Mesh(const Mesh * mesh);
    ~Mesh();

    void Update(const std::vector<Vec3>& p, const std::vector<Vec3>& n);
    void Update(const Transform& t);

    Mesh operator * (Transform t);
    void operator +=(const Mesh& mesh);

    void SetName(const std::string& name);
    void SetMtl(const std::string& mtl);
    void SetMtlFile(const std::string& path);

    void AddVertice(const Vec3& v);
    void AddNormal(const Vec3& n);
    void AddUV(const Vec2 & uv);
    void AddFace(const Face& f);
    void SetTransform(const Transform& t);

    void SetVisible(bool visible);
    void SetChanged(bool changed);

    const std::string &GetName();
    const std::string &GetMtl();
    const std::string &GetMtlFilePath();

    const std::vector<Vec3>& GetVertices();
    const std::vector<Vec3>& GetNormals();
    const std::vector<Vec2>& GetUVs();
    const std::vector<Face>& GetFaces();
    const Transform& GetTransform();

    bool IsVisible();
    bool IsChanged();

    std::vector<int> FindUselessVertices();
    std::vector<int> FindLostIndices(Mesh mesh);

    void CalculateNormal();

    friend class MeshUnion;
public:
    std::vector<Face> faces_;
    std::vector<Vec3> positions_;
    std::vector<Vec3> normals_;
    std::vector<Vec2> texcoords_;
    Transform transform_;
    std::string name_, mtl_file_path_, mtl_name_;

    bool visible_{true};
    bool changed_{false};
    bool use_transform{false};
    bool has_texture_{false};
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
