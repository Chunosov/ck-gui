#include "mainwindow.h"

#include "appevents.h"
#include "ck.h"
#include "searchreposwindow.h"
#include "searchenvswindow.h"
#include "searchpackageswindow.h"
#include "searchprogramswindow.h"
#include "texteditorwindow.h"
#include "widgets/OriMdiToolBar.h"
#include "tools/OriSettings.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QStandardPaths>
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

    // set initial window geometry
    adjustSize();
    auto desktop = QApplication::desktop()->availableGeometry(this);
    resize(desktop.width()*3/4, desktop.height()*3/4);
    move(desktop.center() - rect().center());

    // open first tool window
    QTimer::singleShot(100, this, &MainWindow::runFirstCommand);
}

void MainWindow::createMenu()
{
    QMenu* m;

    menuBar()->setNativeMenuBar(false);

    m = menuBar()->addMenu(tr("&Tool"));
    m->addAction(QIcon(":/icon/repos"), tr("Repos"), this, SLOT(openReposWindow()));
    m->addAction(QIcon(":/icon/envs"), tr("Envs"), this, SLOT(openEnvsWindow()));
    m->addAction(QIcon(":/icon/packages"), tr("Packages"), this, SLOT(openPackagesWindow()));
    m->addAction(QIcon(":/icon/programs"), tr("Programs"), this, SLOT(openProgramsWindow()));
}

void MainWindow::createToolBars()
{
    addToolBar(Qt::BottomToolBarArea, new Ori::Widgets::MdiToolBar(tr("Windows"), _mdiArea));
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

void MainWindow::runFirstCommand()
{
    // Check config before first run
    if (!checkConfigSettings())
    {
        close();
        return;
    }

    openEnvsWindow();
}

bool MainWindow::checkConfigSettings()
{
    Ori::Settings settings;
    settings.beginDefaultGroup();
    // Should be enough wneh ck is installed via pip and is in PATH
    auto reposPath = settings.strValue("ckReposPath");
    if (reposPath.isEmpty())
    {
        // Try to find in HOME dir at first
        reposPath = QStandardPaths::locate(QStandardPaths::HomeLocation, "CK", QStandardPaths::LocateDirectory);
        if (reposPath.isEmpty())
        {
            // Ask user if repos are in non-standard location
            QString reposPath = QFileDialog::getExistingDirectory(this, "Select root forlder of ck-repositories");
            if (reposPath.isEmpty()) return false;
        }
        if (!reposPath.isEmpty())
            settings.setValue("ckReposPath", reposPath);
    }
    return true;
}
