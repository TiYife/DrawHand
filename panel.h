#ifndef PANEL_H
#define PANEL_H

#include "renderMesh.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Panel : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();

    void setMesh(Mesh * mesh);
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
    QBasicTimer timer_;
    QOpenGLShaderProgram program_;
    std::vector<RenderMesh *> meshes_;

    QOpenGLTexture * texture_;

    QMatrix4x4 projection_;

    QVector2D mouse_position_;
    QVector3D rotation_axis_;
    QQuaternion rotation_;
    qreal angular_speed_;
    qreal scale_;
    qreal offset_x_, offset_y_, offset_z_;
    bool press;
};
#endif // PANEL_H
