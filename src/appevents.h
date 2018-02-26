#ifndef APPEVENTS_H
#define APPEVENTS_H

#include <QObject>

class AppEvents : public QObject
{
    Q_OBJECT

public:
    static AppEvents* instance();

    static void info(const QString& msg);
    static void warning(const QString& msg);
    static void error(const QString& msg);

    static void requestTextEditor(const QString& fileName, const QString& editorTitle);

signals:
    void onInfo(const QString& msg);
    void onError(const QString& msg);

    void textEditorRequested(const QString& fileName, const QString& editorTitle);

private:
    AppEvents() {}
};

#endif // APPEVENTS_H

