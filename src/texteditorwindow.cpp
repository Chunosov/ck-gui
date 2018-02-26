#include "texteditorwindow.h"

#include "utils.h"
#include "orion/helpers/OriWidgets.h"
#include "orion/helpers/OriLayouts.h"

#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

TextEditorWindow::TextEditorWindow(const QString &fileName, const QString &editorTitle, QWidget *parent) : QWidget(parent)
{
    setWindowTitle(editorTitle.isEmpty() ? "Editor" : editorTitle);
    setWindowIcon(QIcon(":/icon/editor"));

    _fileName = fileName;

    auto fileNameEditor = new QLineEdit;
    auto f = fileNameEditor->font();
    f.setBold(true);
    fileNameEditor->setFont(f);
    fileNameEditor->setReadOnly(true);
    fileNameEditor->setStyleSheet(QString("QLineEdit { border-style: none; background: %1; padding: 6px }")
        .arg(palette().color(QPalette::Window).name()));
    fileNameEditor->setText(_fileName);

    _editor = new QPlainTextEdit;
    _editor->setWordWrapMode(QTextOption::NoWrap);
    Ori::Gui::setFontMonospace(_editor);

    auto buttonSave = new QPushButton("Save");
    connect(buttonSave, &QPushButton::clicked, this, &TextEditorWindow::save);
    auto buttonReload = new QPushButton("Reload");
    connect(buttonReload, &QPushButton::clicked, this, &TextEditorWindow::reload);

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
    _editor->setPlainText(QString::fromUtf8(Utils::loadTextFromFile(_fileName)));
}

void TextEditorWindow::save()
{
    Utils::saveTextToFile(_fileName, _editor->toPlainText().toUtf8());
}

