#include "texteditorwindow.h"

#include "appevents.h"
#include "helpers/OriWidgets.h"
#include "helpers/OriLayouts.h"

#include <QDebug>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerjson.h>

TextEditorWindow::TextEditorWindow(const QString &fileName, const QString &editorTitle, QWidget *parent) : QWidget(parent)
{
    setWindowTitle(editorTitle.isEmpty() ? "Editor" : editorTitle);
    setWindowIcon(QIcon(":/icon/editor"));

    _fileName = fileName;

    // Make title line
    auto fileNameEditor = new QLineEdit;
    auto f = fileNameEditor->font();
    f.setBold(true);
    fileNameEditor->setFont(f);
    fileNameEditor->setReadOnly(true);
    fileNameEditor->setStyleSheet(QString("QLineEdit { border-style: none; background: %1; padding: 6px }")
        .arg(palette().color(QPalette::Window).name()));
    fileNameEditor->setText(_fileName);

    // Make editor
    _editor = new QsciScintilla;
    _editor->setFolding(QsciScintilla::PlainFoldStyle);
    Ori::Gui::setFontMonospace(_editor);
    QsciLexer *lexer = nullptr;
    auto ext = QFileInfo(_fileName).suffix();
    if (ext.compare("json", Qt::CaseInsensitive) == 0)
        lexer = new QsciLexerJSON(this);
    else if (ext.compare("sh", Qt::CaseInsensitive) == 0)
        lexer = new QsciLexerBash(this);
    if (lexer)
    {
        lexer->setFont(_editor->font());
        _editor->setLexer(lexer);
    }

    // Make tool buttons
    auto buttonSave = new QPushButton("Save");
    connect(buttonSave, &QPushButton::clicked, this, &TextEditorWindow::save);
    auto buttonReload = new QPushButton("Reload");
    connect(buttonReload, &QPushButton::clicked, this, &TextEditorWindow::reload);

    // Make layout
    Ori::Layouts::LayoutV({
                              Ori::Layouts::LayoutH({
                                  fileNameEditor,
                                  buttonSave,
                                  buttonReload,
                              })
                                .setMargin(3)
                                .setSpacing(3),
                              _editor
                          })
            .setMargin(0)
            .setSpacing(0)
            .useFor(this);

    reload();
}

void TextEditorWindow::reload()
{
    QFile file(_fileName);
    if (!file.exists())
        return AppEvents::error(QString("File not found: %1").arg(_fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return AppEvents::error(QString("Unable to open file %1: %2").arg(_fileName).arg(file.errorString()));
    _editor->setText(QString::fromUtf8(file.readAll()));
}

void TextEditorWindow::save()
{
    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return AppEvents::error(QString("Unable to create file %1: %2").arg(_fileName).arg(file.errorString()));
    if (file.write(_editor->text().toUtf8()) == -1)
        return AppEvents::error(QString("Failed writing into file %1: %2").arg(_fileName).arg(file.errorString()));
}

