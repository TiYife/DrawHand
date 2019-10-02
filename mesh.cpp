#include "mesh.h"


Mesh::Mesh()
{
}

Mesh::Mesh(std::string name)
{
    this->name_ = name;
}

Mesh::Mesh(const Mesh * mesh)
{
    this->name_ = mesh->name_;
    this->positions_ = mesh->positions_;
    this->normals_ = mesh->normals_;
    this->texcoords_ = mesh->texcoords_;
    this->faces_ = mesh->faces_;
}


Mesh::~Mesh()
{
    faces_.clear();
    texcoords_.clear();
    normals_.clear();
    positions_.clear();
}

void Mesh::Update(std::vector<Vec3> p, std::vector<Vec3> n)
{
    this->positions_ = p;
    this->normals_ = n;
}

Mesh Mesh::Transfrom(Transform t)
{
    Mesh mesh(this->name_);
    mesh.faces_ = this->faces_;
    mesh.texcoords_ = this->texcoords_;
    for (auto& p : positions_) {
        mesh.positions_.push_back(t * p);
    }

    for (auto& n : normals_) {
        mesh.normals_.push_back(t * n);
    }
    return mesh;
}

Mesh Mesh::operator*(Transform t)
{
    return this->Transfrom(t);
}

void Mesh::operator+=(const Mesh & mesh)
{
    this->positions_.insert(positions_.end(), mesh.positions_.begin(), mesh.positions_.end());
    this->normals_.insert(normals_.end(), mesh.normals_.begin(), mesh.normals_.end());
    this->texcoords_.insert(texcoords_.end(), mesh.texcoords_.begin(), mesh.texcoords_.end());
    size_t size = this->positions_.size();
    for (auto& f : mesh.faces_) {
        faces_.push_back({ static_cast<unsigned>(f.v0 + size), static_cast<unsigned>(f.v1 + size),
                           static_cast<unsigned>(f.v2 + size) });
    }
}

void Mesh::SetName(const std::string& name)
{
    this->name_ = name;
}

void Mesh::SetMtl(const std::string& mtl)
{
    mtl_name_ = mtl;
}

void Mesh::SetMtlFile(const std::string& path)
{
    mtl_file_path_ = path;
}

void Mesh::AddVertice(const Vec3& v)
{
    positions_.push_back(v);
}

void Mesh::AddNormal(const Vec3& n)
{
    normals_.push_back(n);
}

void Mesh::AddUV(const Vec2 & uv)
{
    texcoords_.push_back(uv);
    has_texture_ = true;
}

void Mesh::AddFace(const Face& f)
{
    faces_.push_back(f);
}

void Mesh::SetTransform(const Transform& t)
{
    transform_ = t;
    use_transform = true;
}

const Transform& Mesh::GetTransform()
{
    return transform_;
}

const std::string & Mesh::GetName()
{
    return name_;
}

const std::string & Mesh::GetMtl()
{
    return mtl_name_;
}

const std::string & Mesh::GetMtlFilePath()
{
    return mtl_file_path_;
}

const std::vector<Vec3>& Mesh::GetVertices()
{
    return positions_;
}

const std::vector<Vec3>& Mesh::GetNormals()
{
    return normals_;
}

const std::vector<Vec2>& Mesh::GetUVs()
{
    return texcoords_;
}

const std::vector<Face>& Mesh::GetFaces()
{
    return faces_;
}

std::vector<int> Mesh::FindUselessVertices() {
    std::vector<int> list(positions_.size(), 1);
    std::vector<int> res;
    for (auto& f : faces_) {
        list[f.v0 - 1] = 0;
        list[f.v1 - 1] = 0;
        list[f.v2 - 1] = 0;
    }
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == 1) {
            res.push_back(i);
        }
    }
    return res;
}

std::vector<int> Mesh::FindLostIndices(Mesh mesh)
{
    std::vector<int> list;
    for(int i = 0; i < this->positions_.size();i++){
        if(std::find(mesh.positions_.begin(), mesh.positions_.end(), positions_[i])==mesh.positions_.end()){
            list.push_back(i+1);
        }
    }
    return list;
}

float CalcAngleBetween(const Vec3 &dir_a, const Vec3 &dir_b) {
    float length_dot = dir_a.norm() * dir_b.norm();

    if (length_dot < 0.0001f) {
        length_dot = 0.0001f;
    }
    float f = dir_a.dot(dir_b) / length_dot;
    f = f < -1.0 ? -1.0 : (f > 1.0 ? 1.0 : f);

    return acosf(f);
}


void Mesh::CalculateNormal()
{
    for(auto & n : normals_){
        n=Vec3(0);
    }
    Vec3 pos[3];
    Vec3 dir10,dir20;
    Vec3 norm;

    for(int i = 0;i < faces_.size()/3;i++ ){
        pos[0] = positions_[faces_[i].v0];
        pos[1] = positions_[faces_[i].v1];
        pos[2] = positions_[faces_[i].v2];

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
                normals_[faces_[i].v0] += norm * weight;
            else if(j == 1)
                normals_[faces_[i].v1] += norm * weight;
            else
                normals_[faces_[i].v2] += norm * weight;
        }
//        std::cout<<"\n";
    }
}

MeshUnion::MeshUnion()
{
    clear();
}

MeshUnion::~MeshUnion()
{
    clear();
}

void MeshUnion::operator+=(const Mesh & mesh)
{
    Mesh new_mesh = mesh;

    for (auto &i : new_mesh.faces_) {
        i.v0 += this->maxFaceId;
        i.v1 += this->maxFaceId;
        i.v2 += this->maxFaceId;
    }
    maxFaceId += mesh.positions_.size();
    meshes.push_back(new_mesh);
}

void MeshUnion::clear()
{
    meshes.clear();
}
