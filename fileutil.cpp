#include "fileutil.h"
using namespace std;

Mesh * FileUtil::LoadObj(std::string filename, std::string objname) {
    Mesh *mesh;
    string line, tag;
    float x, y, z;
    string sa, sb, sc;
    unsigned a, b, c;
    string mtl_file_path, mtl_name, group_name;

    int num_v = 0;

    ifstream in(filename);
    if (in) {
        mesh = new Mesh(objname);
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

Mesh * FileUtil::LoadObj(QString filename)
{
//    QFile f(filename);

//    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        qDebug()<<"Error: Obj file cannot be opened!";
//        return 0;
//    }

//    QTextStream ts(&f);

//    float x,y,z;
//    unsigned a,b,c;
//    Mesh obj = new Mesh();

//    while(!ts.atEnd())
//    {
//        QString line = ts.readLine();

//        QStringList strList = line.split(QRegExp("(\\s+)|/"));
//        strList.removeAll("");

//        if(strList.size() <= 0)
//            continue;

//        if(strList[0] == "v")
//        {
//            x = strList[1].toFloat();
//            y = strList[2].toFloat();
//            z = strList[3].toFloat();
//            obj.AddVertice(Vec3(x, y, z));
//        }

//        if(strList[0] == "vt")
//        {
//            x = strList[1].toFloat();
//            y = strList[2].toFloat();
//            obj.AddUV({ x, y });
//        }

//        if(strList[0] == "vn")
//        {
//            Point3 tmp(strList[1].toFloat(),strList[2].toFloat(),strList[3].toFloat());
//            m_normals.push_back(tmp);
//            x = strList[1].toFloat();
//            y = strList[2].toFloat();
//            z = strList[3].toFloat();
//            obj.AddNormal(Vec3(x, y, z));
//        }

//        if(strList[0] == "usemtl")
//        {
//            if(tMesh!=NULL)
//            {
//                tMesh->dataCount=tCount;
//                tCount=0;
//                m_meshes.push_back(tMesh);
//                //delete tMesh;
//            }
//            tMesh=new Mesh;
//            tMesh->dataOffset=offset;
//            for(int i=0;i<m_mtls.size();i++)
//            {
//                if(strList[1] == m_mtls[i]->Name)
//                {
//                    tMesh->material=m_mtls[i];
//                }
//            }
//        }

//        if(strList[0] == "f")
//        {

//            a = strList[1].toINT();
//            b = strList[4].toShort();
//            c = strList[7].toShort();
//            obj.AddFace({ a, b, c });
//        }
//    }
    return new Mesh();
}

//void FileUtil::WriteObj(std::string filename, Obj obj) {
//    ObjUnion objUnion;
//    objUnion += obj;
//    WriteObjUnion(filename, objUnion);
//}

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
        poses.push_back({ Eigen::Quaterniond(w,x,y,z), Vec3(a,b,c) });
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

vector<Eigen::Vector2d> FileUtil::ReadUV(string filename) {
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

vector<Vec3> FileUtil::ReadVec3d(string filename) {
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
