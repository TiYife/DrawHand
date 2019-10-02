#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "commontype.h"

class Transform
{
public:
    Transform(){
        quatate_ = Quat (1,0,0,0);
        translate_ = Vec3(0,0,0);
    }

    Transform(const Quat & q, const Vec3 & p){
        quatate_ = q;
        translate_ = p;
    }

    void setTranslate(const Vec3 & pos ){
        translate_ = pos;
    }

    Vec3 operator * (const Vec3 & vec3){
        Eigen::Isometry3f T = Eigen::Isometry3f::Identity();
        T.rotate(quatate_.toRotationMatrix());
        T.pretranslate(translate_);
        return T * vec3;
    }

    Mat4 ToMat4(){
        Eigen::Isometry3f T = Eigen::Isometry3f::Identity();
        T.rotate(quatate_.toRotationMatrix());
        T.pretranslate(translate_);

        Mat4 mat =T.matrix();
        return mat;
    }


    QMat4 toQMat4(){
        return ToQType(ToMat4());
    }

private:
    Quat quatate_;
    Vec3 translate_;
};

#endif // TRANSFORM_H
