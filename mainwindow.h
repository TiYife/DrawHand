#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "panel.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

//! [class definition with macro]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
//! [class definition with macro]
    explicit MainWindow(const QString &fileName);
    ~MainWindow();

    void tile(const QMainWindow *previous);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    void batch();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();

    void addAuxiliaryShapes();
    void findKeyIndices();
    void drawBall();

    void showMaskImage(bool checked);
    void showHand(bool checked);
    void showBall(bool checked);
    void showCube(bool checked);
    void showBanana(bool checked);
    void showCube2(bool checked);
    void showTorus(bool checked);
    void showAuxiliary(bool checked);
private:
    enum { MaxRecentFiles = 5 };

    void init();
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    void loadFile(const QString &fileName);
    static bool hasRecentFiles();
    void prependToRecentFiles(const QString &fileName);
    void setRecentFilesVisible(bool visible);
    bool saveFile(const QString &dir);
    void setCurrentFile(const QString &fileName);
    static QString strippedName(const QString &fullFileName);
    MainWindow *findMainWindow(const QString &fileName) const;

//    QTextEdit *textEdit;
    Panel * panel;

    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentFileSeparator;
    QAction *recentFileSubMenuAct;

    QString curFile;
    bool isUntitled;

    QString filename_;
    int temp_no_{0};
};

#endif // MAINWINDOW_H
