#include "searchwindowbase.h"
#include "utils.h"
#include "orion/helpers/OriLayouts.h"

#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QPushButton>
#include <QSplitter>
#include <QTextBrowser>

SearchWindowBase::SearchWindowBase(QWidget *parent) : QWidget(parent)
{
    _searchBox = new QLineEdit;
    _titleResults = Utils::makeTitle("Results:");
    _infoPanel = new QTextBrowser;

    auto findByTagsButton = new QPushButton("by tags");
    connect(findByTagsButton, &QPushButton::clicked, this, &SearchWindowBase::findByTags);
    auto findByUidButton = new QPushButton("by UID");
    connect(findByUidButton, &QPushButton::clicked, this, &SearchWindowBase::findByUid);

    _resultsList = new QListWidget;
    _resultsList->setAlternatingRowColors(true);
    _resultsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_resultsList, &QListWidget::customContextMenuRequested, this, &SearchWindowBase::resultsContextMenuRequested);
    connect(_resultsList, &QListWidget::currentItemChanged, this, &SearchWindowBase::resultSelected);

    auto searchPanel = new QWidget;
    Ori::Layouts::LayoutV({
                              Utils::makeTitle("Search:"),
                              Ori::Layouts::LayoutH({
                                  _searchBox,
                                  findByTagsButton,
                                  findByUidButton,
                              }).setSpacing(3),
                              Ori::Layouts::Space(6),
                              _titleResults,
                              _resultsList,
                          })
            .setMargin(0)
            .setSpacing(0)
            .useFor(searchPanel);

    auto splitter = new QSplitter;
    splitter->addWidget(searchPanel);
    splitter->addWidget(_infoPanel);

    Ori::Layouts::LayoutV({splitter})
            .setMargin(0)
            .setSpacing(0)
            .useFor(this);

    _resultsContextMenu = new QMenu(this);
    _resultsContextMenuHeader = makeHeaderItem(_resultsContextMenu);
    _resultsContextMenu->addSeparator();
    _resultsContextMenu->addAction("Copy UID", this, &SearchWindowBase::copyEnvUid);
    _resultsContextMenu->addSeparator();
    _resultsContextMenu->addAction("Edit meta", this, &SearchWindowBase::editMeta);
}

void SearchWindowBase::setTitleAndIcon(const QString& title, const char* iconPath)
{
    setWindowTitle(title);
    setWindowIcon(QIcon(iconPath));
}

QAction* SearchWindowBase::makeHeaderItem(QMenu* menu)
{
    QAction* item = menu->addAction("");
    auto font = item->font();
    font.setBold(true);
    font.setPointSize(font.pointSize()+2);
    item->setFont(font);
    item->setEnabled(false);
    return item;
}

void SearchWindowBase::resultsContextMenuRequested(const QPoint &pos)
{
    auto popupItem = _resultsList->itemAt(pos);
    if (!popupItem) return;

    _resultsContextMenuHeader->setText(uidOf(popupItem));
    _resultsContextMenu->popup(_resultsList->mapToGlobal(pos));
}

QString SearchWindowBase::selectedUid() const
{
    auto items = _resultsList->selectedItems();
    return items.isEmpty()? QString() : uidOf(items.first());
}

QString SearchWindowBase::uidOf(QListWidgetItem* item) const
{
    return item->data(Qt::UserRole).toString();
}

void SearchWindowBase::copyEnvUid() const
{
    QApplication::clipboard()->setText(selectedUid());
}

QString SearchWindowBase::searchText() const
{
    return _searchBox->text().trimmed();
}

QString SearchWindowBase::formatValueHtml(const QString& name, const QString& value)
{
    return QString("<b>%1</b>: %2").arg(name, value);
}

void SearchWindowBase::cleanResults()
{
    _resultsList->clear();
}

void SearchWindowBase::addResult(const QString& uid, const QString& text)
{
    auto item = new QListWidgetItem(_resultsList);
    item->setText(text);
    item->setToolTip(uid);
    item->setData(Qt::UserRole, uid);
    _resultsList->addItem(item);
}
