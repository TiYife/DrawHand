#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "commontype.h"

class Transform
{
public:
    Transform(){};
    Transform(const Quat & q, const Vec3 & p){
        quat_ = q;
        pos_ = p;
    }

    Vec3 operator * (const Vec3 & vec3){
        Eigen::Isometry3d T = Eigen::Isometry3d::Identity();
        T.rotate(quat_.toRotationMatrix());
        T.pretranslate(pos_);
        return T * vec3;
    }

private:
    Quat quat_;
    Vec3 pos_;
};

#endif // TRANSFORM_H
