#include "appevents.h"

#include <QDebug>
#include <QFile>

AppEvents* AppEvents::instance()
{
    static AppEvents obj;
    return &obj;
}

void AppEvents::info(const QString& msg)
{
    qInfo() << msg;
    emit instance()->onInfo(msg);
}

void AppEvents::warning(const QString& msg)
{
    qWarning() << msg;
}

void AppEvents::error(const QString& msg)
{
    qCritical() << "ERROR:" << msg;
    emit instance()->onError(msg);
}

void AppEvents::requestTextEditor(const QString& fileName, const QString &editorTitle)
{
    if (fileName.isEmpty())
    {
        error("File name is not set");
        return;
    }
    if (!QFile::exists(fileName))
    {
        error(QString("File does not exist: %1").arg(fileName));
        return;
    }
    emit instance()->textEditorRequested(fileName, editorTitle);
}
