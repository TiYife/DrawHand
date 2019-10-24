#ifndef PANEL_H
#define PANEL_H

#include <QOpenGLWidget>
#include <QQuaternion>
#include <QBasicTimer>
#include <QOpenGLFramebufferObject>
#include <opencv2/opencv.hpp>
#include "rendermesh.h"
#include "meshbuilders.h"
#include "fileutil.h"

struct CameraParam{
    float fx = 473.297f;
    float fy = fx;
    float cx = 316.561f;
    float cy = 245.293f;
};


class Panel : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();

    void addMesh(unique_ptr<Mesh> mesh);
    void setHandMesh(unique_ptr<Mesh> mesh);
    void addKeyIndices(const std::vector<int> & indices);

    void setMeshVisible(int id, bool show);
    void reloadMeshes(QString path);

    void showDepthMap(bool depth_mode);

    void saveColorImage(QString filename);
    void saveDepthImage(QString filename);

    void saveKeyPos(QString filename);
    void saveNewKeyPos(QString filename, std::vector<QVec3> points);
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initMeshes();

private:
    void clearAuxiliaryMeshes();
    void initAuxiliaryMeshes();
    void updateAuxiliaryMeshes();



private:
    qreal znear_ = 1, zfar_ = 10000, fov_ = 45.0;
    CameraParam camera_param_;
    QMat4 projection_;

    QVec2 mouse_position_;
    QVec3 rotation_axis_;
    QQuaternion rotation_;
    qreal angular_speed_;
    qreal scale_;
    qreal offset_x_, offset_y_, offset_z_;

    bool press_{0};
    bool depth_{0};


    unique_ptr<Mesh> hand_mesh_;
    std::vector<unique_ptr<Mesh>> meshes_;
    unordered_map<Mesh*, unique_ptr<RenderMesh>> mesh_map_;

    std::vector<std::vector<int>> hand_key_indices_;
    std::vector<Vec3> hand_key_pos_;
    std::vector<unique_ptr<Mesh>> auxiliary_meshes_;

    GLuint colorbuffer, depthbuffer;

    cv::Mat color_image_, depth_image_;
};
#endif // PANEL_H
