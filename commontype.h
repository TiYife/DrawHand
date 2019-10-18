#ifndef COMMONTYPE_H
#define COMMONTYPE_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>

#define real_t float

#define Vec4 Eigen::Vector4f
#define Vec3 Eigen::Vector3f
#define Vec2 Eigen::Vector2f
#define Quat Eigen::Quaternionf
#define Mat4 Eigen::Matrix4f

#define QVec4 QVector4D
#define QVec3 QVector3D
#define QVec2 QVector2D
#define QQuat QQuaternion
#define QMat4 QMatrix4x4


#define M_PI 3.141592653579
#define M_PI_2 M_PI/2

static QVec3 ToQType(const Vec3& e){
    QVec3 q;
    q[0] = e[0];
    q[1] = e[1];
    q[2] = e[2];
    return q;
}

static Vec3 ToEType(const QVec3& q){
    Vec3 e;
    e[0] = q[0];
    e[1] = q[1];
    e[2] = q[2];
    return e;
}

static QVec2 ToQType(const Vec2& e){
    QVec2 q;
    q[0] = e[0];
    q[1] = e[1];
    return q;
}

static QQuat ToQType(const Quat& e){
    QQuat q;
    q.setX(e.x());
    q.setY(e.y());
    q.setZ(e.z());
    q.setScalar((e.w()));
    return q;
}


static QMat4 ToQType(const Mat4& e){
    QMat4 qmat;
    real_t * data = new real_t[16];
    for(int i = 0; i < 4; i++){//row
        for(int j = 0; j < 4; j++){//column
            data[4*i+j] = e(i,j);
        }
    }
    return QMat4(data);
}

static Mat4 ToEType(const QMat4& q){
    Mat4 mat;
    real_t * data = new real_t[16];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            data[4*i+j] = q(i,j);
        }
    }
    return Mat4(data);
}


#endif // COMMONTYPE_H
