#include "mainwindow.h"

#include "appevents.h"
#include "ck.h"
#include "searchreposwindow.h"
#include "searchenvswindow.h"
#include "searchpackageswindow.h"
#include "searchprogramswindow.h"
#include "texteditorwindow.h"
#include "orion/widgets/OriMdiToolBar.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/icon/main"));

    connect(AppEvents::instance(), &AppEvents::onError, this, &MainWindow::onError);
    connect(AppEvents::instance(), &AppEvents::textEditorRequested, this, &MainWindow::textEditorRequested);

    _mdiArea = new QMdiArea;
    setCentralWidget(_mdiArea);

    createMenu();
    createToolBars();
    createDocks();

    // set initial window geometry
    adjustSize();
    auto desktop = QApplication::desktop()->availableGeometry(this);
    resize(desktop.width()*0.75, desktop.height()*0.75);
    move(desktop.center() - rect().center());

    // open first tool window
    QTimer::singleShot(100, this, &MainWindow::openEnvsWindow);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenu()
{
    QMenu* m;

    menuBar()->setNativeMenuBar(false);

    m = menuBar()->addMenu(tr("&Tool"));
    m->addAction(QIcon(":/icon/repos"), tr("Repos"), this, &MainWindow::openReposWindow);
    m->addAction(QIcon(":/icon/envs"), tr("Envs"), this, &MainWindow::openEnvsWindow);
    m->addAction(QIcon(":/icon/packages"), tr("Packages"), this, &MainWindow::openPackagesWindow);
    m->addAction(QIcon(":/icon/programs"), tr("Programs"), this, &MainWindow::openProgramsWindow);
}

void MainWindow::createToolBars()
{
    addToolBar(Qt::BottomToolBarArea, new Ori::Widgets::MdiToolBar(tr("Windows"), _mdiArea));
}

void MainWindow::createDocks()
{
}

void MainWindow::onError(const QString& msg)
{
    QMessageBox::critical(this, windowTitle(), msg);
}

void MainWindow::openReposWindow()
{
    showSubWindow(new SearchReposWindow());
}

void MainWindow::openEnvsWindow()
{
    showSubWindow(new SearchEnvsWindow());
}

void MainWindow::openPackagesWindow()
{
    showSubWindow(new SearchPackagesWindow());
}

void MainWindow::openProgramsWindow()
{
    showSubWindow(new SearchProgramsWindow());
}

void MainWindow::textEditorRequested(const QString& fileName, const QString& editorTitle)
{
    showSubWindow(new TextEditorWindow(fileName, editorTitle));
}

void MainWindow::showSubWindow(QWidget* w)
{
    auto subwindow = _mdiArea->addSubWindow(w);
    auto searchWindow = qobject_cast<SearchWindowBase*>(w);
    if (searchWindow) searchWindow->applyFeatures();
    subwindow->setWindowIcon(w->windowIcon());
    subwindow->resize(_mdiArea->size() * 0.7);
    subwindow->move(_mdiArea->rect().center() - subwindow->rect().center());
    subwindow->show();
}
