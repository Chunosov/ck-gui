#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QDockWidget;
class QMdiArea;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMdiArea* _mdiArea;
    QDockWidget *_dockEnvList;

    void createMenu();
    void createToolBars();
    void createDocks();
    void onError(const QString& msg);
    void openEnvsWindow();
    void openPackagesWindow();
    void openProgramsWindow();
    void textEditorRequested(const QString& fileName);
    void showSubWindow(QWidget* w);
};

#endif // MAINWINDOW_H
