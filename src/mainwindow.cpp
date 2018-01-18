#include "mainwindow.h"

#include "appevents.h"
#include "ck.h"
#include "envlistwidget.h"
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
    m->addAction(tr("Envs"), this, &MainWindow::openEnvsWindow);
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

void MainWindow::openEnvsWindow()
{
    showSubWindow(new EnvListWidget());
}

void MainWindow::textEditorRequested(const QString& fileName)
{
    showSubWindow(new TextEditorWindow(fileName));
}

void MainWindow::showSubWindow(QWidget* w)
{
    auto subwindow = _mdiArea->addSubWindow(w);
    subwindow->setWindowIcon(w->windowIcon());
    subwindow->resize(_mdiArea->size() * 0.7);
    subwindow->move(_mdiArea->rect().center() - subwindow->rect().center());
    subwindow->show();
}
