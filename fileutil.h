#ifndef FILEUTIL_H
#define FILEUTIL_H
#include <fstream>
#include <string>
#include <unordered_map>
#include <QtCore>
#include <QFile>
#include <QStringList>
#include "mesh.h"
using namespace std;

enum ObjectType {
    TYPE_BALL = 0, TYPE_CUBE, TYPE_BANANA, TYPE_TORUS, TYPE_CUBE2, OBJECT_TYPE_MAX
};


static const QString kBasePath = "C:/CLAP/data/";

class FileUtil{
public:
    static unique_ptr<Mesh> LoadObj(std::string filename, string objname);

    static unique_ptr<Mesh> LoadObj(QFile& file, string objname);

    static void LoadTransfroms(const string& filename, vector<Vec3>& vertex, vector<Vec3>& normal, vector<Transform>& list);

    static vector<vector<int>> LoadKeyIndices(QString filename);

    static vector<QVec3> LoadKeyPoints(QString filename);



    static void WriteKeyPos(const QString& filename, std::vector<Vec3>& pos);

    static void WriteKeyPos(const QString& filename, std::vector<QVec3>& pos);


    static void  WriteHand(const QString& filename,Mesh *mesh);

    static string GetTime();




    static vector<Transform> ReadPose(string filename );

    static vector<Face> readIndex(string filename);

    static vector<Vec2> ReadUV(string filename);

    static vector<Vec3> ReadVec3(string filename);

    static string GetObjectName(ObjectType type);
};
#endif // FILEUTIL_H
