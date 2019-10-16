#include "fileutil.h"
using namespace std;

unique_ptr<Mesh> FileUtil::LoadObj(std::string filename, std::string objname) {
    unique_ptr<Mesh> mesh(new Mesh(objname));
    string line, tag;
    float x, y, z;
    string sa, sb, sc;
    unsigned a, b, c;
    string mtl_file_path, mtl_name, group_name;

    int num_v = 0;

    ifstream in(filename);
    if (!in) {
        return unique_ptr<Mesh>(nullptr);
    }
    while (getline(in, line))
    {
        istringstream ss(line);
        ss >> tag;
        if (tag == "v") {
            ss >> x >> y >> z;
            mesh ->AddVertice(Vec3(x, y, z));
            num_v++;
        }
        else if (tag == "vn") {
            ss >> x >> y >> z;
            mesh ->AddNormal(Vec3(x, y, z));
        }
        else if (tag == "vt") {
            ss >> x >> y;
            mesh ->AddUV({ x, y });
        }
        else if (tag == "f") {
            ss >> sa >> sb >> sc;
            a = b = c = 0;
            for (int i = 0; sa[i] != '/'&&i<sa.length(); i++) {
                a = a * 10 + sa[i] - '0';
            }
            for (int i = 0; sb[i] != '/'&&i<sb.length(); i++) {
                b = b * 10 + sb[i] - '0';
            }
            for (int i = 0; sc[i] != '/'&&i<sc.length(); i++) {
                c = c * 10 + sc[i] - '0';
            }
            //a = stoi(sa);  b = stoi(sb);  c = stoi(sc);
            mesh ->AddFace({ a - 1u, b - 1u, c - 1u });
        }
        else if (tag == "mtllib") {
            ss >> mtl_file_path;
            mesh ->SetMtlFile(mtl_file_path);
        }
        else if (tag == "usemtl") {
            ss >> mtl_name;
            mesh ->SetMtl(mtl_name);
        }
        else if (tag == "g") {
            ss >> group_name;//todo
        }
        tag = "";
    }
    in.close();
    return mesh;
}

unique_ptr<Mesh> FileUtil::LoadObj(QFile& file, string objname)
{
    unique_ptr<Mesh> mesh(new Mesh(objname));
    string line, tag;
    float x, y, z;
    string sa, sb, sc;
    unsigned a, b, c;
    string mtl_file_path, mtl_name, group_name;

    int num_v = 0;

    file.open(QIODevice::ReadOnly);
    ifstream in(::_fdopen(file.handle(), "r"));
    if (!in) {
        return unique_ptr<Mesh>(nullptr);
    }
    while (getline(in, line))
    {
        istringstream ss(line);
        ss >> tag;
        if (tag == "v") {
            ss >> x >> y >> z;
            mesh ->AddVertice(Vec3(x, y, z));
            num_v++;
        }
        else if (tag == "vn") {
            ss >> x >> y >> z;
            mesh ->AddNormal(Vec3(x, y, z));
        }
        else if (tag == "vt") {
            ss >> x >> y;
            mesh ->AddUV({ x, y });
        }
        else if (tag == "f") {
            ss >> sa >> sb >> sc;
            a = b = c = 0;
            for (int i = 0; sa[i] != '/'&&i<sa.length(); i++) {
                a = a * 10 + sa[i] - '0';
            }
            for (int i = 0; sb[i] != '/'&&i<sb.length(); i++) {
                b = b * 10 + sb[i] - '0';
            }
            for (int i = 0; sc[i] != '/'&&i<sc.length(); i++) {
                c = c * 10 + sc[i] - '0';
            }
            //a = stoi(sa);  b = stoi(sb);  c = stoi(sc);
            mesh ->AddFace({ a - 1u, b - 1u, c - 1u });
        }
        else if (tag == "mtllib") {
            ss >> mtl_file_path;
            mesh ->SetMtlFile(mtl_file_path);
        }
        else if (tag == "usemtl") {
            ss >> mtl_name;
            mesh ->SetMtl(mtl_name);
        }
        else if (tag == "g") {
            ss >> group_name;//todo
        }
        tag = "";
    }
    in.close();
    return mesh;
}

void FileUtil::LoadTransfroms(const string& filename, vector<Eigen::Vector3f> &vertex, vector<Eigen::Vector3f> &normal, vector<Transform> &list)
{
    real_t a, b, c;
    real_t x, y, z, w;
    string line, tag;

    vertex.clear();
    normal.clear();
    list.clear();
    list.resize(5);
    ifstream in(filename);
    if (!in) {
        return;
    }
    while (getline(in, line))
    {
        istringstream ss(line);
        ss >> tag;
        if (tag == "vertex") {
            ss >> x >> y >> z;
            vertex.push_back(Vec3(x, y, z));
        }
        else if (tag == "normal") {
            ss >> x >> y >> z;
            normal.push_back(Vec3(x, y, z));
        }
        else if (tag == "ball") {
            ss >> a >> b >> c >> x >> y >> z >> w;
            list[0] = Transform(Quat(w,x,y,z), Vec3(a,b,c));
        }
        else if (tag == "cube") {
            ss >> a >> b >> c >> x >> y >> z >> w;
            list[1] = Transform(Quat(w,x,y,z), Vec3(a,b,c));
        }
        else if (tag == "banana") {
            ss >> a >> b >> c >> x >> y >> z >> w;
            list[2] = Transform(Quat(w,x,y,z), Vec3(a,b,c));
        }
        else if (tag == "torus") {
            ss >> a >> b >> c >> x >> y >> z >> w;
            list[3] = Transform(Quat(w,x,y,z), Vec3(a,b,c));
        }
        else if (tag == "cube2") {
            ss >> a >> b >> c >> x >> y >> z >> w;
            list[4] = Transform(Quat(w,x,y,z), Vec3(a,b,c));
        }

        tag = "";
    }
    in.close();
}

vector<vector<int>> FileUtil::LoadKeyIndices(QString filename)
{
    vector<vector<int>> ans;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        return ans;
    }
    int id;
    while(!file.atEnd()){
        vector<int> indices;
        QByteArray line = file.readLine();
        QString str(line);
        QStringList list = str.split(" ");
        for(auto & data: list){
           id = data.toInt();
           if(id!=0)
               indices.push_back(id);
        }
        ans.push_back(indices);
    }
    return ans;
}

void FileUtil::WriteKeyPos(const QString& filename, std::vector<Eigen::Vector3f>& pos)
{
    ofstream file;
    file.open(QFileInfo(filename).absoluteFilePath().toStdString());
    for(auto& p : pos){
        file << p.x() <<" " << p.y() <<" " << p.z() <<"\n";
    }
    file.close();
}

string FileUtil::GetTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string s = to_string(ltm->tm_year + 1900) + "-" + to_string(ltm->tm_mon + 1) + "-" + to_string(ltm->tm_mday) + "-" + to_string(ltm->tm_hour) + "-" + to_string(ltm->tm_min);
    //delete ltm;
    return s;
}

vector<Transform> FileUtil::ReadPose(string filename) {
    vector<Transform> poses;
    int i;
    double a, b, c;
    double x, y, z, w;
    ifstream in(filename);
    string line;
    while (getline(in, line)) {
        istringstream ss(line);
        ss >> i >> a >> b >> c >> x >> y >> z >> w;
        poses.push_back({ Quat(w,x,y,z), Vec3(a,b,c) });
    }
    return poses;
}

vector<Face> FileUtil::readIndex(string filename) {
    vector<Face> faces;
    unsigned a, b, c;
    ifstream in(filename);
    string line;
    while (getline(in, line)) {
        istringstream ss(line);
        ss >>  a >> b >> c;
        faces.push_back({ a , b, c});
    }
    return faces;
}

vector<Vec2> FileUtil::ReadUV(string filename) {
    vector<Vec2> data;
    double x, y;
    ifstream in(filename);
    string line;
    while (getline(in, line)) {
        istringstream ss(line);
        ss >> x >> y;
        data.push_back(Vec2(x,y));
    }
    return data;
}

vector<Vec3> FileUtil::ReadVec3(string filename) {
    vector<Vec3> data;
    double x, y, z;
    ifstream in(filename);
    string line;
    while (getline(in, line)) {
        istringstream ss(line);
        ss >> x >> y >> z;
        data.push_back(Vec3(x,y,z));
    }
    return data;
}

string FileUtil::GetObjectName(ObjectType type) {
    string name;
    switch (type)
    {
    case TYPE_BALL:
        name = "ball";
        break;
    case TYPE_CUBE:
        name = "cube";
        break;
    case TYPE_BANANA:
        name = "banana";
        break;
    case TYPE_TORUS:
        name = "torus";
        break;
    case TYPE_CUBE2:
        name = "cube2";
        break;
    case OBJECT_TYPE_MAX:
    default:
        break;
    }

    return name;
}
