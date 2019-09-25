#include "mesh.h"


Mesh::Mesh()
{
}

Mesh::Mesh(std::string name)
{
    this->name_ = name;
}

Mesh::Mesh(const Mesh * obj)
{
    this->name_ = obj->name_;
    this->vertices_ = obj->vertices_;
    this->normals_ = obj->normals_;
    this->UVs_ = obj->UVs_;
    this->faces_ = obj->faces_;
}


Mesh::~Mesh()
{
    faces_.clear();
    UVs_.clear();
    normals_.clear();
    vertices_.clear();
}

void Mesh::Update(std::vector<Eigen::Vector3d> p, std::vector<Eigen::Vector3d> n)
{
    this->vertices_ = p;
    this->normals_ = n;
}

Mesh Mesh::Transfrom(const Pose& p)
{
    Eigen::Isometry3d T = Eigen::Isometry3d::Identity();
    T.rotate(p.q.toRotationMatrix());
    T.pretranslate(p.p);
    return this->Transfrom(T);
}

Mesh Mesh::operator*(const Eigen::Isometry3d& t)
{
    return this->Transfrom(t);
}

void Mesh::operator+=(const Mesh & obj)
{
    this->vertices_.insert(vertices_.end(), obj.vertices_.begin(), obj.vertices_.end());
    this->normals_.insert(normals_.end(), obj.normals_.begin(), obj.normals_.end());
    this->UVs_.insert(UVs_.end(), obj.UVs_.begin(), obj.UVs_.end());
    int size = this->vertices_.size();
    for (auto& f : obj.faces_) {
        faces_.push_back({ static_cast<unsigned short>(f.v0 + size), static_cast<unsigned short>(f.v1 + size),
                           static_cast<unsigned short>(f.v2 + size) });
    }
}

void Mesh::SetName(std::string name)
{
    this->name_ = name;
}

void Mesh::SetMtl(std::string mtl)
{
    mtl_name_ = mtl;
}

void Mesh::SetMtlFile(std::string path)
{
    mtl_file_path_ = path;
}

void Mesh::AddVertice(Eigen::Vector3d v)
{
    vertices_.push_back(v);
}

void Mesh::AddNormal(Eigen::Vector3d n)
{
    normals_.push_back(n);
}

void Mesh::AddUV(const Vec2 & uv)
{
    UVs_.push_back(uv);
}

void Mesh::AddFace(Face f)
{
    faces_.push_back(f);
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

const std::vector<Eigen::Vector3d>& Mesh::GetVertices()
{
    return vertices_;
}

const std::vector<Eigen::Vector3d>& Mesh::GetNormals()
{
    return normals_;
}

const std::vector<Vec2>& Mesh::GetUVs()
{
    return UVs_;
}

const std::vector<Face>& Mesh::GetFaces()
{
    return faces_;
}

std::vector<int> Mesh::FindUselessVertices() {
    std::vector<int> list(vertices_.size(), 1);
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

std::vector<int> Mesh::FindLostIndices(Mesh obj)
{
    std::vector<int> list;
    for(int i = 0; i < this->vertices_.size();i++){
        if(std::find(obj.vertices_.begin(), obj.vertices_.end(), vertices_[i])==obj.vertices_.end()){
            list.push_back(i+1);
        }
    }
    return list;
}



Mesh Mesh::Transfrom(const Eigen::Isometry3d& t)
{
    Mesh obj(this->name_);
    obj.faces_ = this->faces_;
    obj.UVs_ = this->UVs_;
    for (auto& p : vertices_) {
        obj.vertices_.push_back(t * p);
    }

    for (auto& n : normals_) {
        obj.normals_.push_back(t * n);
    }
    return obj;
}

ObjUnion::ObjUnion()
{
    clear();
}

ObjUnion::~ObjUnion()
{
    clear();
}

void ObjUnion::operator+=(const Mesh & obj)
{
    Mesh new_obj = obj;

    for (auto &i : new_obj.faces_) {
        i.v0 += this->maxFaceId;
        i.v1 += this->maxFaceId;
        i.v2 += this->maxFaceId;
    }
    maxFaceId += obj.vertices_.size();
    objs.push_back(new_obj);
}

void ObjUnion::clear()
{
    objs.clear();
}
