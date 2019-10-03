#ifndef PANEL_H
#define PANEL_H

#include <QOpenGLWidget>
#include <QQuaternion>
#include <QBasicTimer>
//#include "render.h"
#include <unordered_map>
#include "rendermesh.h"
#include "meshbuilders.h"


class Panel : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();

    void addMesh(unique_ptr<Mesh> mesh);
    void setHandMesh(unique_ptr<Mesh> mesh);
    void addKeyIndices(const std::vector<int> & indices);

    void changeMeshVisible(int id);
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void clearAuxiliaryMeshes();
    void updateAuxiliaryMeshes();



private:
//    QBasicTimer timer_;
//    QOpenGLShaderProgram hand_program_;
//    QOpenGLShaderProgram default_program_;

//    QOpenGLTexture * texture_;

    QMat4 projection_;

    QVec2 mouse_position_;
    QVec3 rotation_axis_;
    QQuaternion rotation_;
    qreal angular_speed_;
    qreal scale_;
    qreal offset_x_, offset_y_, offset_z_;
    bool press;

//    unique_ptr<Render> render;
    unique_ptr<Mesh> hand_mesh_;
    std::vector<unique_ptr<Mesh>> auxiliary_meshes_;
    std::vector<unique_ptr<Mesh>> meshes_;
    std::vector<std::vector<int>> hand_key_indices;

    unordered_map<Mesh*, unique_ptr<RenderMesh>> mesh_map_;

};
#endif // PANEL_H
