#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include "panel.h"
#include "meshbuilders.h"

Panel::Panel(QWidget *parent) :
    QOpenGLWidget(parent),
    texture_(0),
    angular_speed_(0),
    press(false),
    scale_(0.05),
    offset_x_(0),
    offset_y_(0),
    offset_z_(-5.0),
    hand_mesh_(0)
{
    this->grabKeyboard();
}

Panel::~Panel()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    doneCurrent();
    delete texture_;
    delete hand_mesh_;

    for(auto it = meshes_.rbegin(); it!=meshes_.rend();it++)
        delete *it;

}

//! [0]
void Panel::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mouse_position_ = QVector2D(e->localPos());
    press = true;
}

void Panel::mouseReleaseEvent(QMouseEvent *e)
{
    press = false;

}

void Panel::mouseMoveEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mouse_position_;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() * 0.05;

    // Calculate new rotation axis as weighted sum
    angular_speed_ = acc;
    rotation_axis_ = (rotation_axis_ * angular_speed_ + n * acc).normalized();
    rotation_ = QQuaternion::fromAxisAndAngle(rotation_axis_, angular_speed_) * rotation_;

    update();
    // Increase angular speed
    //angular_speed_ += acc;
}

void Panel::wheelEvent(QWheelEvent *e)
{
    if(e->delta()>0)
        scale_*=1.1;
    else
        scale_/=1.1;
    update();
}

void Panel::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Left:
        offset_x_-=0.1;
        break;
    case Qt::Key_Right:
        offset_x_+=0.1;
        break;
    case Qt::Key_Up:
        offset_y_+=0.1;
        break;
    case Qt::Key_Down:
        offset_y_-=0.1;
        break;
    case Qt::Key_Plus:
        offset_z_+=0.1;
        break;
    case Qt::Key_Minus:
        offset_z_-=0.1;
        break;
    default:
        break;
    }
    update();
}

void Panel::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    //angular_speed_ *= 0.99;
//    angular_speed_ = 0;
//    // Stop rotation when speed goes below threshold
//    if (angular_speed_ < 0.01) {
//        angular_speed_ = 0.0;
//    } else {
//        // Update rotation
//        rotation_ = QQuaternion::fromAxisAndAngle(rotation_axis_, angular_speed_) * rotation_;

//        // Request an update
//        update();
//    }
}

void Panel::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(128, 128, 128, 1);

    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Use QBasicTimer because its faster than QTimer
    timer_.start(12, this);
}

void Panel::initShaders()
{
    // Compile vertex shader
    if (!program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resource/shaders/vshader.vert"))
        close();

    // Compile fragment shader
    if (!program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resource/shaders/fshader.frag"))
        close();

    // Link shader pipeline
    if (!program_.link())
        close();

    // Bind shader pipeline for use
    if (!program_.bind())
        close();
}

void Panel::initTextures()
{
    // Load cube.png image
    texture_ = new QOpenGLTexture(QImage(":/resource/images/handD.bmp").mirrored());

    // Set nearest filtering mode for texture minification
    texture_->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture_->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture_->setWrapMode(QOpenGLTexture::Repeat);
}

void Panel::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 10.0, fov = 45.0;

    // Reset projection
    projection_.setToIdentity();

    // Set perspective projection
    projection_.perspective(fov, aspect, zNear, zFar);
}

void Panel::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture_->bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(offset_x_, offset_y_, offset_z_);
    matrix.rotate(rotation_);
    matrix.scale(scale_);

    // Set modelview-projection matrix
    program_.setUniformValue("mvp_matrix", projection_ * matrix);

    // Use texture unit 0 which contains cube.png
    program_.setUniformValue("texture", 0);

    // Draw cube geometry
    for(auto& mesh_:meshes_)
        mesh_->draw(&program_);
    if(hand_mesh_)
        hand_mesh_->draw(&program_);
}


void Panel::addMesh(Mesh * mesh){
    meshes_.push_back(new RenderMesh(mesh));
}

void Panel::setHandMesh(Mesh *mesh)
{
    if(hand_mesh_!=nullptr)
        delete hand_mesh_;
    hand_mesh_ = new RenderMesh(mesh);
}

void Panel::addKeyIndices(const std::vector<int> &indices)
{
    hand_key_indices.push_back(indices);

    Vec3 pos = Vec3(0,0,0);
    for(auto& i: indices){
        //todo mesh_
        pos+=hand_mesh_->mesh_->positions_[i];

    }
    pos/=indices.size();

    meshes_.push_back(new RenderMesh(MeshBuilders::CreateSphere(pos, 1.0)));

}
