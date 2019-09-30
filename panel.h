#ifndef PANEL_H
#define PANEL_H

#include <QOpenGLWidget>
#include <QQuaternion>
#include <QBasicTimer>
#include "render.h"

class Panel : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();

    void addMesh(Mesh * mesh);
    void setHandMesh(Mesh * mesh);
    void addKeyIndices(const std::vector<int> & indices);
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initTextures();

private:
    void clearAuxiliaryMeshes();
    void updateAuxiliaryMeshes();



private:
//    QBasicTimer timer_;
//    QOpenGLShaderProgram hand_program_;
//    QOpenGLShaderProgram default_program_;

//    QOpenGLTexture * texture_;

    QMatrix4x4 projection_;

    QVector2D mouse_position_;
    QVector3D rotation_axis_;
    QQuaternion rotation_;
    qreal angular_speed_;
    qreal scale_;
    qreal offset_x_, offset_y_, offset_z_;
    bool press;

    unique_ptr<Render> render;
    unique_ptr<Mesh> hand_mesh_;
    std::vector<unique_ptr<Mesh>> auxiliary_meshes_;
    std::vector<std::vector<int>> hand_key_indices;

};
#endif // PANEL_H
