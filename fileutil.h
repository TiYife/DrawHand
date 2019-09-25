#ifndef FILEUTIL_H
#define FILEUTIL_H
#include <fstream>
#include <string>
#include <QtCore>
#include <QFile>
#include <QStringList>
#include "mesh.h"
using namespace std;

enum ObjectType {
    TYPE_BALL = 0, TYPE_CUBE, TYPE_BANANA, TYPE_TORUS, TYPE_CUBE2, OBJECT_TYPE_MAX
};

class FileUtil{
public:
    static Mesh * LoadObj(std::string filename, string objname);

//    static vector<int> LoadMarkPoints

    static Mesh * LoadObj(QString filename);

    //static void WriteObjUnion(string filename, ObjUnion objs);

    static void WriteObj(string filename, Mesh obj);

    static vector<Transform> ReadPose(string filename );

    static vector<Face> readIndex(string filename);

    static vector<Vec2> ReadUV(string filename);

    static vector<Eigen::Vector3d> ReadVec3d(string filename);

    static string GetObjectName(ObjectType type);
};
#endif // FILEUTIL_H
