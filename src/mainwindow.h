#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMdiArea;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QMdiArea* _mdiArea;

    void createMenu();
    void createToolBars();
    void onError(const QString& msg);
    void openReposWindow();
    void openEnvsWindow();
    void openPackagesWindow();
    void openProgramsWindow();
    void textEditorRequested(const QString& fileName, const QString &editorTitle);
    void showSubWindow(QWidget* w);
    void runFirstCommand();
    bool checkConfigSettings();
};

#endif // MAINWINDOW_H
