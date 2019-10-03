#include <QMouseEvent>
#include "panel.h"

Panel::Panel(QWidget *parent) :
    QOpenGLWidget(parent),
//    texture_(0),
    angular_speed_(0),
    press(false),
    scale_(0.01),
    offset_x_(0),
    offset_y_(-1.5),
    offset_z_(-5),
    hand_mesh_(nullptr)
{
//    render = unique_ptr<Render>(new Render());
    this->grabKeyboard();
}

Panel::~Panel()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    doneCurrent();
//    delete texture_;
//    delete hand_mesh_;
    clearAuxiliaryMeshes();

}

//! [0]
void Panel::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mouse_position_ = QVec2(e->localPos());
    press = true;
}

void Panel::mouseReleaseEvent(QMouseEvent *e)
{
    press = false;

}

void Panel::mouseMoveEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVec2 diff = QVec2(e->localPos()) - mouse_position_;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVec3 n = QVec3(diff.y(), diff.x(), 0.0).normalized();

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

void Panel::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(128, 128, 128, 1);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    initMeshes();
}

void Panel::changeMeshVisible(int id)
{
    if(id == -1)
        hand_mesh_->SetVisible(1 - hand_mesh_->IsVisible());
    else if(id == -2)
        auxiliary_meshes_[id]->SetVisible(1 - auxiliary_meshes_[id]->IsVisible());
    else
        meshes_[id]->SetVisible(1 - meshes_[id]->IsVisible());
    update();
}

void Panel::reloadMeshes(QString path)
{
    vector<Vec3> vertex;
    vector<Vec3> normal;
    vector<Transform> list;
    FileUtil::LoadTransfroms(path.toStdString(), vertex, normal, list);

    hand_mesh_->Update(vertex, normal);
    for(int i = 0; i < 5 ; i++ ){
        meshes_[i]->Update(list[i]);
    }
}

void Panel::clearAuxiliaryMeshes()
{
    auxiliary_meshes_.clear();
}

void Panel::updateAuxiliaryMeshes()
{
    unique_ptr<Mesh> mesh;
    for(auto & indices : hand_key_indices){
        Vec3 pos = Vec3(0,0,0);
        for(auto& i: indices){
            //todo mesh_
            pos+=hand_mesh_->positions_[i];

        }
        pos/=indices.size();
        mesh = std::move(MeshBuilders::CreateSphere(pos, 15));
        mesh_map_[mesh.get()] = unique_ptr<RenderMesh>(new SimpleRenderMesh(mesh.get(), QColor(255,0,0)));
        auxiliary_meshes_.push_back(std::move(mesh));
    }

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



    // Calculate model view transformation
    QMat4 matrix;
    matrix.translate(offset_x_, offset_y_, offset_z_);
    matrix.rotate(rotation_);
    matrix.scale(scale_);
    for(auto& it = mesh_map_.begin(); it != mesh_map_.end();it++){
        it->second->draw(matrix, projection_);
    }

}

void Panel::initMeshes()
{

    setHandMesh(FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/hand.obj", "hand"));
    meshes_.push_back(FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/ball.obj", "ball"));
    meshes_.push_back(FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/cube.obj", "cube"));
    meshes_.push_back(FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/banana.obj", "banana"));
    meshes_.push_back(FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/torus.obj", "torus"));
    meshes_.push_back(FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/cube2.obj", "cube2"));

    for(auto & mesh :meshes_){
//        mesh->SetVisible(false);
        mesh_map_[mesh.get()] = unique_ptr<RenderMesh>(new SimpleRenderMesh(mesh.get(), QColor(0,255,0)));
    }

}


void Panel::addMesh(unique_ptr<Mesh> mesh){
    mesh_map_[mesh.get()] = unique_ptr<RenderMesh>(new SimpleRenderMesh(mesh.get(), QColor(255,0,0)));
    auxiliary_meshes_.push_back(std::move(mesh));
}

void Panel::setHandMesh(unique_ptr<Mesh> mesh)
{
    if(hand_mesh_!=nullptr){
        clearAuxiliaryMeshes();
    }

    mesh_map_[mesh.get()] = unique_ptr<RenderMesh>(new TextureRenderMesh(mesh.get(), QString(":/resource/images/handD.bmp")));
    hand_mesh_ = std::move(mesh);
    updateAuxiliaryMeshes();
}

void Panel::addKeyIndices(const std::vector<int> &indices)
{
    hand_key_indices.push_back(indices);
    updateAuxiliaryMeshes();
}
