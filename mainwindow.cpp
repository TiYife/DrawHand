
#include <QtWidgets>
#include <string>
#include "mainwindow.h"
#include <windows.h>

MainWindow::MainWindow()
{
    init();
    setCurrentFile(QString());
}

MainWindow::MainWindow(const QString &fileName)
{
    init();
    loadFile(fileName);
}

MainWindow::~MainWindow()
{
    delete panel;
    delete recentFileSeparator;
    delete recentFileActs;
    //delete recentFileSubMenuAct;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::newFile()
{
    //    MainWindow *other = new MainWindow;
    //    other->tile(this);
    //    other->show();
}

void MainWindow::open()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        panel -> reloadMeshes(fileName);

    QFileInfo info = QFileInfo(fileName);
    filename_ = info.baseName();
}

void MainWindow::batch()
{
    const QStringList filelist = QFileDialog::getOpenFileNames(this,tr("文件选择"),tr("/home"),
                                                               tr("文本文件(* txt)"));

    if(filelist.empty())return;
    QString savepath = QFileInfo(filelist[0]).absoluteDir().absolutePath() + "-data/";

    //    QString savepath = kBasePath + QString::fromStdString(FileUtil::GetTime()) + "/";
    QDir dir(savepath);
    if(!dir.exists())
        dir.mkdir(savepath);

    for(int i = 3; i < 4; i++){
        panel->changeRotation(i);
        QString path = savepath;
        switch (i) {
        case 0:
            path += "front/";
            break;
        case 1:
            path += "back/";
            break;
        case 2:
            path += "left/";
            break;
        case 3:
            path += "right/";
            break;
        case 4:
            path += "top/";
            break;
        case 5:
            path += "bottom/";
            break;
        default:
            break;
        }

        QDir dir(path);
        if(!dir.exists())
            dir.mkdir(path);

        panel->changeRotation(i);

        if (!filelist.isEmpty()){
            for(auto& fileName : filelist){
                if (!fileName.isEmpty())
                    panel -> reloadMeshes(fileName);
                QFileInfo info = QFileInfo(fileName);
                //            filename_ = info.baseName();
                filename_ = QString("%1").arg(info.baseName().toInt(), 4, 10, QLatin1Char('0'));

                saveFile(path);
            }
        }
    }
}

bool MainWindow::save()
{
    filename_ = QString::fromStdString(to_string(temp_no_++));
    QString savepath = kBasePath + "temp/";
    QDir dir(savepath);
    if(!dir.exists())
        dir.mkdir(savepath);
    saveFile(savepath);
    return true;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}


bool MainWindow::saveFile(const QString &path)
{

    panel->showMaskImage(0);
    panel->saveColorImage(path + "color_" + filename_ + ".png");
    panel->saveDepthImage(path + "depth_" + filename_ + ".png");

    panel->showMaskImage(true);
    panel->saveColorImage(path + "mask_" + filename_ + ".png");

    panel->showMaskImage(0);

    panel->saveKeyPos(path + "key_points_" + filename_ +".txt");


    return true;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About SDI"),
                       tr("The <b>SDI</b> example demonstrates how to write single "
                          "document interface applications using Qt."));
}

void MainWindow::documentWasModified()
{
    setWindowModified(true);
}

void MainWindow::addAuxiliaryShapes()
{
    //    int list[] = {1374, 1375, 1376, 1377, 1378, 1379, 1380, 1381, 1382, 1383, 1384, 1385, 1386, 1387, 1388, 1389, 1390, 1391, 1392, 1393, 1394, 1395, 1396, 1397, 1398, 1399, 1400, 1401, 1402, 1403, 1404, 1405, 1406, 1407, 1408, 1409, 1410, 1411, 1412, 1413, 1414, 1415, 1416, 1417, 1418, 1419, 1420, 1421, 1422, 1423, 1424, 1425, 1426, 1427, 1428, 1429, 1430, 1431, 1432, 1433, 1434, 1435, 1436, 1437, 1438, 1439, 1440, 1441, 1442, 1443, 1444, 1445, 1446, 1447, 1448, 1449, 1450, 1451, 1452, 1453, 1454, 2418, 2419, 2420, 2423, 2424, 2425, 2426, 2427, 2428, 2429, 2433, 2434, 2435, 2437, 2438, 2439, 2440, 2441, 2481, 2482, 2483, 2486, 2487, 2488, 2489, 2490, 2491, 2492, 2493, 2494, 2495, 2496, 2497, 2500, 2501, 2504, 2505, 2506, 2507, 2508, 2509, 2555, 2556, 2557, 2562, 2563, 2564, 2565, 2566, 2567, 2568, 2569, 2570, 2571, 2572, 2573, 2580, 2581, 2582, 2585, 2586, 2587, 2588, 2589, 2590, 2591, 2592, 2593, 2629, 2630, 2631, 2633, 2634, 2635, 2636, 2637, 2641, 2642, 2643, 2646, 2647, 2648, 2649, 2650, 2651, 2652, 2653, 2654, 2655, 2656, 2657, 3934, 3935, 3936, 3937, 3938, 3939, 3940, 3941, 3942, 3943, 3944, 3945, 3946, 3947, 3948, 3949, 3950, 3951, 3952, 3953, 3954, 3955, 3956, 3962, 3964, 3990, 3991, 3993, 3994, 3995, 3996, 3997, 3998, 3999, 4001, 4002, 4003, 4004, 4005, 4006, 4007, 4008, 4009, 4010, 4011, 4012};
    //    std::vector<int> indices(list, list + 218);

    const QString filename = QFileDialog::getOpenFileName(this);
    if (filename.isEmpty())
        return;
    std::vector<std::vector<int>> key_indices = FileUtil::LoadKeyIndices(filename);

    for(auto& indices:key_indices)
        panel ->addKeyIndices(indices);
}

void MainWindow::findKeyIndices()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;

    string s = "D:/Documents/Projects/QT/DrawHand/resource/ori-objs/hand.obj";
    string s2 = fileName.toStdString();
    unique_ptr<Mesh> ori = FileUtil::LoadObj(s, "");
    std::cout<<s2<<std::endl;
    unique_ptr<Mesh> mesh = FileUtil::LoadObj(s2, "");

    std::vector<int> indices = ori->FindLostIndices(mesh.get());
    for(auto & i: indices){
        std::cout<<i<<" ";
    }
    std::cout<<std::endl;

}

void MainWindow::drawBall()
{
    //    panel->addMesh(MeshBuilders::CreateSphere(Vec3(0,0,0), 0.1));
    //    const QString filename = QFileDialog::getOpenFileName(this);
    //    QFileInfo info(filename);
    //    std::cout<<info.absoluteFilePath().toStdString()<<std::endl;

    //    cv::Mat image = cv::imread(info.absoluteFilePath().toStdString(), cv::IMREAD_UNCHANGED);
    //    cv::Mat change = cv::Mat(panel->height(), panel->width(), CV_32FC1, image.data);

    //    cv::imshow("read_8uc3", image);
    //    cv::imshow("read_32fc1", change);

//    const QStringList filelist = QFileDialog::getOpenFileNames(this,tr("文件选择"),tr("/home"),
//                                                               tr("文本文件(* txt)"));

//    QString savepath;
//    QString saveDir;
//    QString filepath = filelist[0];
//    QFileInfo dirInfo(filepath);
//    saveDir = dirInfo.absoluteDir().absolutePath();

//    for(auto& path: filelist){
//        QFileInfo info(path);

//        savepath = saveDir + "/new/" + info.baseName() + ".txt";
//        std::cout<<savepath.toStdString()<<std::endl;
//        std::vector<QVec3> points = FileUtil::LoadKeyPoints(path);
//        panel->saveNewKeyPos(savepath, points);

//    }

    int begin = 0;
    int end = 0;
    int base = 0;

    QDialog dialog(this);
    QFormLayout form(&dialog);

    panel->releaseKeyboard();
    // Value0
    QString label = QString("base: ");
    QLineEdit *box = new QLineEdit(&dialog);
    form.addRow(label, box);
    // Value1
    QString label1 = QString("begin: ");
    QLineEdit *box1 = new QLineEdit(&dialog);
    form.addRow(label1, box1);
    // Value2
    QString label2 = QString("end: ");
    QLineEdit *box2 = new QLineEdit(&dialog);
    form.addRow(label2, box2);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        base = box->text().toInt();
        begin = box1->text().toInt() + base;
        end = box2->text().toInt() + base;
    }

    const QString dirName = QFileDialog::getExistingDirectory(this);
    if (!dirName.isEmpty()){
        for(int i = 0; i < 6; i++){
            QString path = dirName;

            switch (i) {
            case 0:
                path += "/front/";
                break;
            case 1:
                path += "/back/";
                break;
            case 2:
                path += "/left/";
                break;
            case 3:
                path += "/right/";
                break;
            case 4:
                path += "/top/";
                break;
            case 5:
                path += "/bottom/";
                break;
            default:
                break;
            }
            QString filename;
            for(int id = begin; id <= end; id++){
                filename = path + "color_" + QString("%1").arg(id, 4, 10, QLatin1Char('0')) + ".png";
                QFile file(filename);
                if(file.exists())
                    file.remove();
                std::cout<<"deleting file : " + filename.toStdString()<<std::endl;

                filename = path + "depth_" + QString("%1").arg(id, 4, 10, QLatin1Char('0')) + ".png";
                QFile file1(filename);
                if(file1.exists())
                    file1.remove();
                std::cout<<"deleting file : " + filename.toStdString()<<std::endl;

                filename = path + "mask_" + QString("%1").arg(id, 4, 10, QLatin1Char('0')) + ".png";
                QFile file2(filename);
                if(file2.exists())
                    file2.remove();
                std::cout<<"deleting file : " + filename.toStdString()<<std::endl;

                filename = path + "key_points_" + QString("%1").arg(id, 4, 10, QLatin1Char('0')) + ".txt";
                QFile file3(filename);
                if(file3.exists())
                    file3.remove();
                std::cout<<"deleting file : " + filename.toStdString()<<std::endl;
            }
        }
    }
    panel->grabKeyboard();
}

void MainWindow::showMaskImage(bool checked)
{
    panel->showMaskImage(checked);
}

void MainWindow::showHand(bool checked)
{
    panel -> setMeshVisible(-1, checked);
}

void MainWindow::showBall(bool checked)
{
    panel -> setMeshVisible(0, checked);
}

void MainWindow::showCube(bool checked)
{
    panel -> setMeshVisible(1, checked);
}

void MainWindow::showBanana(bool checked)
{
    panel -> setMeshVisible(2, checked);
}

void MainWindow::showCube2(bool checked)
{
    panel -> setMeshVisible(4, checked);
}

void MainWindow::showTorus(bool checked)
{
    panel -> setMeshVisible(3, checked);
}

void MainWindow::showAuxiliary(bool checked)
{
    panel -> setMeshVisible(-2, checked);
}

void MainWindow::init()
{
    setAttribute(Qt::WA_DeleteOnClose);

    isUntitled = true;

    //    textEdit = new QTextEdit;
    //    setCentralWidget(textEdit);

    panel = new Panel();
    setCentralWidget(panel);

    createActions();
    createStatusBar();

    readSettings();

    //    connect(textEdit->document(), &QTextDocument::contentsChanged,
    //            this, &MainWindow::documentWasModified);

    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::tile(const QMainWindow *previous)
{
    if (!previous)
        return;
    int topFrameWidth = previous->geometry().top() - previous->pos().y();
    if (!topFrameWidth)
        topFrameWidth = 40;
    const QPoint pos = previous->pos() + 2 * QPoint(topFrameWidth, topFrameWidth);
    if (QApplication::desktop()->availableGeometry(this).contains(rect().bottomRight() + pos))
        move(pos);
}

//! [implicit tr context]
void MainWindow::createActions()
{

    // file munu
    {
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
        QToolBar *fileToolBar = addToolBar(tr("File"));

        const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/resource/images/new.png"));
        QAction *openAct = new QAction(openIcon, tr("&open"), this);
        openAct->setStatusTip(tr("Open a new file"));
        connect(openAct, &QAction::triggered, this, &MainWindow::open);
        fileMenu->addAction(openAct);
        fileToolBar->addAction(openAct);

        const QIcon batchIcon = QIcon::fromTheme("document-batch", QIcon(":/resource/images/open.png"));
        QAction *batchAct = new QAction(batchIcon, tr("&batch..."), this);
        batchAct->setStatusTip(tr("batch process a list of existing files"));
        connect(batchAct, &QAction::triggered, this, &MainWindow::batch);
        fileMenu->addAction(batchAct);
        fileToolBar->addAction(batchAct);

        const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/resource/images/save.png"));
        QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
        saveAct->setShortcuts(QKeySequence::Save);
        saveAct->setStatusTip(tr("Save the document to disk"));
        connect(saveAct, &QAction::triggered, this, &MainWindow::save);
        fileMenu->addAction(saveAct);
        fileToolBar->addAction(saveAct);

        const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
        QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
        saveAsAct->setShortcuts(QKeySequence::SaveAs);
        saveAsAct->setStatusTip(tr("Save the document under a new name"));

        fileMenu->addSeparator();

        QAction *closeAct = fileMenu->addAction(tr("&Close"), this, &QWidget::close);
        closeAct->setShortcut(tr("Ctrl+W"));
        closeAct->setStatusTip(tr("Close this window"));

        const QIcon exitIcon = QIcon::fromTheme("application-exit");
        QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), qApp, &QApplication::closeAllWindows);
        exitAct->setShortcuts(QKeySequence::Quit);
        exitAct->setStatusTip(tr("Exit the application"));
    }

    // show munu
    {
        QMenu *showMenu = menuBar()->addMenu(tr("&Show"));
        QToolBar *showToolBar = addToolBar(tr("Show"));

        const QIcon maskIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *maskAct = new QAction(maskIcon, tr("mask"), this);
        maskAct->setShortcuts(QKeySequence::Cut);
        maskAct->setStatusTip(tr("show mask"));
        maskAct->setCheckable(true);
        maskAct->setChecked(false);
        connect(maskAct, &QAction::triggered, this, &MainWindow::showMaskImage);
        showMenu->addAction(maskAct);
        showToolBar->addAction(maskAct);


        const QIcon showhandIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showhandAct = new QAction(showhandIcon, tr("show hand"), this);
        showhandAct->setShortcuts(QKeySequence::Cut);
        showhandAct->setStatusTip(tr("show hand"));
        showhandAct->setCheckable(true);
        showhandAct->setChecked(true);
        connect(showhandAct, &QAction::triggered, this, &MainWindow::showHand);
        showMenu->addAction(showhandAct);
        showToolBar->addAction(showhandAct);

        const QIcon showballIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showballAct = new QAction(showballIcon, tr("show ball"), this);
        showballAct->setShortcuts(QKeySequence::Cut);
        showballAct->setStatusTip(tr("show ball"));
        showballAct->setCheckable(true);
        showballAct->setChecked(true);
        connect(showballAct, &QAction::triggered, this, &MainWindow::showBall);
        showMenu->addAction(showballAct);
        showToolBar->addAction(showballAct);

        const QIcon showcubeIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showcubeAct = new QAction(showcubeIcon, tr("show cube"), this);
        showcubeAct->setShortcuts(QKeySequence::Cut);
        showcubeAct->setStatusTip(tr("show cube"));
        showcubeAct->setCheckable(true);
        showcubeAct->setChecked(false);
        connect(showcubeAct, &QAction::triggered, this, &MainWindow::showCube);
        showMenu->addAction(showcubeAct);
        showToolBar->addAction(showcubeAct);

        const QIcon showbananaIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showbananaAct = new QAction(showbananaIcon, tr("show banana"), this);
        showbananaAct->setShortcuts(QKeySequence::Cut);
        showbananaAct->setStatusTip(tr("show banana"));
        showbananaAct->setCheckable(true);
        showbananaAct->setChecked(false);
        connect(showbananaAct, &QAction::triggered, this, &MainWindow::showBanana);
        showMenu->addAction(showbananaAct);
        showToolBar->addAction(showbananaAct);

        const QIcon showtorusIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showtorusAct = new QAction(showtorusIcon, tr("show torus"), this);
        showtorusAct->setShortcuts(QKeySequence::Cut);
        showtorusAct->setStatusTip(tr("show torus"));
        showtorusAct->setCheckable(true);
        showtorusAct->setChecked(false);
        connect(showtorusAct, &QAction::triggered, this, &MainWindow::showTorus);
        showMenu->addAction(showtorusAct);
        showToolBar->addAction(showtorusAct);

        const QIcon showcube2Icon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showcube2Act = new QAction(showcube2Icon, tr("show cube2"), this);
        showcube2Act->setShortcuts(QKeySequence::Cut);
        showcube2Act->setStatusTip(tr("show cube2"));
        showcube2Act->setCheckable(true);
        showcube2Act->setChecked(false);
        connect(showcube2Act, &QAction::triggered, this, &MainWindow::showCube2);
        showMenu->addAction(showcube2Act);
        showToolBar->addAction(showcube2Act);


        const QIcon showauxIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showauxAct = new QAction(showauxIcon, tr("show aux"), this);
        showauxAct->setShortcuts(QKeySequence::Cut);
        showauxAct->setStatusTip(tr("show aux"));
        showauxAct->setCheckable(true);
        showauxAct->setChecked(true);
        connect(showauxAct, &QAction::triggered, this, &MainWindow::showAuxiliary);
        showMenu->addAction(showauxAct);
        showToolBar->addAction(showauxAct);
    }

    // view munu
    {
        QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
        QToolBar *viewToolBar = addToolBar(tr("View"));

        const QIcon frontIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *frontAct = new QAction(frontIcon, tr("front"), this);
        frontAct->setStatusTip(tr("view form front"));
        connect(frontAct, &QAction::triggered, [this](){this->panel->changeRotation(0);});
        viewMenu->addAction(frontAct);
        viewToolBar->addAction(frontAct);

        const QIcon backIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *backAct = new QAction(backIcon, tr("back"), this);
        backAct->setStatusTip(tr("view form back"));
        connect(backAct, &QAction::triggered, [this](){this->panel->changeRotation(1);});
        viewMenu->addAction(backAct);
        viewToolBar->addAction(backAct);

        const QIcon leftIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *leftAct = new QAction(leftIcon, tr("left"), this);
        leftAct->setStatusTip(tr("view form left"));
        connect(leftAct, &QAction::triggered, [this](){this->panel->changeRotation(2);});
        viewMenu->addAction(leftAct);
        viewToolBar->addAction(leftAct);

        const QIcon rightIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *rightAct = new QAction(rightIcon, tr("right"), this);
        rightAct->setStatusTip(tr("view form right"));
        connect(rightAct, &QAction::triggered, [this](){this->panel->changeRotation(3);});
        viewMenu->addAction(rightAct);
        viewToolBar->addAction(rightAct);

        const QIcon topIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *topAct = new QAction(topIcon, tr("top"), this);
        topAct->setStatusTip(tr("view form top"));
        connect(topAct, &QAction::triggered, [this](){this->panel->changeRotation(4);});
        viewMenu->addAction(topAct);
        viewToolBar->addAction(topAct);

        const QIcon bottomIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *bottomAct = new QAction(bottomIcon, tr("bottom"), this);
        bottomAct->setStatusTip(tr("view form bottom"));
        connect(bottomAct, &QAction::triggered, [this](){this->panel->changeRotation(5);});
        viewMenu->addAction(bottomAct);
        viewToolBar->addAction(bottomAct);
    }

    // tool munu
    {
        QMenu *toolMenu = menuBar()->addMenu(tr("&Tool"));
        QToolBar *toolToolBar = addToolBar(tr("Tool"));

        const QIcon auxIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *auxAct = new QAction(auxIcon, tr("aux"), this);
        auxAct->setShortcuts(QKeySequence::Cut);
        auxAct->setStatusTip(tr("add Auxiliary shapes"));
        connect(auxAct, &QAction::triggered, this, &MainWindow::addAuxiliaryShapes);
        toolMenu->addAction(auxAct);
        toolToolBar->addAction(auxAct);

        const QIcon ballIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *drawAct = new QAction(ballIcon, tr("Cu&t"), this);
        drawAct->setShortcuts(QKeySequence::Cut);
        drawAct->setStatusTip(tr("draw ball"));
        connect(drawAct, &QAction::triggered, this, &MainWindow::drawBall);
        toolMenu->addAction(drawAct);
        toolToolBar->addAction(drawAct);

        const QIcon findIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *findAct = new QAction(findIcon, tr("Cu&t"), this);
        findAct->setShortcuts(QKeySequence::Cut);
        findAct->setStatusTip(tr("get joint indices"));
        connect(findAct, &QAction::triggered, this, &MainWindow::findKeyIndices);
        toolMenu->addAction(findAct);
        toolToolBar->addAction(findAct);
    }
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::loadFile(const QString &fileName)
{
    QString s = fileName;
    unique_ptr<Mesh> mesh = FileUtil::LoadObj(s.toStdString(), "");

    panel->setHandMesh(std::move(mesh));

}

void MainWindow::setRecentFilesVisible(bool visible)
{
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }

static QStringList readRecentFiles(QSettings &settings)
{
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}

static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

bool MainWindow::hasRecentFiles()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}

void MainWindow::prependToRecentFiles(const QString &fileName)
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentFilesVisible(!recentFiles.isEmpty());
}



void MainWindow::setCurrentFile(const QString &fileName)
{
    //    static int sequenceNumber = 1;

    //    isUntitled = fileName.isEmpty();
    //    if (isUntitled) {
    //        curFile = tr("document%1.txt").arg(sequenceNumber++);
    //    } else {
    //        curFile = QFileInfo(fileName).canonicalFilePath();
    //    }

    //    textEdit->document()->setModified(false);
    //    setWindowModified(false);

    //    if (!isUntitled && windowFilePath() != curFile)
    //        MainWindow::prependToRecentFiles(curFile);

    //    setWindowFilePath(curFile);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

MainWindow *MainWindow::findMainWindow(const QString &fileName) const
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin && mainWin->curFile == canonicalFilePath)
            return mainWin;
    }

    return 0;
}
