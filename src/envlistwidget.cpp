#include "envlistwidget.h"

#include "appevents.h"
#include "ck.h"
#include "utils.h"
#include "orion/helpers/OriLayouts.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QPushButton>
#include <QSplitter>
#include <QTextBrowser>

EnvListWidget::EnvListWidget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Envs");
    setWindowIcon(QIcon(":/icon/envs"));

    _searchBox = new QLineEdit;

    _envList = new QListWidget;
    _envList->setAlternatingRowColors(true);
    _envList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_envList, &QListWidget::customContextMenuRequested, this, &EnvListWidget::envListContextMenuRequested);
    connect(_envList, &QListWidget::currentItemChanged, this, &EnvListWidget::envSelected);

    auto findByTagsButton = new QPushButton("by tags");
    connect(findByTagsButton, &QPushButton::clicked, this, &EnvListWidget::findByTags);
    auto findByUidButton = new QPushButton("by UID");
    connect(findByUidButton, &QPushButton::clicked, this, &EnvListWidget::findByUid);

    _titleResults = Utils::makeTitle("Results:");

    auto envListPanel = new QWidget;

    Ori::Layouts::LayoutV({
                              Utils::makeTitle("Search:"),
                              Ori::Layouts::LayoutH({
                                  _searchBox,
                                  findByTagsButton,
                                  findByUidButton,
                              }).setSpacing(3),
                              Ori::Layouts::Space(6),
                              _titleResults,
                              _envList,
                          })
            .setMargin(0)
            .setSpacing(0)
            .useFor(envListPanel);

    _envInfoPanel = new QTextBrowser;

    auto splitter = new QSplitter;
    splitter->addWidget(envListPanel);
    splitter->addWidget(_envInfoPanel);

    Ori::Layouts::LayoutV({splitter})
            .setMargin(0)
            .setSpacing(0)
            .useFor(this);

    _envListContextMenu = new QMenu(this);
    _envListContextMenuHeader = makeHeaderItem(_envListContextMenu);
    _envListContextMenu->addSeparator();
    _envListContextMenu->addAction("Copy UID", this, &EnvListWidget::copyEnvUid);
    _envListContextMenu->addSeparator();
    _envListContextMenu->addAction("Edit script", this, &EnvListWidget::editEnvScript);
    _envListContextMenu->addAction("Edit meta", this, &EnvListWidget::editEnvMeta);
    _envListContextMenu->addSeparator();
    _envListContextMenu->addAction("Refresh env", this, &EnvListWidget::refreshEnv);

    populateEnvs("", "");
}

void EnvListWidget::populateEnvs(const QString& tags, const QString& uid)
{
    CK ck;
    _envList->clear();
    for (const CkEntry& env : ck.queryEnvsByTags(tags))
    {
        if (!uid.isEmpty())
        {
            if (!env.uid.contains(uid)) continue;
        }

        auto item = new QListWidgetItem(_envList);
        item->setText(env.name);
        item->setToolTip(env.uid);
        item->setData(Qt::UserRole, env.uid);
        _envList->addItem(item);
    }
}

void EnvListWidget::findByTags()
{
    populateEnvs(_searchBox->text().trimmed(), "");
}

void EnvListWidget::findByUid()
{
    populateEnvs("", _searchBox->text().trimmed());
}

void EnvListWidget::envSelected(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)
    if (!current) return;

    QString uid = current->data(Qt::UserRole).toString();
    if (uid.isEmpty()) return;

    CkEnvMeta meta(uid);
    if (!meta.ok()) return;

    CkEnvInfo info(uid);
    if (!info.ok()) return;

    showEnvInfo(info, meta);
}

QString EnvListWidget::formatValueHtml(const QString& name, const QString& value)
{
    return QString("<b>%1</b>: %2").arg(name, value);
}

void EnvListWidget::showEnvInfo(const CkEnvInfo& info, const CkEnvMeta& meta)
{
    QString s =
        "<p>" + formatValueHtml("uid", info.backup_data_uid()) +
        "<p>" + formatValueHtml("name", info.data_name()) +
        "<p>" + formatValueHtml("full_path", meta.full_path());
    _envInfoPanel->setHtml(s);
}

QAction* EnvListWidget::makeHeaderItem(QMenu* menu)
{
    QAction* item = menu->addAction("");
    auto font = item->font();
    font.setBold(true);
    font.setPointSize(font.pointSize()+2);
    item->setFont(font);
    item->setEnabled(false);
    return item;
}

void EnvListWidget::copyEnvUid()
{
    QApplication::clipboard()->setText(selectedUid());
}

QString EnvListWidget::selectedUid()
{
    auto items = _envList->selectedItems();
    return items.isEmpty()? QString() : uidOf(items.first());
}

void EnvListWidget::envListContextMenuRequested(const QPoint &pos)
{
    auto popupItem = _envList->itemAt(pos);
    if (!popupItem) return;

    _envListContextMenuHeader->setText(uidOf(popupItem));
    _envListContextMenu->popup(_envList->mapToGlobal(pos));
}

QString EnvListWidget::uidOf(QListWidgetItem* item)
{
    return item->data(Qt::UserRole).toString();
}

void EnvListWidget::editEnvScript()
{
    AppEvents::requestTextEditor(CK::envScriptPath(selectedUid()));
}

void EnvListWidget::editEnvMeta()
{
    AppEvents::requestTextEditor(CK::envMetaPath(selectedUid()));
}

void EnvListWidget::refreshEnv()
{
    CK().refreshEnv(selectedUid());
}
