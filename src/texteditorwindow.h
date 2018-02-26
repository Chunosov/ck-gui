#ifndef TEXTEDITORWINDOW_H
#define TEXTEDITORWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPlainTextEdit;
QT_END_NAMESPACE

class TextEditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditorWindow(const QString& fileName, const QString& editorTitle, QWidget *parent = nullptr);

private:
    QString _fileName;
    QPlainTextEdit* _editor;

    void reload();
    void save();
};

#endif // TEXTEDITORWINDOW_H
