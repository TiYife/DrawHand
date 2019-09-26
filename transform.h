#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "commontype.h"

class Transform
{
public:
    Transform(){
        quatate_ = Eigen::Quaterniond (1,0,0,0);
        translate_ = Vec3(0,0,0);
    };

    Transform(const Quat & q, const Vec3 & p){
        quatate_ = q;
        translate_ = p;
    }

    void setTranslate(const Vec3 & pos ){
        translate_ = pos;
    }

    Vec3 operator * (const Vec3 & vec3){
        Eigen::Isometry3d T = Eigen::Isometry3d::Identity();
        T.rotate(quatate_.toRotationMatrix());
        T.pretranslate(translate_);
        return T * vec3;
    }

private:
    Quat quatate_;
    Vec3 translate_;
};

#endif // TRANSFORM_H
