#ifndef TEXT_EDITOR_WINDOW_H
#define TEXT_EDITOR_WINDOW_H

#include <QWidget>

class QsciScintilla;

class TextEditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditorWindow(const QString& fileName, const QString& editorTitle, QWidget *parent = nullptr);

private:
    QString _fileName;
    QsciScintilla* _editor;

    void reload();
    void save();
};

#endif // TEXT_EDITOR_WINDOW_H
