
#include <QtWidgets>
#include <string>
#include "mainwindow.h"

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
    if (!filelist.isEmpty()){
        for(auto& fileName : filelist){
            if (!fileName.isEmpty())
                panel -> reloadMeshes(fileName);

            QFileInfo info = QFileInfo(fileName);
            filename_ = info.baseName();
            auto a = info.absoluteFilePath();
            qDebug()<<a;
            save();
        }
    }
}

bool MainWindow::save()
{
    panel->showDepthMap(0);
    panel->grab().save("pics/color_" + filename_ + ".png");
    panel->showDepthMap(1);
    panel->grab().scaled(640, 480).save("pics/depth_" + filename_ + ".png");
    panel->showDepthMap(0);

    panel->saveKeyPos("pics/key_points_" + filename_ +".txt");
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
        std::cout<<i<<std::endl;
    }
}

void MainWindow::drawBall()
{
    panel->addMesh(MeshBuilders::CreateSphere(Vec3(0,0,0), 0.1));
}

void MainWindow::showDepthMap(bool checked)
{
    panel->showDepthMap(checked);
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
    panel -> setMeshVisible(3, checked);
}

void MainWindow::showTorus(bool checked)
{
    panel -> setMeshVisible(4, checked);
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





// view munu
    {
        QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
        QToolBar *viewToolBar = addToolBar(tr("View"));

        const QIcon depthIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *depthAct = new QAction(depthIcon, tr("depth"), this);
        depthAct->setShortcuts(QKeySequence::Cut);
        depthAct->setStatusTip(tr("show depth"));
        depthAct->setCheckable(true);
        depthAct->setChecked(false);
        connect(depthAct, &QAction::triggered, this, &MainWindow::showDepthMap);
        viewMenu->addAction(depthAct);
        viewToolBar->addAction(depthAct);


        const QIcon showhandIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showhandAct = new QAction(showhandIcon, tr("show hand"), this);
        showhandAct->setShortcuts(QKeySequence::Cut);
        showhandAct->setStatusTip(tr("show hand"));
        showhandAct->setCheckable(true);
        showhandAct->setChecked(true);
        connect(showhandAct, &QAction::triggered, this, &MainWindow::showHand);
        viewMenu->addAction(showhandAct);
        viewToolBar->addAction(showhandAct);

        const QIcon showballIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showballAct = new QAction(showballIcon, tr("show ball"), this);
        showballAct->setShortcuts(QKeySequence::Cut);
        showballAct->setStatusTip(tr("show ball"));
        showballAct->setCheckable(true);
        showballAct->setChecked(true);
        connect(showballAct, &QAction::triggered, this, &MainWindow::showBall);
        viewMenu->addAction(showballAct);
        viewToolBar->addAction(showballAct);

        const QIcon showcubeIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showcubeAct = new QAction(showcubeIcon, tr("show cube"), this);
        showcubeAct->setShortcuts(QKeySequence::Cut);
        showcubeAct->setStatusTip(tr("show cube"));
        showcubeAct->setCheckable(true);
        showcubeAct->setChecked(false);
        connect(showcubeAct, &QAction::triggered, this, &MainWindow::showCube);
        viewMenu->addAction(showcubeAct);
        viewToolBar->addAction(showcubeAct);

        const QIcon showbananaIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showbananaAct = new QAction(showbananaIcon, tr("show banana"), this);
        showbananaAct->setShortcuts(QKeySequence::Cut);
        showbananaAct->setStatusTip(tr("show banana"));
        showbananaAct->setCheckable(true);
        showbananaAct->setChecked(false);
        connect(showbananaAct, &QAction::triggered, this, &MainWindow::showBanana);
        viewMenu->addAction(showbananaAct);
        viewToolBar->addAction(showbananaAct);

        const QIcon showtorusIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showtorusAct = new QAction(showtorusIcon, tr("show torus"), this);
        showtorusAct->setShortcuts(QKeySequence::Cut);
        showtorusAct->setStatusTip(tr("show torus"));
        showtorusAct->setCheckable(true);
        showtorusAct->setChecked(false);
        connect(showtorusAct, &QAction::triggered, this, &MainWindow::showTorus);
        viewMenu->addAction(showtorusAct);
        viewToolBar->addAction(showtorusAct);

        const QIcon showcube2Icon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showcube2Act = new QAction(showcube2Icon, tr("show cube2"), this);
        showcube2Act->setShortcuts(QKeySequence::Cut);
        showcube2Act->setStatusTip(tr("show cube2"));
        showcube2Act->setCheckable(true);
        showcube2Act->setChecked(false);
        connect(showcube2Act, &QAction::triggered, this, &MainWindow::showCube2);
        viewMenu->addAction(showcube2Act);
        viewToolBar->addAction(showcube2Act);


        const QIcon showauxIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *showauxAct = new QAction(showauxIcon, tr("show aux"), this);
        showauxAct->setShortcuts(QKeySequence::Cut);
        showauxAct->setStatusTip(tr("show aux"));
        showauxAct->setCheckable(true);
        showauxAct->setChecked(false);
        connect(showauxAct, &QAction::triggered, this, &MainWindow::showAuxiliary);
        viewMenu->addAction(showauxAct);
        viewToolBar->addAction(showauxAct);
    }

// tool munu
    {
        QMenu *toolMenu = menuBar()->addMenu(tr("&Tool"));
        QToolBar *toolToolBar = addToolBar(tr("Tool"));

        const QIcon auxIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *auxAct = new QAction(auxIcon, tr("Cu&t"), this);
        auxAct->setShortcuts(QKeySequence::Cut);
        auxAct->setStatusTip(tr("add Auxiliary shapes"));
        connect(auxAct, &QAction::triggered, this, &MainWindow::addAuxiliaryShapes);
        toolMenu->addAction(auxAct);
        toolToolBar->addAction(auxAct);

        const QIcon ballIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *drawAct = new QAction(ballIcon, tr("Cu&t"), this);
        drawAct->setShortcuts(QKeySequence::Cut);
        drawAct->setStatusTip(tr("get joint indices"));
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


bool MainWindow::saveFile(const QString &fileName)
{
//    QFile file(fileName);
//    if (!file.open(QFile::WriteOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("SDI"),
//                             tr("Cannot write file %1:\n%2.")
//                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
//        return false;
//    }

//    QTextStream out(&file);
//    QApplication::setOverrideCursor(Qt::WaitCursor);
//    out << textEdit->toPlainText();
//    QApplication::restoreOverrideCursor();

//    setCurrentFile(fileName);
//    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
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
