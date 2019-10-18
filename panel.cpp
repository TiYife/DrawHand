#include <QMouseEvent>
#include <QGuiApplication>
#include <iostream>
#include "panel.h"

Panel::Panel(QWidget *parent) :
    QOpenGLWidget(parent),
//    texture_(0),
    angular_speed_(0),
    press_(false),
    scale_(0.01),
    offset_x_(0),
    offset_y_(0),
    offset_z_(-1000),
    hand_mesh_(nullptr)
{
//    render = unique_ptr<Render>(new Render());
    this->grabKeyboard();
     makeCurrent();
     setFixedSize(640,480);
     resizeGL(640, 480);
     depth_image_ = cv::Mat(this->height(), this->width(), CV_32FC1, cv::Scalar(0));
     color_image_ = cv::Mat(this->height(), this->width(), CV_8UC3, cv::Scalar(0));
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
    press_ = true;
}

void Panel::mouseReleaseEvent(QMouseEvent *e)
{
    press_ = false;

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
        offset_z_+=10;
    else
        offset_z_-=10;
    update();
}

void Panel::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Left:
        offset_x_-=10;
        break;
    case Qt::Key_Right:
        offset_x_+=10;
        break;
    case Qt::Key_Up:
        offset_y_+=10;
        break;
    case Qt::Key_Down:
        offset_y_-=10;
        break;
    case Qt::Key_Plus:
        offset_z_+=10;
        break;
    case Qt::Key_Minus:
        offset_z_-=10;
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

//    glGenTextures(1, &texture_color_buffer);
//    glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer, 0);
}

void Panel::resizeGL(int w, int h)
{
    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
//    const qreal zNear = 1, zFar = 1000, fov = 45.0;

//    // Calculate aspect ratio
//    qreal aspect = qreal(w) / qreal(h ? h : 1);

//    // Reset projection
//    projection_.setToIdentity();

//    // Set perspective projection
//    projection_.perspective(fov, aspect, zNear, zFar);


        float fx = camera_param_.fx * w / 640;
        float fy = camera_param_.fy * h / 480;
        float cx = camera_param_.cx * w / 640;
        float cy = camera_param_.cy * h / 480;

        projection_.setToIdentity();

        projection_(0, 0) = 2 * fx / w;
        projection_(0, 2) = -(2 * cx - w) / w;

        projection_(1, 1) = 2 * fy / h;
        projection_(1, 2) = (2 * cy - h) / h;

        projection_(2, 2) = -(zfar_ + znear_) / (zfar_ - znear_);
        projection_(2, 3) = (-2 * zfar_ * znear_) /(zfar_ - znear_);

        projection_(3, 2) = -1.0f;
        projection_(3, 3) = 0.0f;

    std::cout<<ToEType(projection_).transpose()<<std::endl;

}

void Panel::paintGL()
{
    // Calculate model view transformation
    QMat4 matrix;
    matrix.translate(offset_x_, offset_y_, offset_z_);
    matrix.rotate(rotation_);
    std::cout<<ToEType(matrix)<<std::endl;
//    matrix.scale(scale_);

    std::cout<<ToEType(matrix)<<std::endl;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto& it = mesh_map_.begin(); it != mesh_map_.end();it++){
        it->second->draw(matrix, projection_, depth_);
    }
    glReadPixels(0,0,this->width(),this->height(),GL_BGR,GL_UNSIGNED_BYTE, color_image_.data);
    glReadPixels(0,0,this->width(),this->height(),GL_DEPTH_COMPONENT,GL_FLOAT, depth_image_.data);
}



void Panel::initMeshes()
{

    setHandMesh(FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/hand.obj", "hand"));

    auto ball = FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/ball.obj", "ball");
    mesh_map_[ball.get()] = unique_ptr<RenderMesh>(new TextureRenderMesh(ball.get(), QString(":/resource/images/ballD.bmp")));
    meshes_.push_back(std::move(ball));

    auto cube = FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/cube.obj", "cube");
    mesh_map_[cube.get()] = unique_ptr<RenderMesh>(new TextureRenderMesh(cube.get(), QString(":/resource/images/cubeD.bmp")));
    cube->SetVisible(false);
    meshes_.push_back(std::move(cube));

    auto banana = FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/banana.obj", "banana");
    mesh_map_[banana.get()] = unique_ptr<RenderMesh>(new TextureRenderMesh(banana.get(), QString(":/resource/images/bananaD.bmp")));
    banana->SetVisible(false);
    meshes_.push_back(std::move(banana));

    auto torus = FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/torus.obj", "torus");
    mesh_map_[torus.get()] = unique_ptr<RenderMesh>(new TextureRenderMesh(torus.get(), QString(":/resource/images/torusD.bmp")));
    torus->SetVisible(false);
    meshes_.push_back(std::move(torus));

    auto cube2 = FileUtil::LoadObj("D:/Documents/Projects/QT/DrawHand/resource/ori-objs/cube2.obj", "cube2");
    mesh_map_[cube2.get()] = unique_ptr<RenderMesh>(new TextureRenderMesh(cube2.get(), QString(":/resource/images/cube2D.bmp")));
    cube2->SetVisible(false);
    meshes_.push_back(std::move(cube2));
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
    updateAuxiliaryMeshes();
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
    hand_key_indices_.push_back(indices);
    initAuxiliaryMeshes();
}



void Panel::setMeshVisible(int id, bool show)
{
    if(id == -1)
        hand_mesh_->SetVisible(show);
    else if(id == -2){
        for(auto& aux: auxiliary_meshes_){
            aux->SetVisible(show);
        }
    }
    else
        meshes_[id]->SetVisible(show);
    update();
}

void Panel::showDepthMap(bool depth_mode)
{
//    for(auto& it = mesh_map_.begin(); it !=mesh_map_.end(); it++){
//        it->second->DepthMode(depth_mode);
//    }
    depth_ = depth_mode;
    repaint();
}



void Panel::saveColorImage(QString filename)
{
//    cv::Mat color_image = cv::Mat(this->height(), this->width(), CV_8UC3, cv::Scalar(0));
//    glReadPixels(0,0,this->width(),this->height(),GL_BGR,GL_UNSIGNED_BYTE, color_image.data);

    resizeGL(640, 480);

    cv::Mat color_image_flipped;
    cv::flip(color_image_, color_image_flipped, 0);
    cv::imshow("color", color_image_flipped);
//    cv::imwrite(filename.toStdString(), color_image_flipped);
}

void Panel::saveDepthImage(QString filename)
{
//    cv::Mat image = cv::Mat(this->height(), this->width(), CV_32FC1, cv::Scalar(0));
//    glReadPixels(0,0,this->width(),this->height(),GL_DEPTH_COMPONENT,GL_FLOAT, image.data);

    resizeGL(640, 480);
    std::vector<Vec3> vertex;
    QVec4 pos4;
    QVec3 pos3;
    QVec3 vet;
    float d01,d_1,z;

    QMat4 view;
    view.translate(offset_x_, offset_y_, offset_z_);
    view.rotate(rotation_);

    QMat4 K;
    K.setToIdentity();
    K(0,0)=camera_param_.fx;
    K(0,2)=camera_param_.cx;
    K(1,1)=camera_param_.fy;
    K(1,2)=camera_param_.cy;
    K(2,2)=1;


    cv::Mat depth_image_flipped;
    cv::flip(depth_image_, depth_image_flipped, 0);
    cv::Mat color_image_flipped;
    cv::flip(color_image_, color_image_flipped, 0);

    std::cout<<"data: ";
    for(int i = 0;i<depth_image_flipped.rows;i++){
        float * color_data = color_image_flipped.ptr<float>(i);
        float * depth_data = depth_image_flipped.ptr<float>(i);
        for(int j = 0; j < depth_image_flipped.cols; j++){
            if(depth_data[j]!=1.0f){
//                depth_data[j] = (zfar_*znear_)/(znear_*depth_data[j]-zfar_*depth_data[j]+zfar_);
//                depth_data[j] = (zfar_-znear_)*depth_data[j]+znear_;
                d01 = depth_data[j];
                z = (zfar_*znear_)/(znear_*d01-zfar_*d01+zfar_);
                d_1 = (zfar_*z+znear_*+2*zfar_*znear_)/(zfar_*z-znear_*z);

                pos3 = K.inverted()*QVec3(j,i,1)*z;
                pos4 = QVec4(pos3,1);//view.inverted() *

//                pos4 = (projection_*view).inverted() * QVec4(i,j,d_1, -z);

                vet = QVec3(pos4.x()/pos4.w(), pos4.y()/pos4.w(), pos4.z()/pos4.w());
                vertex.push_back(ToEType(vet));
                std::cout<<z<<" ";
            }
        }
        std::cout<<std::endl;


    }
    FileUtil::WriteKeyPos("key.txt",vertex);
    std::vector<Vec3> ori_vertices = hand_mesh_->positions_;
    std::vector<Vec3> new_vertices;
    for(auto & p : ori_vertices){
        QVec4 pos = view * QVec4(p.x(),p.y(),p.z(),1);
        p = Vec3(pos.x()/pos.w(), -pos.y()/pos.w(), -pos.z()/pos.w());
        new_vertices.push_back(p);
    }
    FileUtil::WriteKeyPos("ori_in_camera.txt", new_vertices);
//    std::cout<<image_flipped.rows<<" "<<image_flipped.cols<<std::endl;
//    std::cout<<this->width()<<" "<<this->height()<<std::endl;

//     cv::imwrite(filename.toStdString(), image_flipped);
    cv::imshow("depth", depth_image_flipped);
}

void Panel::saveKeyPos(QString filename)
{
    FileUtil::WriteKeyPos(filename, hand_key_pos_);
}



void Panel::clearAuxiliaryMeshes()
{
    auxiliary_meshes_.clear();
}

void Panel::initAuxiliaryMeshes()
{
    unique_ptr<Mesh> mesh;
    size_t size = hand_key_indices_.size();
    hand_key_pos_.resize(size, Vec3(0,0,0));

    for(size_t i = 0; i < size; i++){
        for(auto& j: hand_key_indices_[i]){
            //todo mesh_
            hand_key_pos_[i] += hand_mesh_->positions_[j];

        }
        hand_key_pos_[i] /= hand_key_indices_[i].size();
        mesh = std::move(MeshBuilders::CreateSphere(hand_key_pos_[i], 10));
        mesh_map_[mesh.get()] = unique_ptr<RenderMesh>(new SimpleRenderMesh(mesh.get(), QColor(255,0,0)));
        auxiliary_meshes_.push_back(std::move(mesh));
    }
}

void Panel::updateAuxiliaryMeshes()
{
    size_t size = hand_key_indices_.size();
    hand_key_pos_.resize(size, Vec3(0,0,0));

    for(size_t i = 0; i < size; i++){
        for(auto& j: hand_key_indices_[i]){
            hand_key_pos_[i] += hand_mesh_->positions_[j];
        }

        hand_key_pos_[i] /= hand_key_indices_[i].size();
        Transform t;
        t.setTranslate(hand_key_pos_[i]);
        auxiliary_meshes_[i]->SetTransform(t);
        mesh_map_[auxiliary_meshes_[i].get()] ->update();
    }

}

