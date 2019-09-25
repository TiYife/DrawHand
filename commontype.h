#ifndef COMMONTYPE_H
#define COMMONTYPE_H

#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <string>
#include <iostream>

#define Vec3 Eigen::Vector3d
#define Vec2 Eigen::Vector2d
#define Quat Eigen::Quaterniond
#define real_t double

struct Face
{
    unsigned v0, v1, v2;
};

struct Vertex
{
    Vec3 position;
    Vec3 normal;
    Vec2 texcoord;
};


#endif // COMMONTYPE_H
