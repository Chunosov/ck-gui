#include "searchenvswindow.h"

#include "appevents.h"
#include "ck.h"

#include <QMenu>
#include <QTextBrowser>

SearchEnvsWindow::SearchEnvsWindow(QWidget *parent) : SearchWindowBase(parent)
{
    setTitleAndIcon("Envs", ":/icon/envs");

    _resultsContextMenu->addAction("Edit script", this, &SearchEnvsWindow::editEnvScript);
    _resultsContextMenu->addSeparator();
    _resultsContextMenu->addAction("Refresh env", this, &SearchEnvsWindow::refreshEnv);

    populateEnvs("", "");
}

void SearchEnvsWindow::populateEnvs(const QString& tags, const QString& uid)
{
    CK ck;
    cleanResults();
    for (const CkEntry& env : ck.queryEnvsByTags(tags))
    {
        if (!uid.isEmpty())
        {
            if (!env.uid.contains(uid)) continue;
        }
        addResult(env.uid, env.name);
    }
}

void SearchEnvsWindow::findByTags()
{
    populateEnvs(searchText(), "");
}

void SearchEnvsWindow::findByUid()
{
    populateEnvs("", searchText());
}

void SearchEnvsWindow::resultSelected(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)
    if (!current) return;

    QString uid = uidOf(current);
    if (uid.isEmpty()) return;

    CkEnvMeta meta(uid);
    if (!meta.ok()) return;

    CkEnvInfo info(uid);
    if (!info.ok()) return;

    showEnvInfo(info, meta);
}

void SearchEnvsWindow::showEnvInfo(const CkEnvInfo& info, const CkEnvMeta& meta)
{
    QString s =
        "<p>" + formatValueHtml("uid", info.backup_data_uid()) +
        "<p>" + formatValueHtml("name", info.data_name()) +
        "<p>" + formatValueHtml("full_path", meta.full_path());
    _infoPanel->setHtml(s);
}

void SearchEnvsWindow::editEnvScript()
{
    AppEvents::requestTextEditor(CK::envScriptPath(selectedUid()));
}

void SearchEnvsWindow::editMeta()
{
    AppEvents::requestTextEditor(CK::envMetaPath(selectedUid()));
}

void SearchEnvsWindow::refreshEnv()
{
    CK().refreshEnv(selectedUid());
}
