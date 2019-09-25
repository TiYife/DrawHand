
#include <QtWidgets>
#include <string>
#include "mainwindow.h"
#include "fileutil.h"

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
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    MainWindow *other = new MainWindow;
    other->tile(this);
    other->show();
}

void MainWindow::open()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        openFile(fileName);
}

void MainWindow::openFile(const QString &fileName){
    //todo change to loadobj
    MainWindow *existing = findMainWindow(fileName);
    if (existing) {
        existing->show();
        existing->raise();
        existing->activateWindow();
        return;
    }

    if (isUntitled && !isWindowModified()) {
        loadFile(fileName);
        return;
    }

    MainWindow *other = new MainWindow(fileName);
//    if (other->isUntitled) {
//        delete other;
//        return;
//    }
    other->tile(this);
    other->show();
}

bool MainWindow::save()
{
    return isUntitled ? saveAs() : saveFile(curFile);
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

void MainWindow::findIndices()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;

    string s = "D:/Documents/Projects/QT/DrawHand/resource/ori-objs/hand.obj";
    string s2 = fileName.toStdString();
    Mesh ori = FileUtil::LoadObj(s, "");
    std::cout<<s2<<std::endl;
    Mesh mesh = FileUtil::LoadObj(s2, "");

    std::vector<int> indices = ori.FindLostIndices(mesh);
    for(auto & i: indices){
        std::cout<<i<<std::endl;
    }
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
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
//! [implicit tr context]
    QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/resource/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/resource/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

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

    QMenu *recentMenu = fileMenu->addMenu(tr("Recent..."));
    connect(recentMenu, &QMenu::aboutToShow, this, &MainWindow::updateRecentFileActions);
    recentFileSubMenuAct = recentMenu->menuAction();

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = recentMenu->addAction(QString(), this, &MainWindow::openRecentFile);
        recentFileActs[i]->setVisible(false);
    }

    recentFileSeparator = fileMenu->addSeparator();

    setRecentFilesVisible(MainWindow::hasRecentFiles());

    QAction *closeAct = fileMenu->addAction(tr("&Close"), this, &QWidget::close);
    closeAct->setShortcut(tr("Ctrl+W"));
    closeAct->setStatusTip(tr("Close this window"));

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), qApp, &QApplication::closeAllWindows);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    QMenu *toolMenu = menuBar()->addMenu(tr("&Tool"));
    QToolBar *toolToolBar = addToolBar(tr("Tool"));

#ifndef QT_NO_CLIPBOARD
    const QIcon findIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
    QAction *findAct = new QAction(findIcon, tr("Cu&t"), this);
    findAct->setShortcuts(QKeySequence::Cut);
    findAct->setStatusTip(tr("get joint indices"));
    connect(findAct, &QAction::triggered, this, &MainWindow::findIndices);
    toolMenu->addAction(findAct);
    toolToolBar->addAction(findAct);

//    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
//    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
//    copyAct->setShortcuts(QKeySequence::Copy);
//    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
//                             "clipboard"));
//    connect(copyAct, &QAction::triggered, textEdit, &QTextEdit::copy);
//    editMenu->addAction(copyAct);
//    editToolBar->addAction(copyAct);

//    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
//    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
//    pasteAct->setShortcuts(QKeySequence::Paste);
//    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
//                              "selection"));
//    connect(pasteAct, &QAction::triggered, textEdit, &QTextEdit::paste);
//    editMenu->addAction(pasteAct);
//    editToolBar->addAction(pasteAct);

    menuBar()->addSeparator();
#endif // !QT_NO_CLIPBOARD

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

#ifndef QT_NO_CLIPBOARD
//    cutAct->setEnabled(false);
//    copyAct->setEnabled(false);
//    connect(textEdit, &QTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
//    connect(textEdit, &QTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
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

bool MainWindow::maybeSave()
{
//    if (!textEdit->document()->isModified())
//        return true;
//    const QMessageBox::StandardButton ret
//        = QMessageBox::warning(this, tr("SDI"),
//                               tr("The document has been modified.\n"
//                                  "Do you want to save your changes?"),
//                               QMessageBox::Save | QMessageBox::Discard
//                               | QMessageBox::Cancel);
//    switch (ret) {
//    case QMessageBox::Save:
//        return save();
//    case QMessageBox::Cancel:
//        return false;
//    default:
//        break;
//    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
//    std::string s = fileName.toLocal8Bit().constData();
    QString s = fileName;
    Mesh mesh = FileUtil::LoadObj(s.toStdString(), "");

    panel->setMesh(&mesh);
//    QFile file(fileName);
//    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("SDI"),
//                             tr("Cannot read file %1:\n%2.")
//                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
//        return;
//    }

//    QTextStream in(&file);
//    QApplication::setOverrideCursor(Qt::WaitCursor);
//    textEdit->setPlainText(in.readAll());
//    QApplication::restoreOverrideCursor();

//    setCurrentFile(fileName);
//    statusBar()->showMessage(tr("File loaded"), 2000);
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

void MainWindow::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for ( ; i < count; ++i) {
        const QString fileName = MainWindow::strippedName(recentFiles.at(i));
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for ( ; i < MaxRecentFiles; ++i)
        recentFileActs[i]->setVisible(false);
}

void MainWindow::openRecentFile()
{
    if (const QAction *action = qobject_cast<const QAction *>(sender()))
        openFile(action->data().toString());
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
